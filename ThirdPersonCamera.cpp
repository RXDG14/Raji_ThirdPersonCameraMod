#include <Windows.h>
#include "../SDK.hpp"
#include "Globals.h"
#include "ThirdPersonCamera.h"

static POINT screenCenter = { 0, 0 };
static bool bCameraInitialized = false;

float OriginalSpringArmTargetArmLength = 0;
bool OriginalSpringArmUsePawnControlRotation = false;
bool OriginalSpringArmInheritYaw = false;
bool OriginalSpringArmInheritPitch = false;
bool OriginalSpringArmInheritRoll = false;

static SDK::USceneComponent* OriginalCameraParent = nullptr;
static SDK::USceneComponent* OriginalSpringArmParent = nullptr;

void TPC_Turn(float deltaX)
{
    MyPlayerControllerCached->AddYawInput(deltaX * 0.1f);
}

void TPC_LookUp(float deltaY)
{
    MyPlayerControllerCached->AddPitchInput(deltaY * 0.1f);
}

void SwitchToThirdPersonCameraView()
{
    if (!bCameraInitialized)
    {
        RECT screenRect;
        GetClientRect(GetDesktopWindow(), &screenRect);
        screenCenter.x = (screenRect.right - screenRect.left) / 2;
        screenCenter.y = (screenRect.bottom - screenRect.top) / 2;
        SetCursorPos(screenCenter.x, screenCenter.y);
        ShowCursor(FALSE);
        bCameraInitialized = true;
    }

    MyPawnCached->bUseControllerRotationYaw = false; //was true
    MyPawnCached->bUseControllerRotationPitch = false;
    MyPawnCached->bUseControllerRotationRoll = false;

    if (MyPawnCached->CharacterMovement)
    {
        MyPawnCached->CharacterMovement->bOrientRotationToMovement = true; //was false
        MyPawnCached->CharacterMovement->RotationRate = SDK::FRotator(0, 720, 0);
    }

    // Re-parent SpringArm to Root
    if (MyPawnCached->SpringArm)
    {
        if (!OriginalSpringArmParent)
        {
            OriginalSpringArmParent = MyPawnCached->SpringArm->GetAttachParent();
            OriginalSpringArmTargetArmLength = MyPawnCached->SpringArm->TargetArmLength;
            OriginalSpringArmUsePawnControlRotation = MyPawnCached->SpringArm->bUsePawnControlRotation;
            OriginalSpringArmInheritYaw = MyPawnCached->SpringArm->bInheritYaw;
            OriginalSpringArmInheritPitch = MyPawnCached->SpringArm->bInheritPitch;
            OriginalSpringArmInheritRoll = MyPawnCached->SpringArm->bInheritRoll;
        }

        MyPawnCached->SpringArm->DetachFromParent(false, false);

        MyPawnCached->SpringArm->K2_AttachToComponent(
            MyPawnCached->K2_GetRootComponent(),
            SDK::FName(),
            SDK::EAttachmentRule::SnapToTarget,
            SDK::EAttachmentRule::SnapToTarget,
            SDK::EAttachmentRule::SnapToTarget,
            false
        );

        MyPawnCached->SpringArm->TargetArmLength = 600.0f;
        MyPawnCached->SpringArm->bUsePawnControlRotation = true;
        MyPawnCached->SpringArm->bInheritYaw = false; // was true
        MyPawnCached->SpringArm->bInheritPitch = true;
        MyPawnCached->SpringArm->bInheritRoll = false;
    }

    // Re-parent Camera to SpringArm
    if (MainCamCached)
    {
        if (!OriginalCameraParent)
        {
            OriginalCameraParent = MainCamCached->GetAttachParent();
        }

        MainCamCached->DetachFromParent(false, false);

        MainCamCached->K2_AttachToComponent(
            MyPawnCached->SpringArm,
            SDK::FName(),
            SDK::EAttachmentRule::SnapToTarget,
            SDK::EAttachmentRule::SnapToTarget,
            SDK::EAttachmentRule::SnapToTarget,
            false
        );
    }

    // Mouse
    POINT currPos;
    GetCursorPos(&currPos);
    float deltaX = static_cast<float>(currPos.x - screenCenter.x);
    float deltaY = static_cast<float>(currPos.y - screenCenter.y);

    TPC_Turn(deltaX);
    TPC_LookUp(deltaY);

    SetCursorPos(screenCenter.x, screenCenter.y);
}

void SwitchToOriginalCameraView()
{
    if (!bCameraInitialized)
        return;

    ShowCursor(TRUE);
    bCameraInitialized = false;

    // Restore Camera parent
    if (MainCamCached && OriginalCameraParent)
    {
        MainCamCached->DetachFromParent(false, false);

        MainCamCached->K2_AttachToComponent(
            OriginalCameraParent,
            SDK::FName(),
            SDK::EAttachmentRule::SnapToTarget,
            SDK::EAttachmentRule::SnapToTarget,
            SDK::EAttachmentRule::SnapToTarget,
            false
        );

        OriginalCameraParent = nullptr;
    }

    // Restore SpringArm parent and settings
    if (MyPawnCached && MyPawnCached->SpringArm && OriginalSpringArmParent)
    {
        MyPawnCached->SpringArm->DetachFromParent(false, false);

        MyPawnCached->SpringArm->K2_AttachToComponent(
            OriginalSpringArmParent,
            SDK::FName(),
            SDK::EAttachmentRule::SnapToTarget,
            SDK::EAttachmentRule::SnapToTarget,
            SDK::EAttachmentRule::SnapToTarget,
            false
        );

        MyPawnCached->SpringArm->TargetArmLength = OriginalSpringArmTargetArmLength;
        MyPawnCached->SpringArm->bUsePawnControlRotation = OriginalSpringArmUsePawnControlRotation;
        MyPawnCached->SpringArm->bInheritYaw = OriginalSpringArmInheritYaw;
        MyPawnCached->SpringArm->bInheritPitch = OriginalSpringArmInheritPitch;
        MyPawnCached->SpringArm->bInheritRoll = OriginalSpringArmInheritRoll;

        OriginalSpringArmParent = nullptr;
    }

    if (MyPawnCached)
    {
        MyPawnCached->bUseControllerRotationYaw = false;
        if (MyPawnCached->CharacterMovement)
        {
            MyPawnCached->CharacterMovement->bOrientRotationToMovement = true;
        }
    }

    // Reset cached pointers
    MainCamCached = nullptr;
    //MyPawnCached = nullptr;
    //MyPlayerControllerCached = nullptr;
}

void ThirdPersonCamera()
{
    if (!bThirdPersonCameraEnabled)
    {
        SwitchToOriginalCameraView();
        return;
    }

    if (!MyPlayerControllerCached->MainCamera || !MyPlayerControllerCached->MainCamera->CameraComponent)
        return;

    MainCamCached = MyPlayerControllerCached->MainCamera->CameraComponent;

    SwitchToThirdPersonCameraView();
}
