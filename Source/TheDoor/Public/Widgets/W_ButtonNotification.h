// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "W_ButtonNotification.generated.h"


UCLASS()
class THEDOOR_API UW_ButtonNotification : public UUserWidget
{
	GENERATED_BODY()

public:
    FORCEINLINE void ShowWidget()
    {
        SetVisibility(ESlateVisibility::Visible);
    }

    FORCEINLINE void HideWidget()
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
};
