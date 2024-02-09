// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheDoor/TheDoorCharacter.h"

#include "TheDoorMainCharacter.generated.h"

class UInputAction;
class UWidgetComponent;
class UW_PlayerNickname;
class APS_TheDoor;

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

    UPROPERTY()
    TObjectPtr<APS_TheDoor> PS_TheDoor;

    virtual void BeginPlay() override;
    virtual void PostInitializeComponents() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    UFUNCTION()
    virtual void Interact();

    virtual void CreatePlayerNicknameWidget();
    virtual void UpdatePlayerNicknameWidget(const FText& NewName);

private:
    UPROPERTY()
    TObjectPtr<UW_PlayerNickname> WBP_Nickname;

    UPROPERTY(ReplicatedUsing=OnRep_Nickname)
    FText Nickname{ FText::FromString("Unknown") };

    UFUNCTION(Server, Reliable)
    void ServerSetNickname(const FText& NewNickname);

    UFUNCTION()
    void OnRep_Nickname();

    bool IsPlayerAController();
};
