// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetFocusManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Widget.h"
#include "Templates/SharedPointer.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


bool UWidgetFocusManagerSubsystem::OnPlayerAction_Implementation(const FName& ActionName)
{
	if (CurrentInputInfo)
	{
		return IPlayerOperationInterface::Execute_OnPlayerAction(CurrentInputInfo, ActionName);
	}
	return false;
}

bool UWidgetFocusManagerSubsystem::OnPlayerAxis_Implementation(const FName& AxisName, const float& AxisFloat)
{
	if (CurrentInputInfo)
	{
		return IPlayerOperationInterface::Execute_OnPlayerAxis(CurrentInputInfo, AxisName, AxisFloat);
	}
	return false;
}

UWidgetFocusManagerSubsystem* UWidgetFocusManagerSubsystem::Get(UObject* InObject)
{
	if (InObject && InObject->GetWorld())
	{
		return InObject->GetWorld()->GetSubsystem<UWidgetFocusManagerSubsystem>();
	}
	return nullptr;
}

void UWidgetFocusManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UIList = NewObject<UUIInputObject>();
}

void UWidgetFocusManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	CurrentInputInfo = nullptr;
	UIList = nullptr;
}

void UWidgetFocusManagerSubsystem::Tick(float DeltaTime)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UWidgetFocusManagerSubsystem::Tick);
	Super::Tick(DeltaTime);

	if (UKismetSystemLibrary::IsDedicatedServer(this))
	{
		return;
	}

	if (!GetCurPlayerController()) return;
	if (CurrentInputInfo && CurrentInputInfo->InputActive)
	{
		if (CurrentInputInfo->UIInputInfo.CurWidget.IsValid())
		{
			if (!UWidgetFocusManagerLib::WidgetIsDisplay(CurrentInputInfo->UIInputInfo.CurWidget.Get()))
			{
				TryUpdateCurrentWidget();
			}
		}
		else
		{
			TryUpdateCurrentWidget();
		}
	}
}

APlayerController* UWidgetFocusManagerSubsystem::GetCurPlayerController()
{
	if (!CurPlayerController)
	{
		CurPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (CurrentInputInfo && CurPlayerController)
		{
			TryUpdateCurrentInput(CurrentInputInfo,true);
		}
	}
	return CurPlayerController;
}

UUIInputObject* UWidgetFocusManagerSubsystem::GetInputInfo(class UUserWidget* TargetWidget)
{
	if (!UIList) return nullptr;
	return UIList->TryGetInputInfoByUI(TargetWidget);
}

void UWidgetFocusManagerSubsystem::TryUpdateCurrentWidget()
{
	if (!CurrentInputInfo) return;
	if (CurrentInputInfo)
	{
		UUIInputObject* Temp = CurrentInputInfo->GetValidUIInputObject_OneLevel();
		if (!Temp)
		{
			Temp = CurrentInputInfo->GetValidUIInputObject_Pre();
		}
		if (Temp)
		{
			TryUpdateCurrentInput(Temp);
			return;
		}
	}

	if (CurrentInputInfo != UIList)
	{
		TryUpdateCurrentInput(UIList);
	}
}

void UWidgetFocusManagerSubsystem::TryUpdateCurrentInput(UUIInputObject* TargetUIInputObject, bool bisForce)
{
	if (!GetCurPlayerController() || !TargetUIInputObject) return;

	if (TargetUIInputObject && (bisForce || CurrentInputInfo != TargetUIInputObject))
	{
		if (TargetUIInputObject->IsValid())
		{
			switch (TargetUIInputObject->UIInputInfo.TargetInputMode)
			{
			case EInputModeType::GameAndUI:
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetCurPlayerController(), nullptr, TargetUIInputObject->UIInputInfo.bLockMouseToViewport ? EMouseLockMode::LockOnCapture : EMouseLockMode::DoNotLock, TargetUIInputObject->UIInputInfo.bHideCursorDuringCapture);
				break;
			case EInputModeType::UIOnly:
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetCurPlayerController(), nullptr, TargetUIInputObject->UIInputInfo.bLockMouseToViewport ? EMouseLockMode::LockOnCapture : EMouseLockMode::DoNotLock);
				break;
			default:
				UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetCurPlayerController());
				break;
			}

			UIList->UpdateVisivility(TargetUIInputObject->UIInputInfo.CurWidget.Get(), TargetUIInputObject->UIInputInfo.WidgetVisibility, TargetUIInputObject->UIInputInfo.ParentWidgetVisibility, TargetUIInputObject->UIInputInfo.bNeedOtherWidgetVisibility);

			if (TargetUIInputObject->UIInputInfo.TargetInputMode != EInputModeType::GameOnly)
			{
				TryFoucsTargetWidget((CurrentInputInfo && CurrentInputInfo->UIInputInfo.CauseWidget.IsValid()) ? CurrentInputInfo->UIInputInfo.CauseWidget.Get() : TargetUIInputObject->UIInputInfo.CurWidget.Get());
			}
		}
		else
		{
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetCurPlayerController());
		}
		CurrentInputInfo = TargetUIInputObject;
		CurrentInputInfo->InputActive = true;
		if (OnCurrentInputChange.IsBound()) OnCurrentInputChange.Broadcast();
		UIList->FixList();
	}
}

