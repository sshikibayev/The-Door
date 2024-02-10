// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactives/Lever.h"

#include "Components/BoxComponent.h"


ALever::ALever()
{
	PrimaryActorTick.bCanEverTick = false;
    BC_InteractionArea = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionArea"));
    BC_InteractionArea->SetCollisionProfileName(TEXT("OverlapAll"));
    SetRootComponent(BC_InteractionArea);

    LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
    LeverMesh->SetupAttachment(RootComponent);

    Tags.Add(DoorData.GetInteractiveTag());
}

void ALever::BeginPlay()
{
	Super::BeginPlay();

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
