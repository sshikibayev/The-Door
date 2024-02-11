// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "PS_TheDoor.generated.h"

class UGI_Default;

UCLASS()
class THEDOOR_API APS_TheDoor : public APlayerState
{
    GENERATED_BODY()

public:
    FORCEINLINE FText GetPlayerNickname()
    {
        return PlayerNickname;
    }

protected:
    virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;

private:
    FText PlayerNickname{ FText::FromString("Unknown") };

    UPROPERTY()
    TObjectPtr<UGI_Default> GI_Default;

    void InitializePlayerNickname();
};