void UWidgetFocusManagerSubsystem::TryRemoveWidgetFromWidget(class UUserWidget* FocusWidget /*= nullptr*/)
{
	if (!IsValid(FocusWidget) || !(FocusWidget->IsValidLowLevelFast()))
	{
		return;
	}
	TryRemoveUIInputObject(GetInputInfo(FocusWidget));
}

void UWidgetFocusManagerSubsystem::TryInsertWidgetFocus(const FPlayerInputSetting& PlayerInputSetting)
{
	UUIInputObject* AddedInputObject = TryAddInputInfo(PlayerInputSetting);
	if (!AddedInputObject)
	{
		//UE_LOG(LogTemp, Error, TEXT("SetPlayerInputMode_New::TryAddInputInfo falure!-->%s"), *PlayerInputSetting.FocusWidget.Get()->GetDisplayLabel());
		return;
	}
	if (PlayerInputSetting.bisAutoSet) TryUpdateCurrentInput(AddedInputObject);
}

void UWidgetFocusManagerSubsystem::TryRemoveUIInputObject(UUIInputObject* NeedRemoveUIInputObject)
{
	if (!NeedRemoveUIInputObject) return;
	if (NeedRemoveUIInputObject->CheckUIInputObjectIsInChildList(CurrentInputInfo))
	{
		UUIInputObject* T = NeedRemoveUIInputObject->GetValidUIInputObject_OneLevel();
		if (!T)
		{
			T = NeedRemoveUIInputObject->GetValidUIInputObject_Pre();
		}
		TryUpdateCurrentInput(T);
	}
	NeedRemoveUIInputObject->TryRemoveUIInputObject();
}

UUIInputObject* UWidgetFocusManagerSubsystem::TryAddInputInfo(const FPlayerInputSetting& PlayerInputSetting)
{
	if (PlayerInputSetting.NeedInsertUIList)
	{
		//该widget需要RootWidget
		if (PlayerInputSetting.RootWidget)
		{
			//查找RootWidget是否一致
			UUIInputObject* RootUIInputObject;
			RootUIInputObject = GetInputInfo(PlayerInputSetting.RootWidget);
			if (RootUIInputObject)
			{
				UUIInputObject* UIInputObjectTemp;
				UIInputObjectTemp = RootUIInputObject->TryGetInputInfoInChildWidgetList(PlayerInputSetting.FocusWidget);
				//RootWidget一致
				if (UIInputObjectTemp)
				{
					return UIInputObjectTemp;
				}
				//删除不一致的input
				else
				{
					TryRemoveUIInputObject(GetInputInfo(PlayerInputSetting.FocusWidget));
				}
				return RootUIInputObject->TryAddInputInfoInChildWidgetList(PlayerInputSetting, this);
			}
			else
			{
				FPlayerInputSetting PlayerInputSettingTemp = PlayerInputSetting;
				PlayerInputSettingTemp.FocusWidget = PlayerInputSetting.RootWidget;
				RootUIInputObject = UIList->TryAddInputInfoInChildWidgetList(PlayerInputSettingTemp, this);
				return RootUIInputObject->TryAddInputInfoInChildWidgetList(PlayerInputSetting, this);
			}
		}
		//该widget不需要RootWidget
		else
		{
			//UIList已经存在Widget
			UUIInputObject* UIInputObjectTemp = GetInputInfo(PlayerInputSetting.FocusWidget);
			if (UIInputObjectTemp)
			{
				UIInputObjectTemp->UIInputInfo.ConvertUIInputInfoFromPlayerSetting(PlayerInputSetting);
				return UIInputObjectTemp;
			}
		}

		return UIList->TryAddInputInfoInChildWidgetList(PlayerInputSetting, this);
	}
	return nullptr;
}


void UWidgetFocusManagerSubsystem::TryFoucsTargetWidget(UWidget* TargetWidget)
{
	if (TargetWidget && IsValid(TargetWidget))
	{
		TargetWidget->SetFocus();
	}
}


void UWidgetFocusManagerSubsystem::TryFoucsTargetWidget(class UUserWidget* TargetWidget)
{
	TryUpdateCurrentInput(GetInputInfo(TargetWidget));
}

UUserWidget* UWidgetFocusManagerSubsystem::GetCurrentInputWidget()
{
	if (CurrentInputInfo && CurrentInputInfo->IsValid())
	{
		return CurrentInputInfo->UIInputInfo.CurWidget.Get();
	}
	return nullptr;
}
