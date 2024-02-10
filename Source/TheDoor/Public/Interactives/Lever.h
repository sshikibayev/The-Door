// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Structs/TheDoorData.h"

#include "Lever.generated.h"

class UBoxComponent;
struct FTheDoorData;

UCLASS()
class THEDOOR_API ALever : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ALever();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> LeverMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
    TObjectPtr<UBoxComponent> BC_InteractionArea;

    UPROPERTY(EditAnywhere, Category = Target)
    TObjectPtr<AActor> Target;

    UPROPERTY()
    FTheDoorData DoorData;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
    virtual void Interact() override;
};
