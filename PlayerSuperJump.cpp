#include <Windows.h>
#include "../SDK.hpp"
#include "Globals.h"

void PlayerSuperJump()
{
    if (!bPlayerSuperJumpEnabled)
        return;

    if (MyPawnCached->CharacterMovement)
    {
        MyPawnCached->CharacterMovement->AirControl = 1000.0f;
        MyPawnCached->CharacterMovement->AirControlBoostMultiplier = 10;
    }

    // Left Alt
    if (GetAsyncKeyState(VK_MENU) & 0x1)
    {
        MyPawnCached->LaunchCharacter(SDK::FVector(0, 0, 2000), true, true);
    }
}