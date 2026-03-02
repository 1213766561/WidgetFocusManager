// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerOperationInterface.h"
#include "Components/SlateWrapperTypes.h"
#include "WidgetFocusManagerLib.h"
#include "UIInputObject.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class  EInputModeType :uint8
{
	GameOnly,
	GameAndUI,
	UIOnly
};

USTRUCT(BlueprintType)
struct FPlayerInputSetting
{
	GENERATED_USTRUCT_BODY()
public:
	/*Focus目标UI*/
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* FocusWidget = nullptr;
	/*RootWidget*/
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* RootWidget = nullptr;
	/*InputMode*/
	UPROPERTY(BlueprintReadWrite)
	EInputModeType InputModeType = EInputModeType::GameAndUI;
	/*bLockMouseToViewport*/
	UPROPERTY(BlueprintReadWrite)
	bool bLockMouseToViewport = false;
	/*bHideCursorDuringCapture*/
	UPROPERTY(BlueprintReadWrite)
	bool bHideCursorDuringCapture = true;
	/*是否塞入UI列表管理*/
	UPROPERTY(BlueprintReadWrite)
	bool NeedInsertUIList = true;
	/*其他界面显示规则*/
	UPROPERTY(BlueprintReadWrite)
	ESlateVisibility OtherWidgetVisibility = ESlateVisibility::SelfHitTestInvisible;
	/*当前界面显示规则*/
	UPROPERTY(BlueprintReadWrite)
	ESlateVisibility WidgetVisibility = ESlateVisibility::SelfHitTestInvisible;
	/*其他界面显示规则是否应用*/
	UPROPERTY(BlueprintReadWrite)
	bool bNeedOtherWidgetVisibility = true;
	/*当前界面是由什么button等小组件界面触发CasueSWidget*/
	UPROPERTY(BlueprintReadWrite)
	class UWidget* CasueSWidget = nullptr;
	/*是否立即应用以上设置*/
	UPROPERTY(BlueprintReadWrite)
	bool bisAutoSet = true;
};

USTRUCT(BlueprintType)
struct FUIInputInfo
{
	GENERATED_BODY()
public:
	TWeakObjectPtr<UUserWidget> CurWidget;
	EInputModeType TargetInputMode = EInputModeType::GameOnly;
	ESlateVisibility ParentWidgetVisibility = ESlateVisibility::SelfHitTestInvisible;
	ESlateVisibility WidgetVisibility = ESlateVisibility::SelfHitTestInvisible;
	bool bLockMouseToViewport = false;
	bool bHideCursorDuringCapture = true;
	bool bNeedOtherWidgetVisibility = true;

	TWeakObjectPtr<UWidget> CauseWidget = nullptr;

	void ConvertUIInputInfoFromPlayerSetting(const FPlayerInputSetting& PlayerInputSetting)
	{
		CurWidget = PlayerInputSetting.FocusWidget;
		TargetInputMode = PlayerInputSetting.InputModeType;
		bHideCursorDuringCapture = PlayerInputSetting.bHideCursorDuringCapture;
		bLockMouseToViewport = PlayerInputSetting.bLockMouseToViewport;
		ParentWidgetVisibility = PlayerInputSetting.OtherWidgetVisibility;
		bNeedOtherWidgetVisibility = PlayerInputSetting.bNeedOtherWidgetVisibility;
		WidgetVisibility = PlayerInputSetting.WidgetVisibility;
		CauseWidget = PlayerInputSetting.CasueSWidget;
	};

};

UCLASS()
class WIDGETFOCUSMANAGER_API UUIInputObject : public UObject, public IPlayerOperationInterface
{
	GENERATED_BODY()
public:
	//~UUIInputObject()
	//{
	//	if (UIInputInfo.CurWidget.IsValid())
	//	{
	//		if (UWidgetFocusManagerLib::WidgetIsDisplay(UIInputInfo.CurWidget.Get()))
	//		{
	//			UIInputInfo.CurWidget.Get()->RemoveFromParent();
	//		}
	//		UIInputInfo.CurWidget = nullptr;
	//	}
	//};

	virtual bool OnPlayerAction_Implementation(const FName& ActionName) override;
	virtual bool OnPlayerAxis_Implementation(const FName& AxisName, const float& AxisFloat) override;

	UPROPERTY()
	TArray<UUIInputObject*> ChildWidgetList;
	UPROPERTY()
	UUIInputObject* ParentUIInputObject;
	UPROPERTY()
	FUIInputInfo UIInputInfo;
	
	bool InputActive = false;

	bool IsValid()
	{
		return UIInputInfo.CurWidget.IsValid();
	};

	UUIInputObject* TryGetInputInfoInChildWidgetList(UUserWidget* TargetWidget)
	{
		if (!TargetWidget) return nullptr;
		for (auto& T : ChildWidgetList)
		{
			if (!T->UIInputInfo.CurWidget.IsValid()) continue;
			if (T->UIInputInfo.CurWidget.Get() == TargetWidget) return T;
		}

		return nullptr;
	}

