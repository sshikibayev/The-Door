// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/PC_MainMenu.h"

#include <Menu.h>
#include "Game Settings/GI_Default.h"


void APC_MainMenu::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    CreateMainMenu();
}

void APC_MainMenu::SetupPlayerName()
{
    if (TObjectPtr<UGI_Default> GI_Default{ Cast<UGI_Default>(GetGameInstance()) })
    {
        if (IsValid(WBP_MainMenu))
        {
            GI_Default->SetPlayerName(WBP_MainMenu->GetPlayerName());
        }
    }
}

void APC_MainMenu::CreateMainMenu()
{
    if (!GetWorld())
    {
        return;
    }

    WBP_MainMenu = CreateWidget<UMenu>(GetWorld(), MainMenuClass);

    if (IsValid(WBP_MainMenu))
    {
        WBP_MainMenu->SetupMenu(NumberOfConnection, TypeOfMatch, LobbyPath);
        WBP_MainMenu->OnHostPressed.AddDynamic(this, &ThisClass::SetupPlayerName);
        WBP_MainMenu->OnJoinPressed.AddDynamic(this, &ThisClass::SetupPlayerName);
    }
}
