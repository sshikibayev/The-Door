// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheDoor/TheDoorCharacter.h"
#include "Structs/TheDoorData.h"

#include "TheDoorMainCharacter.generated.h"

class UInputAction;
class UWidgetComponent;
class UW_PlayerNickname;
class APS_TheDoor;
class UBoxComponent;


UCLASS()
class THEDOOR_API ATheDoorMainCharacter : public ATheDoorCharacter
{
    GENERATED_BODY()

public:
    ATheDoorMainCharacter();

    FORCEINLINE FText GetNickname()
    {
        return Nickname;
    }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TSoftObjectPtr<UInputAction> IA_Interact;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
    TObjectPtr<UWidgetComponent> WC_Nickname;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
    TSubclassOf<UW_PlayerNickname> W_NicknameClass;

    UPROPERTY(EditAnywhere, Category = Input)
    bool bNicknameVisible{ true };

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
    TObjectPtr<UBoxComponent> BC_InteractionArea;

    UPROPERTY()
    TObjectPtr<APS_TheDoor> PS_TheDoor;

    FName InteractiveTag;

    virtual void BeginPlay() override;
    virtual void PostInitializeComponents() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    UFUNCTION()
    virtual void Interact();

    UFUNCTION()
    virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    virtual void CreatePlayerNicknameWidget();
    virtual void UpdatePlayerNicknameWidget(const FText& NewName);

private:
    UPROPERTY()
    TObjectPtr<UW_PlayerNickname> WBP_Nickname;

    UPROPERTY(ReplicatedUsing=OnRep_Nickname)
    FText Nickname{ FText::FromString("Unknown") };

    UPROPERTY()
    TObjectPtr<AActor> Interactive;
    UPROPERTY()
    FTheDoorData DoorData;

    UFUNCTION()
    void MakeInteraction();

    UFUNCTION(Server, Reliable)
    void ServerSetNickname(const FText& NewNickname);
    UFUNCTION(Server, Unreliable)
    void ServerMakeInteraction();

    UFUNCTION()
    void OnRep_Nickname();
};
