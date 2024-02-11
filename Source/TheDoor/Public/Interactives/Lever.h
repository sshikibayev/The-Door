// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Structs/TheDoorData.h"

#include "Lever.generated.h"

class UBoxComponent;
class UWidgetComponent;
class UW_ButtonNotification;

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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
    TObjectPtr<UWidgetComponent> WC_ButtonNotification;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
    TSubclassOf<UW_ButtonNotification> W_ButtonNotificationClass;

    UPROPERTY(EditAnywhere, Category = Target)
    TObjectPtr<AActor> Target;

    UPROPERTY()
    FTheDoorData DoorData;

    FName PlayerTag;
    bool bInteracted;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void Interact() override;

    UFUNCTION()
    virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    virtual void CreateButtonNotificationWidget();

private:
    UPROPERTY()
    TObjectPtr<UW_ButtonNotification> WBP_ButtonNotification;

    void ShowButtonWidget();
    void HideButtonWidget();
};
