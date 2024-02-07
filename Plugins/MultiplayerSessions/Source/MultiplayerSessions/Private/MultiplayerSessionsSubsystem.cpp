// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"


UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() :
    CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
    FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
    JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
    DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
    StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
}

void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
    if (!IsValidSessionInterface())
    {
        return;
    }

    if (TObjectPtr<FNamedOnlineSession> ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession))
    {
        bCreateSessionOnDestroy = true;
        LastNumPublicConnections = NumPublicConnections;
        LastMatchType = MatchType;

        DestroySession();
    }

    //Store the delegate in a FDelegateHandle so we can later remove it from the delegate list.
    CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

    LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
    if (LastSessionSettings.IsValid())
    {
        LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
        LastSessionSettings->NumPublicConnections = NumPublicConnections;
        LastSessionSettings->bAllowJoinInProgress = true;
        LastSessionSettings->bAllowJoinViaPresence = true;
        LastSessionSettings->bUsesPresence = true;
        LastSessionSettings->bShouldAdvertise = true;
        LastSessionSettings->bUseLobbiesIfAvailable = true;
        LastSessionSettings->BuildUniqueId = 1;

        LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    }


    if (!GetWorld())
    {
        return;
    }

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    bool bIsSessionCreatedSuccessfully = SessionInterface->CreateSession
    (
        *LocalPlayer->GetPreferredUniqueNetId(),
        NAME_GameSession,
        *LastSessionSettings
    );

    if (!bIsSessionCreatedSuccessfully)
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

        OnCreateSessionBroadcast(false);
    }

}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
    if (!IsValidSessionInterface())
    {
        return;
    }

    FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

    LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (LastSessionSearch.IsValid())
    {
        LastSessionSearch->MaxSearchResults = MaxSearchResults;
        LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
        LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

        if (!GetWorld())
        {
            return;
        }

        if (const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
        {
            bool bWasFindSessionSuccessful = SessionInterface->FindSessions
            (
                *LocalPlayer->GetPreferredUniqueNetId(),
                LastSessionSearch.ToSharedRef()
            );

            if (!bWasFindSessionSuccessful)
            {
                SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
                OnFindSessionBroadcast(TArray<FOnlineSessionSearchResult>(), false);
            }
        }
    }
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
    if (!IsValidSessionInterface())
    {
        OnJoinSessionBroadcast(EOnJoinSessionCompleteResult::UnknownError);

        return;
    }

    JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

    if (!GetWorld())
    {
        return;
    }

    if (const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
    {
        bool bWasJoinSessionSuccessful = SessionInterface->JoinSession
        (
            *LocalPlayer->GetPreferredUniqueNetId(),
            NAME_GameSession,
            SessionResult
        );

        if (!bWasJoinSessionSuccessful)
        {
            SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
            OnJoinSessionBroadcast(EOnJoinSessionCompleteResult::UnknownError);
        }
    }
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
    if (!IsValidSessionInterface())
    {
        OnDestroySessionBroadcast(false);
        return;
    }

    DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

    if (!SessionInterface->DestroySession(NAME_GameSession))
    {
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
        OnDestroySessionBroadcast(false);
    }
}

void UMultiplayerSessionsSubsystem::StartSession()
{
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (IsValidSessionInterface())
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
    }

    //Broadcast custom delegate
    OnCreateSessionBroadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    if (IsValidSessionInterface())
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
    }

    //Broadcast custom delegate
    if (!LastSessionSearch.IsValid() || LastSessionSearch->SearchResults.Num() <= 0)
    {
        OnFindSessionBroadcast(TArray<FOnlineSessionSearchResult>(), false);
    }
    else
    {
        OnFindSessionBroadcast(LastSessionSearch->SearchResults, bWasSuccessful);
    }
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (IsValidSessionInterface())
    {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
    }

    OnJoinSessionBroadcast(Result);
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (IsValidSessionInterface())
    {
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
    }

    if (bWasSuccessful && bCreateSessionOnDestroy)
    {
        bCreateSessionOnDestroy = false;
        CreateSession(LastNumPublicConnections, LastMatchType);
    }

    OnDestroySessionBroadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

bool UMultiplayerSessionsSubsystem::IsValidSessionInterface()
{
    if (!SessionInterface)
    {
        IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
        if (Subsystem)
        {
            SessionInterface = Subsystem->GetSessionInterface();
        }
    }

    return SessionInterface.IsValid();
}

void UMultiplayerSessionsSubsystem::OnCreateSessionBroadcast(bool bStatus)
{
    if (MultiplayerOnCreateSessionComplete.IsBound())
    {
        MultiplayerOnCreateSessionComplete.Broadcast(bStatus);
    }
}

void UMultiplayerSessionsSubsystem::OnFindSessionBroadcast(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccsessful)
{
    if (MultiplayerOnFindSessionsComplete.IsBound())
    {
        MultiplayerOnFindSessionsComplete.Broadcast(SessionResults, bWasSuccsessful);
    }
}

void UMultiplayerSessionsSubsystem::OnJoinSessionBroadcast(EOnJoinSessionCompleteResult::Type Result)
{
    if (MultiplayerOnJoinSessionComplete.IsBound())
    {
        MultiplayerOnJoinSessionComplete.Broadcast(Result);
    }
}

void UMultiplayerSessionsSubsystem::OnStartSessionBroadcast(bool bWasSuccsessful)
{

}

void UMultiplayerSessionsSubsystem::OnDestroySessionBroadcast(bool bWasSuccsessful)
{
    if (MultiplayerOnDestroySessionComplete.IsBound())
    {
        MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccsessful);
    }
}
