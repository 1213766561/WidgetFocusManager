// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerOperationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerOperationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WIDGETFOCUSMANAGER_API IPlayerOperationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerOperation")
	bool OnPlayerAction(const FName& ActionName);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerOperation")
	bool OnPlayerAxis(const FName& AxisName,const float& AxisFloat);
};
