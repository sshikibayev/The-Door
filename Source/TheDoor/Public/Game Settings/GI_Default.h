// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "GI_Default.generated.h"


UCLASS()
class THEDOOR_API UGI_Default : public UGameInstance
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    FORCEINLINE void SetPlayerName(const FText& NewName)
    {
        PlayerName = NewName;
    }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE FText GetPlayerName() const
    {
        return PlayerName;
    }

private:
    FText PlayerName{ FText::FromString(TEXT("Unknown")) };

};
