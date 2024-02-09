// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PS_TheDoor.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Game Settings/GI_Default.h"

void APS_TheDoor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (GetNetMode() == ENetMode::NM_ListenServer)
    {
        GI_Default = Cast<UGI_Default>(GetGameInstance());
        if (IsValid(GI_Default))
        {
           PlayerNickname = GI_Default->GetPlayerName();
        }
    }

    if (!HasAuthority())
    {
        GI_Default = Cast<UGI_Default>(GetGameInstance());
        if (IsValid(GI_Default))
        {
            PlayerNickname = GI_Default->GetPlayerName();
        }
    }
}

void APS_TheDoor::BeginPlay()
{
    Super::BeginPlay();

}

void APS_TheDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}