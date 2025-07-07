#pragma once

#include "../SDK.hpp"

extern bool bModMenuCreated;
extern bool bModMenuEnabled;
extern bool bPlayerSuperJumpEnabled;
extern bool bPlayerSuperSpeedEnabled;
extern bool bThirdPersonCameraEnabled;

extern bool bWasSuperJumpButtonPressed;
extern bool bWasSuperSpeedButtonPressed;
extern bool bWasThirdPersonCameraButtonPressed;

extern SDK::UWorld* WorldCached;
extern SDK::UCameraComponent* MainCamCached;
extern SDK::ABP_PlayerCharacter_C* MyPawnCached;
extern SDK::ABP_PlayerController_C* MyPlayerControllerCached;

extern SDK::UWidgetBP_DebugNarrationButton_C* PlayerSuperJumpButton;
extern SDK::UWidgetBP_DebugNarrationButton_C* PlayerSuperSpeedButton;
extern SDK::UWidgetBP_DebugNarrationButton_C* ThirdPersonCameraButton;