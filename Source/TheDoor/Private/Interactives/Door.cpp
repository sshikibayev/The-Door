// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactives/Door.h"

#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    SetRootComponent(DoorMesh);

    bIsOpen = false;
    InterpSpeed = 10.0f;
    MoveDistance = 700.0f;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

    SetReplicateMovement(true);

    InitialLocation = GetActorLocation();
    TargetLocation = InitialLocation;
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector CurrentLocation = GetActorLocation();
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
    SetActorLocation(NewLocation);
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ThisClass, bIsOpen, COND_SkipOwner);
}

void ADoor::Interact()
{
    AuthToggleDoor();
}

void ADoor::AuthToggleDoor()
{
    if (HasAuthority())
    {
        bIsOpen = !bIsOpen;
        OnRep_DoorState();
    }
}

void ADoor::OnRep_DoorState()
{
    //If true, the door starts opening,
    //otherwise closing.
    bIsOpen == true ? TargetLocation = InitialLocation - FVector(MoveDistance, 0.0f, 0.0f) : TargetLocation = InitialLocation;
}
