// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "W_PlayerNickname.generated.h"


UCLASS()
class THEDOOR_API UW_PlayerNickname : public UUserWidget
{
	GENERATED_BODY()

public:
    FORCEINLINE void SetPlayerNickname(const FText& NewNickname)
    {
        TB_Nickname->SetText(NewNickname);
    }

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TB_Nickname;
};