	void UpdateVisivility(UUserWidget* TargetWidget, ESlateVisibility TargetVisibility, ESlateVisibility OtherVisibility,bool OtherVisibilityIsValid)
	{
		if (UIInputInfo.CurWidget.IsValid())
		{
			if (UIInputInfo.CurWidget != TargetWidget)
			{
				if (OtherVisibilityIsValid)
				{
					UIInputInfo.CurWidget.Get()->SetVisibility(OtherVisibility);

				}
			}
			else
			{
				UIInputInfo.CurWidget.Get()->SetVisibility(TargetVisibility);
			}
		}
		for (auto T : ChildWidgetList)
		{
			T->UpdateVisivility(TargetWidget, TargetVisibility, OtherVisibility, OtherVisibilityIsValid);
		}
	};

	UUIInputObject* TryGetInputInfoByUI(UUserWidget* TargetWidget)
	{
		if (UIInputInfo.CurWidget == TargetWidget)
		{
			return this;
		}
		for (auto& T : ChildWidgetList)
		{
			if (T->TryGetInputInfoByUI(TargetWidget)) return T->TryGetInputInfoByUI(TargetWidget);
		}
		return nullptr;
	}

	UUIInputObject* TryAddInputInfoInChildWidgetList(const FPlayerInputSetting& PlayerInputSetting,UObject* OwnerObject)
	{
		if (!OwnerObject) return nullptr;
		UUIInputObject* Temp = NewObject<UUIInputObject>(OwnerObject);
		Temp->UIInputInfo.ConvertUIInputInfoFromPlayerSetting(PlayerInputSetting);
		Temp->ParentUIInputObject = this;
		ChildWidgetList.Add(Temp);
		return Temp;
	};

	void TryRemoveAllUIFromPanel()
	{
		for (auto T : ChildWidgetList)
		{
			T->TryRemoveAllUIFromPanel();
		}
		if (UIInputInfo.CurWidget.IsValid() && UWidgetFocusManagerLib::WidgetIsDisplay(UIInputInfo.CurWidget.Get()))
		{
			UIInputInfo.CurWidget.Get()->RemoveFromParent();
		}
	}

	void TryRemoveUIInputObject()
	{
		TryRemoveAllUIFromPanel();
		if (!ParentUIInputObject) return;
		if (ParentUIInputObject->ChildWidgetList.Contains(this))
		{
			ParentUIInputObject->ChildWidgetList.Remove(this);
			return;
		}
	};

	void FixList()
	{
		TArray<UUIInputObject*> NewList;
		for (auto T:ChildWidgetList)
		{
			if (T->IsValid())
			{
				if (T->InputActive && !UWidgetFocusManagerLib::WidgetIsDisplay(T->UIInputInfo.CurWidget.Get()))
				{
					continue;
				}
				T->FixList();
				NewList.Add(T);
			}
		}
		ChildWidgetList = NewList;
	};

	UUIInputObject* GetValidUIInputObject_OneLevel()
	{
		if (ParentUIInputObject)
		{
			for (int i = 1; i <= ParentUIInputObject->ChildWidgetList.Num(); i++)
			{
				UUIInputObject* T = ParentUIInputObject->ChildWidgetList[ParentUIInputObject->ChildWidgetList.Num() - i];
				if (T->IsValid() && T != this)
				{
					UUIInputObject* Child = T->GetValidUIInputObject_Post();
					if (Child)
					{
						return Child;
					}
					return T;
				}
			}
		}
		return nullptr;
	};

	UUIInputObject* GetValidUIInputObject_Post()
	{
		for (int i = 1; i <= ChildWidgetList.Num(); i++)
		{
			UUIInputObject* T = ChildWidgetList[ChildWidgetList.Num() - i];
			if (T->IsValid())
			{
				UUIInputObject* Child = T->GetValidUIInputObject_Post();
				if (Child)
				{
					return Child;
				}
				return T;
			}
		}
		return this;
	};

	UUIInputObject* GetValidUIInputObject_Pre()
	{
		if (ParentUIInputObject)
		{
			if (ParentUIInputObject->IsValid())
			{
				return ParentUIInputObject;
			}

			UUIInputObject* T = ParentUIInputObject->GetValidUIInputObject_OneLevel();
			if (T && T->IsValid())
			{
				return T;
			}
			return ParentUIInputObject->GetValidUIInputObject_Pre();
		}

		return nullptr;
	};

	bool CheckUIInputObjectIsInChildList(UUIInputObject* MarkedUIInputObject)
	{
		if (this == MarkedUIInputObject)
		{
			return true;
		}
		for (auto T:ChildWidgetList)
		{
			if (T->CheckUIInputObjectIsInChildList(MarkedUIInputObject)) return true;
		}
		return false;
	};
};
