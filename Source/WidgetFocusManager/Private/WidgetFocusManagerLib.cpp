// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFocusManagerLib.h"
#include "Blueprint/UserWidget.h"

bool UWidgetFocusManagerLib::WidgetIsDisplay(UUserWidget* TargetWidget)
{
	if (!TargetWidget) return false;

	if (!TargetWidget->IsInViewport()) return false;

	//if (TargetWidget->Visibility == ESlateVisibility::Collapsed) return false;


	return true;
}
