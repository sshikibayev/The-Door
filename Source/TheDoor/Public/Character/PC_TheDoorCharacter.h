// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_TheDoorCharacter.generated.h"


UCLASS()
class THEDOOR_API APC_TheDoorCharacter : public APlayerController
{
	GENERATED_BODY()

public:
    FORCEINLINE FText GetPlayerName()
    {
        return PlayerName;
    }

protected:
    virtual void PostInitializeComponents() override;

private:
    FText PlayerName;

};
