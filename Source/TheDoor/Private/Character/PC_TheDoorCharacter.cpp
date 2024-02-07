// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PC_TheDoorCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Game Settings/GI_Default.h"


void APC_TheDoorCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (GetNetMode() == ENetMode::NM_ListenServer)
    {
        if (TObjectPtr<UGI_Default> GI_Default{ Cast<UGI_Default>(GetGameInstance()) })
        {
            PlayerName = GI_Default->GetPlayerName();
        }
    }

    if (!HasAuthority())
    {
        if (TObjectPtr<UGI_Default> GI_Default{ Cast<UGI_Default>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetGameInstance()) })
        {
            PlayerName = GI_Default->GetPlayerName();
        }
    }
}
