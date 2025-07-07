#include <Windows.h>
#include "../SDK.hpp"
#include "Globals.h"

void PlayerSuperSpeed()
{
	if (!bPlayerSuperSpeedEnabled)
		return;

    // ctrl
    if (GetAsyncKeyState(VK_CONTROL) & 0x1)
    {
        SDK::FVector forwardVector = MyPawnCached->GetActorForwardVector();
        SDK::FVector dashVelocity = forwardVector * 3000.0f;
        MyPawnCached->CharacterMovement->Velocity = dashVelocity;
    }

}