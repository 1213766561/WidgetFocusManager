// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIInputObject.h"
#include "PlayerOperationInterface.h"
#include "WidgetFocusManagerSubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCurrentInputChange);

UCLASS(Blueprintable, BlueprintType)
class WIDGETFOCUSMANAGER_API UWidgetFocusManagerSubsystem : public UTickableWorldSubsystem, public IPlayerOperationInterface
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class APlayerController* CurPlayerController;
public:
	virtual bool OnPlayerAction_Implementation(const FName& ActionName) override;
	virtual bool OnPlayerAxis_Implementation(const FName& AxisName, const float& AxisFloat) override;

	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UWidgetFocusManagerSubsystem, STATGROUP_Tickables); };
	UFUNCTION(BlueprintCallable)
	static UWidgetFocusManagerSubsystem* Get(UObject* InObject);
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;

	APlayerController* GetCurPlayerController();

	UFUNCTION(BlueprintCallable)
	void TryInsertWidgetFocus(const FPlayerInputSetting& PlayerInputSetting);

	UPROPERTY()
	UUIInputObject* CurrentInputInfo = nullptr;

	UPROPERTY()
	UUIInputObject* UIList = nullptr;

	void TryRemoveUIInputObject(UUIInputObject* NeedRemoveUIInputObject);

	UUIInputObject* TryAddInputInfo(const FPlayerInputSetting& PlayerInputSetting);

	//By Widget
	UUIInputObject* GetInputInfo(class UUserWidget* TargetWidget);


	void TryUpdateCurrentWidget();

	void TryUpdateCurrentInput(UUIInputObject* TargetUIInputObject,bool bisForce = false);

	UFUNCTION(BlueprintCallable)
	void TryRemoveWidgetFromWidget(class UUserWidget* FocusWidget = nullptr);

	UFUNCTION(BlueprintCallable)
	void TryFoucsTargetWidget(class UUserWidget* TargetWidget);

	UFUNCTION(BlueprintCallable)
	class UUserWidget* GetCurrentInputWidget();

	void TryFoucsTargetWidget(UWidget* TargetWidget);

	UPROPERTY(BlueprintAssignable)
	FOnCurrentInputChange OnCurrentInputChange;
};
