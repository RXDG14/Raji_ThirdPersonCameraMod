#include "Globals.h"

bool bModMenuCreated = false;
bool bModMenuEnabled = false;
bool bPlayerSuperJumpEnabled = false;
bool bPlayerSuperSpeedEnabled = false;
bool bThirdPersonCameraEnabled = false;

bool bWasSuperJumpButtonPressed = false;
bool bWasSuperSpeedButtonPressed = false;
bool bWasThirdPersonCameraButtonPressed = false;

SDK::UWorld* WorldCached = nullptr;
SDK::UCameraComponent* MainCamCached = nullptr;
SDK::ABP_PlayerCharacter_C* MyPawnCached = nullptr;
SDK::ABP_PlayerController_C* MyPlayerControllerCached = nullptr;

SDK::UWidgetBP_DebugNarrationButton_C* PlayerSuperJumpButton = nullptr;
SDK::UWidgetBP_DebugNarrationButton_C* PlayerSuperSpeedButton = nullptr;
SDK::UWidgetBP_DebugNarrationButton_C* ThirdPersonCameraButton = nullptr;
