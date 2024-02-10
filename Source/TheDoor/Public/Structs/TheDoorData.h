#pragma once

#include "CoreMinimal.h"

#include "TheDoorData.generated.h"

USTRUCT(BlueprintType)
struct FTheDoorData
{
	GENERATED_BODY()

public:
    FORCEINLINE FName GetInteractiveTag()
    {
        return InteractiveTag;
    }

private:
    UPROPERTY(VisibleAnywhere, Category = Tag)
    FName InteractiveTag{ "Interactive" };
};
