// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CodeInputWidget.generated.h"

/**
 * 
 */
UCLASS()
class JAVACOMPILERTEST_API UCodeInputWidget : public UUserWidget
{
	GENERATED_BODY()
    public:
	UFUNCTION(BlueprintCallable, Category = "Code-Related" )
	void compileCode(FText codeToCompile);
};
