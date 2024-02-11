// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactives/Lever.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/W_ButtonNotification.h"
#include "Character/TheDoorMainCharacter.h"


ALever::ALever()
{
	PrimaryActorTick.bCanEverTick = false;

    BC_InteractionArea = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionArea"));
    BC_InteractionArea->SetCollisionProfileName(TEXT("OverlapAll"));
    SetRootComponent(BC_InteractionArea);

    LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
    LeverMesh->SetupAttachment(RootComponent);

    WC_ButtonNotification = CreateDefaultSubobject<UWidgetComponent>(TEXT("ButtonNotification"));
    WC_ButtonNotification->SetupAttachment(GetRootComponent());

    Tags.Add(DoorData.GetInteractiveTag());

    PlayerTag = DoorData.GetPlayerTag();
    bInteracted = false;
}

void ALever::BeginPlay()
{
	Super::BeginPlay();

    if (!IsRunningDedicatedServer())
    {
        BC_InteractionArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
        BC_InteractionArea->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);

        CreateButtonNotificationWidget();
        HideButtonWidget();
    }
}

void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALever::Interact()
{
    if (IsValid(Target))
    {
        TObjectPtr<IInteractable> TargetInteractable = Cast<IInteractable>(Target);
        if (TargetInteractable)
        {
            TargetInteractable->Interact();
        }
    }
}

void ALever::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (IsValid(OtherActor) && OtherActor->ActorHasTag(PlayerTag))
    {
        if (CastChecked<ACharacter>(OtherActor)->IsLocallyControlled() && !bInteracted)
        {
            bInteracted = true;
            ShowButtonWidget();
        }
    }
}

void ALever::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (IsValid(OtherActor) && OtherActor->ActorHasTag(PlayerTag))
    {
        if (CastChecked<ACharacter>(OtherActor)->IsLocallyControlled() && bInteracted)
        {
            bInteracted = false;
            HideButtonWidget();
        }
    }
}

void ALever::CreateButtonNotificationWidget()
{
    if (IsValid(W_ButtonNotificationClass))
    {
        WBP_ButtonNotification = CreateWidget<UW_ButtonNotification>(GetWorld(), W_ButtonNotificationClass);
        if (IsValid(WBP_ButtonNotification))
        {
            WC_ButtonNotification->SetWidget(WBP_ButtonNotification);
        }
    }
}

void ALever::ShowButtonWidget()
{
    if (IsValid(WBP_ButtonNotification))
    {
        WBP_ButtonNotification->ShowWidget();
    }
}

void ALever::HideButtonWidget()
{
    if (IsValid(WBP_ButtonNotification))
    {
        WBP_ButtonNotification->HideWidget();
    }
}
