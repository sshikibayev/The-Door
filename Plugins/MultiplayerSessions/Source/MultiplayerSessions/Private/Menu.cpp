// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Components/Button.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/EditableText.h"

void UMenu::SetupMenu(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
    NumPublicConnections = NumberOfPublicConnections;
    MatchType = TypeOfMatch;
    PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);

    if (!IsInViewport())
    {
        AddToViewport();
        SetVisibility(ESlateVisibility::Visible);
        bIsFocusable = true;
    }

    if (!GetWorld())
    {
        return;
    }

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        FInputModeUIOnly InputModeData;
        InputModeData.SetWidgetToFocus(TakeWidget());
        InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

        PlayerController->SetInputMode(InputModeData);
        PlayerController->SetShowMouseCursor(true);
    }

    if (TObjectPtr<UGameInstance> GameInstance = GetGameInstance())
    {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
    }

    if (IsValid(MultiplayerSessionsSubsystem))
    {
        MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
        MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
        MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
        MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
        MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
    }
}

FText UMenu::GetPlayerName()
{
    if (ET_PlayerName)
    {
        return ET_PlayerName->GetText();
    }

    return FText::FromString(TEXT("Unknown"));
}

bool UMenu::Initialize()
{
    if (!Super::Initialize())
    {
        return false;
    }

    AddDelegates();

    return true;
}

void UMenu::NativeDestruct()
{
    MenuTearDown();

    Super::NativeDestruct();
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Warning, TEXT("bWasSuccessful: true"));

        if (!GetWorld())
        {
            return;
        }

        GetWorld()->ServerTravel(PathToLobby);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Session creation failed"));

        BTN_Host->SetIsEnabled(true);
    }
}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
    if (!MultiplayerSessionsSubsystem)
    {
        return;
    }

    for (auto Result : SessionResults)
    {
        FString SettingsValue;
        Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
        if (SettingsValue == MatchType)
        {
            MultiplayerSessionsSubsystem->JoinSession(Result);
            return;
        }
    }

    if (!bWasSuccessful || SessionResults.Num() <= 0)
    {
        BTN_Join->SetIsEnabled(true);
    }
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
    if (TObjectPtr<IOnlineSubsystem> Subsystem = IOnlineSubsystem::Get())
    {
        IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();

        if (SessionInterface.IsValid())
        {
            FString Address;
            SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

            if (TObjectPtr<APlayerController> PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
            {
                PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
            }
        }
    }

    if (Result != EOnJoinSessionCompleteResult::Success)
    {
        BTN_Join->SetIsEnabled(true);
    }
}

void UMenu::OnStartSession(bool bWasSuccessful)
{

}

void UMenu::OnDestroySession(bool bWasSuccessful)
{

}

void UMenu::HostBtnClicked()
{
    BTN_Host->SetIsEnabled(false);

    OnHostPressed.Broadcast();

    if (MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
    }
}

void UMenu::JoinBtnClicked()
{
    BTN_Join->SetIsEnabled(false);

    OnJoinPressed.Broadcast();

    if (MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->FindSessions(10000);
    }
}

void UMenu::QuitBtnClicked()
{
    if (GetWorld())
    {
        UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand("quit");
    }
}

void UMenu::AddDelegates()
{
    if (BTN_Host && BTN_Join)
    {
        BTN_Host->OnClicked.AddDynamic(this, &ThisClass::HostBtnClicked);
        BTN_Join->OnClicked.AddDynamic(this, &ThisClass::JoinBtnClicked);
        BTN_Quit->OnClicked.AddDynamic(this, &ThisClass::QuitBtnClicked);
    }
}

void UMenu::RemoveDelegates()
{
    if (BTN_Host && BTN_Join)
    {
        BTN_Host->OnClicked.RemoveDynamic(this, &ThisClass::HostBtnClicked);
        BTN_Join->OnClicked.RemoveDynamic(this, &ThisClass::JoinBtnClicked);
        BTN_Quit->OnClicked.RemoveDynamic(this, &ThisClass::QuitBtnClicked);
    }
}

void UMenu::CleaningPointers()
{
    BTN_Host = nullptr;
    BTN_Join = nullptr;
    ET_PlayerName = nullptr;
    MultiplayerSessionsSubsystem = nullptr;
}

void UMenu::MenuTearDown()
{
    RemoveFromParent();

    if (!GetWorld())
    {
        return;
    }

    RemoveDelegates();

    CleaningPointers();

    if (TObjectPtr<APlayerController> PlayerController = GetWorld()->GetFirstPlayerController())
    {
        FInputModeGameOnly InputModeData;
        PlayerController->SetInputMode(InputModeData);
        PlayerController->SetShowMouseCursor(false);
    }
}
