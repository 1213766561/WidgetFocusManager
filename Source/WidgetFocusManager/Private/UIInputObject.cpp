// Fill out your copyright notice in the Description page of Project Settings.


#include "UIInputObject.h"

bool UUIInputObject::OnPlayerAction_Implementation(const FName& ActionName)
{
    if(UIInputInfo.CurWidget.IsValid() &&
        UIInputInfo.CurWidget.Get()->Implements<UPlayerOperationInterface>())
    {
        if (IPlayerOperationInterface::Execute_OnPlayerAction(UIInputInfo.CurWidget.Get(), ActionName))
        {
			return true;
        };
	}
    if (ParentUIInputObject)
    {
        return ParentUIInputObject->OnPlayerAction_Implementation(ActionName);
    }
	return false;
}

bool UUIInputObject::OnPlayerAxis_Implementation(const FName& AxisName, const float& AxisFloat)
{
    if (UIInputInfo.CurWidget.IsValid() &&
        UIInputInfo.CurWidget.Get()->Implements<UPlayerOperationInterface>())
    {
        if (IPlayerOperationInterface::Execute_OnPlayerAxis(UIInputInfo.CurWidget.Get(), AxisName, AxisFloat))
        {
            return true;
        };
    }
    if (ParentUIInputObject)
    {
        return ParentUIInputObject->OnPlayerAxis_Implementation(AxisName, AxisFloat);
    }
    return false;
}
