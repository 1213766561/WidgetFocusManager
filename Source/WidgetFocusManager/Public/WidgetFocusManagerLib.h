// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetFocusManagerLib.generated.h"

/**
 * 
 */
UCLASS()
class WIDGETFOCUSMANAGER_API UWidgetFocusManagerLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//检测Widget是否在显示
	UFUNCTION(BlueprintCallable)
	static bool WidgetIsDisplay(class UUserWidget* TargetWidget);
};
