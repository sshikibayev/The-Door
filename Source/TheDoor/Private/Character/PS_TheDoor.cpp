// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PS_TheDoor.h"

#include "Game Settings/GI_Default.h"

void APS_TheDoor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (!IsRunningDedicatedServer())
    {
        InitializePlayerNickname();
    }
}

void APS_TheDoor::BeginPlay()
{
    Super::BeginPlay();

}

void APS_TheDoor::InitializePlayerNickname()
{
    GI_Default = Cast<UGI_Default>(GetGameInstance());
    if (IsValid(GI_Default))
    {
        PlayerNickname = GI_Default->GetPlayerName();
    }
}
