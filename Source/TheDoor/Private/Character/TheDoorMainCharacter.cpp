// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TheDoorMainCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

#include "Interfaces/Interactable.h"
#include "Widgets/W_PlayerNickname.h"
#include "Character/PS_TheDoor.h"


ATheDoorMainCharacter::ATheDoorMainCharacter()
{
    BC_InteractionArea = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionArea"));
    BC_InteractionArea->SetCollisionProfileName(TEXT("OverlapAll"));
    BC_InteractionArea->SetupAttachment(GetRootComponent());

    WC_Nickname = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerNicknameWidget"));
    WC_Nickname->SetupAttachment(GetRootComponent());
    WC_Nickname->SetWidgetSpace(EWidgetSpace::Screen);
    WC_Nickname->SetupAttachment(GetRootComponent());
}

void ATheDoorMainCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    //Create a nickname widget for every instance of the Character.
    if (GetNetMode() != ENetMode::NM_DedicatedServer)
    {
        CreatePlayerNicknameWidget();
    }
}

void ATheDoorMainCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (GetNetMode() != ENetMode::NM_DedicatedServer)
    {
        if (IsValid(WC_Nickname) && IsPlayerAController() && !bNicknameVisible)
        {
            WC_Nickname->SetVisibility(bNicknameVisible);
        }
    }

    if (HasAuthority() && IsValid(BC_InteractionArea))
    {
        BC_InteractionArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
        BC_InteractionArea->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);
    }
}

void ATheDoorMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(IA_Interact.Get(), ETriggerEvent::Started, this, &ThisClass::Interact);
    }
}

void ATheDoorMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, Nickname);
}

void ATheDoorMainCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    //Update a nickname widget only for,
    //Listen Server as a player.
    if (IsPlayerAController())
    {
        if (PS_TheDoor = Cast<APS_TheDoor>(GetPlayerState()))
        {
            Nickname = PS_TheDoor->GetPlayerNickname();
            UpdatePlayerNicknameWidget(Nickname);
        }
    }
}

void ATheDoorMainCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    //When client's PS is ready,
    //get the client copy only and update,
    //the name for the server.
    if (IsValid(GetOwner()))
    {
        if (PS_TheDoor = Cast<APS_TheDoor>(GetPlayerState()))
        {
            ServerSetNickname(PS_TheDoor->GetPlayerNickname());
        }
    }
}

void ATheDoorMainCharacter::ServerSetNickname_Implementation(const FText& NewNickname)
{
    //Set a replicated variable on a server,
    //To update clients names on a Listen server.
    Nickname = NewNickname;
    UpdatePlayerNicknameWidget(Nickname);
}

void ATheDoorMainCharacter::CreatePlayerNicknameWidget()
{
    if (IsValid(W_NicknameClass))
    {
        WBP_Nickname = CreateWidget<UW_PlayerNickname>(GetWorld(), W_NicknameClass);
        if (IsValid(WC_Nickname))
        {
            WC_Nickname->SetWidget(WBP_Nickname);
        }
    }
}

void ATheDoorMainCharacter::UpdatePlayerNicknameWidget(const FText& NewName)
{
    if (IsValid(WBP_Nickname))
    {
        WBP_Nickname->SetPlayerNickname(NewName);
    }
}

void ATheDoorMainCharacter::OnRep_Nickname()
{
    //Update the nickname for all clients.
    UpdatePlayerNicknameWidget(Nickname);
}

void ATheDoorMainCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (IsValid(OtherActor) && OtherActor != this && OtherActor->ActorHasTag(DoorData.GetInteractiveTag()))
    {
        Interactive = OtherActor;
    }
}

void ATheDoorMainCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (Interactive)
    {
        Interactive = nullptr;
    }
}

bool ATheDoorMainCharacter::IsPlayerAController()
{
    if (IsValid(GetController()))
    {
        return GetController()->IsLocalPlayerController();
    }

    return false;
}

void ATheDoorMainCharacter::Interact()
{
    if (HasAuthority())
    {
        if (IsValid(Interactive))
        {
            TObjectPtr<IInteractable> InteractObject{ Cast<IInteractable>(Interactive) };
            if (InteractObject)
            {
                InteractObject->Interact();
            }
        }
    }
    else
    {
        ServerMakeInteraction();
    }

}

void ATheDoorMainCharacter::ServerMakeInteraction_Implementation()
{
    Interact();
}
