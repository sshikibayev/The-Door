// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "PC_TheDoor.generated.h"


UCLASS()
class THEDOOR_API APC_TheDoor : public APlayerController
{
	GENERATED_BODY()

protected:
    virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* aPawn) override;
    virtual void Tick(float DeltaSeconds) override;
};
