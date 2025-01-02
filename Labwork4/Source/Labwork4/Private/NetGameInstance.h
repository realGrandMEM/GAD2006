// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void host(FString MapName);

	UFUNCTION(BlueprintCallable)
	void join(FString Address);
};
