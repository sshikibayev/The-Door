// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "PS_TheDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerNicknameSet);

class UGI_Default;

UCLASS()
class THEDOOR_API APS_TheDoor : public APlayerState
{
    GENERATED_BODY()

public:
    FOnPlayerNicknameSet OnPlayerNicknameSet;

    FORCEINLINE FText GetPlayerNickname()
    {
        return PlayerNickname;
    }

protected:
    virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    FText PlayerNickname{ FText::FromString("Unknown") };

    UPROPERTY()
    TObjectPtr<UGI_Default> GI_Default;
};
