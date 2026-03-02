// WuxiaCameraComponent.cpp
#include "World/WuxiaCameraComponent.h"

UWuxiaCameraComponent::UWuxiaCameraComponent()
{
	ApplyIsometricSettings();
}

void UWuxiaCameraComponent::ApplyIsometricSettings()
{
	SetRelativeRotation(FRotator(IsometricPitch, IsometricYaw, 0.f));

	if (bUseOrthographic)
	{
		SetProjectionMode(ECameraProjectionMode::Orthographic);
		SetOrthoWidth(DefaultOrthoWidth);
	}
	else
	{
		SetProjectionMode(ECameraProjectionMode::Perspective);
		SetFieldOfView(DefaultPerspectiveFOV);
	}
}
