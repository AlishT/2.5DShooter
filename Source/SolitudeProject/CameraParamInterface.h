// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraParamInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCameraParamInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOLITUDEPROJECT_API ICameraParamInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void SetCameraNewParam(float ArmLenght, float LocationZ, float Offset);
};
