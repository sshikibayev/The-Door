// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "PC_MainMenu.generated.h"

class UMenu;

UCLASS()
class THEDOOR_API APC_MainMenu : public APlayerController
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    TSubclassOf<UMenu> MainMenuClass;

protected:
    UPROPERTY(EditAnywhere)
    int NumberOfConnection{ 16 };
    UPROPERTY(EditAnywhere)
    FString TypeOfMatch{ "FreeForAll" };
    UPROPERTY(EditAnywhere)
    FString LobbyPath{ "/Game/Maps/StartMap" };

    virtual void PostInitializeComponents() override;

private:
    UPROPERTY()
    TObjectPtr<UMenu> WBP_MainMenu;

    UFUNCTION()
    void SetupPlayerName();

    void CreateMainMenu();
};
