#include <Windows.h>
#include <iostream>
#include "MinHook.h"
#include "SDK.hpp"
#include "SDK/CoreUObject_classes.hpp"
#include "SDK/Engine_classes.hpp"
#include "Mods/Globals.h"
#include "Mods/ModMenu.h"
#include "Mods/PlayerSuperJump.h"
#include "Mods/PlayerSuperSpeed.h"
#include "Mods/ThirdPersonCamera.h"

#pragma comment(lib, "libMinHook.x64.lib")

using tProcessEvent = void(__fastcall*)(SDK::UObject*, SDK::UFunction*, void*);
static tProcessEvent oProcessEvent = nullptr;

void __fastcall hkProcessEvent(SDK::UObject* obj, SDK::UFunction* func, void* params)
{
    if (func && func->GetName() == "ReceiveTick")
    {
        // & 1 = button pressed once
        if (GetAsyncKeyState(VK_F1) & 1) bPlayerSuperJumpEnabled = !bPlayerSuperJumpEnabled;
        if (GetAsyncKeyState(VK_F2) & 1) bPlayerSuperSpeedEnabled = !bPlayerSuperSpeedEnabled;
        if (GetAsyncKeyState(VK_F3) & 1) bThirdPersonCameraEnabled = !bThirdPersonCameraEnabled;
        if (GetAsyncKeyState(VK_F4) & 1) bModMenuEnabled = !bModMenuEnabled;

        ModMenuMain();
        ModMenuButtonsPressed();
        
        PlayerSuperJump();
        ThirdPersonCamera();
        PlayerSuperSpeed();
    }

    oProcessEvent(obj, func, params);
}

DWORD WINAPI MainThread(LPVOID)
{
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

    MH_Initialize();
    uintptr_t base = (uintptr_t)GetModuleHandle(nullptr);
    void* peAddr = (void*)(base + 15045152);

    MH_CreateHook(peAddr, hkProcessEvent, (LPVOID*)&oProcessEvent);
    MH_EnableHook(peAddr);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        CreateThread(nullptr, 0, MainThread, nullptr, 0, nullptr);
    }
    if(reason == DLL_PROCESS_DETACH)
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
    }

    return TRUE;
}