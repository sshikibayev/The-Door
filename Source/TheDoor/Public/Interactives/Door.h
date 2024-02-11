// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"

#include "Door.generated.h"

UCLASS()
class THEDOOR_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ADoor();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
    float InterpSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
    float MoveDistance;

    UPROPERTY(ReplicatedUsing = OnRep_DoorState)
    bool bIsOpen;

    FVector InitialLocation;
    FVector TargetLocation;

	virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void Interact() override;
    virtual void AuthToggleDoor();

    UFUNCTION()
    void OnRep_DoorState();
};
