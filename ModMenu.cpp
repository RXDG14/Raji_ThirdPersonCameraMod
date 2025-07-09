#include "../SDK.hpp"
#include "Globals.h"

SDK::UWidgetBP_DebugNarrationButton_C* SpawnMenuButton(const wchar_t* Label, const SDK::FVector2D& Position, SDK::UCanvasPanel* CanvasPanel)
{
    auto ButtonClass = SDK::UWidgetBP_DebugNarrationButton_C::StaticClass();
    auto Button = (SDK::UWidgetBP_DebugNarrationButton_C*)SDK::UWidgetBlueprintLibrary::Create(WorldCached, ButtonClass, MyPlayerControllerCached);
    if (!Button) return nullptr;

    auto Slot = (SDK::UCanvasPanelSlot*)CanvasPanel->AddChildToCanvas(Button);
    if (!Slot) return nullptr;

    Slot->SetPosition(Position);
    Slot->SetSize(SDK::FVector2D(480.f, 100.f));

    // Set Text
    SDK::FString ButtonLabel(Label);
    SDK::FText LabelText = SDK::UKismetTextLibrary::Conv_StringToText(ButtonLabel);

    auto FontInfo = Button->TextBlock_0->Font;
    FontInfo.Size = 30;
    Button->TextBlock_0->SetFont(FontInfo);
    Button->TextBlock_0->SetText(LabelText);

    return Button;
}

void UpdateModMenuButtonColors()
{
    if (PlayerSuperJumpButton && PlayerSuperSpeedButton && ThirdPersonCameraButton)
    {
        SDK::FLinearColor ActiveColor(0, 1, 0, 1);
        SDK::FLinearColor InActiveColor(1, 0, 0, 1);
        PlayerSuperJumpButton->NarrationButton->SetBackgroundColor(bPlayerSuperJumpEnabled ? ActiveColor : InActiveColor);
        PlayerSuperSpeedButton->NarrationButton->SetBackgroundColor(bPlayerSuperSpeedEnabled ? ActiveColor : InActiveColor);
        ThirdPersonCameraButton->NarrationButton->SetBackgroundColor(bThirdPersonCameraEnabled ? ActiveColor : InActiveColor);
    }

}

void ModMenuButtonsPressed()
{
    UpdateModMenuButtonColors();

    if (PlayerSuperJumpButton)
    {
        bool bIsPressed = PlayerSuperJumpButton->NarrationButton->IsPressed();

        if (bIsPressed && !bWasSuperJumpButtonPressed)
        {
            bPlayerSuperJumpEnabled = !bPlayerSuperJumpEnabled;
        }

        bWasSuperJumpButtonPressed = bIsPressed;
    }

    if (PlayerSuperSpeedButton)
    {
        bool bIsPressed = PlayerSuperSpeedButton->NarrationButton->IsPressed();

        if (bIsPressed && !bWasSuperSpeedButtonPressed)
        {
            bPlayerSuperSpeedEnabled = !bPlayerSuperSpeedEnabled;
        }

        bWasSuperSpeedButtonPressed = bIsPressed;
    }

    if (ThirdPersonCameraButton)
    {
        bool bIsPressed = ThirdPersonCameraButton->NarrationButton->IsPressed();

        if (bIsPressed && !bWasThirdPersonCameraButtonPressed)
        {
            bThirdPersonCameraEnabled = !bThirdPersonCameraEnabled;
        }

        bWasThirdPersonCameraButtonPressed = bIsPressed;
    }
}

void CreateModMenu()
{
    if (!MyPlayerControllerCached->DebugMenuWidget) 
    {
        auto DebugMenuClass = SDK::UWidgetBP_DebugMenu_C::StaticClass();
        auto DebugMenu = (SDK::UWidgetBP_DebugMenu_C*)SDK::UWidgetBlueprintLibrary::Create(WorldCached, DebugMenuClass, MyPlayerControllerCached);
        if (!DebugMenu)
        {
            std::cout << "Failed to create DebugMenuWidget.\n";
            return;
        }

        MyPlayerControllerCached->DebugMenuWidget = DebugMenu;
        DebugMenu->AddToViewport(999);
        DebugMenu->ShowFPS();
        DebugMenu->DebugButton->SetIsEnabled(true);
        DebugMenu->DebugButton->SetVisibility(SDK::ESlateVisibility::Visible);
        DebugMenu->ChangeLevelButton->SetIsEnabled(true);
        DebugMenu->ChangeLevelButton->SetVisibility(SDK::ESlateVisibility::Visible);
        DebugMenu->ResetLevelButton->SetIsEnabled(true);
        DebugMenu->ResetLevelButton->SetVisibility(SDK::ESlateVisibility::Visible);
        DebugMenu->NarrationDebugButton->SetIsEnabled(true);
        DebugMenu->NarrationDebugButton->SetVisibility(SDK::ESlateVisibility::Visible);
        DebugMenu->CombatDialoguesDebugButton->SetIsEnabled(true);
        DebugMenu->CombatDialoguesDebugButton->SetVisibility(SDK::ESlateVisibility::Visible);
        DebugMenu->CinematicsDebugButton->SetIsEnabled(true);
        DebugMenu->CinematicsDebugButton->SetVisibility(SDK::ESlateVisibility::Visible);

        auto DebugMenuWidget = MyPlayerControllerCached->DebugMenuWidget;
        auto CurrentParentWidget = DebugMenuWidget->DebugButton->GetParent();
        while (CurrentParentWidget)
        {
            if (CurrentParentWidget->IsA(SDK::UCanvasPanel::StaticClass()))
            {
                auto CanvasPanel = (SDK::UCanvasPanel*)CurrentParentWidget;
                
                PlayerSuperJumpButton = SpawnMenuButton(L"Player Super Jump", { 800.f, 200.f }, CanvasPanel);
                PlayerSuperSpeedButton = SpawnMenuButton(L"Player Super Speed", { 800.f, 300.f }, CanvasPanel);
                ThirdPersonCameraButton = SpawnMenuButton(L"Third-Person Camera", { 800.f, 400.f }, CanvasPanel);
                
                bModMenuCreated = true;
                break;
            }
            CurrentParentWidget = CurrentParentWidget->GetParent();
        }
    }
}

void DestroyModMenu()
{
    if (bModMenuCreated && MyPlayerControllerCached->DebugMenuWidget)
    {
        if (PlayerSuperJumpButton)
        {
            PlayerSuperJumpButton->RemoveFromParent();
            PlayerSuperJumpButton = nullptr;
        }
        if (PlayerSuperSpeedButton)
        {
            PlayerSuperSpeedButton->RemoveFromParent();
            PlayerSuperSpeedButton = nullptr;
        }
        if (ThirdPersonCameraButton)
        {
            ThirdPersonCameraButton->RemoveFromParent();
            ThirdPersonCameraButton = nullptr;
        }
        if (MyPlayerControllerCached && MyPlayerControllerCached->DebugMenuWidget)
        {
            MyPlayerControllerCached->DebugMenuWidget->RemoveFromParent();
            MyPlayerControllerCached->DebugMenuWidget = nullptr;
        }
        bModMenuCreated = false;
    }
}

void ShowModMenu()
{
    if(MyPlayerControllerCached->DebugMenuWidget)
    {
        MyPlayerControllerCached->DebugMenuWidget->SetVisibility(SDK::ESlateVisibility::Visible);
    }
    else
    {
        CreateModMenu();
    }
}

void HideModMenu()
{
    if (MyPlayerControllerCached->DebugMenuWidget)
    {
        MyPlayerControllerCached->DebugMenuWidget->SetVisibility(SDK::ESlateVisibility::Hidden);
    }
}

void ModMenuMain()
{
    SDK::UWorld* World = SDK::UWorld::GetWorld();
    if (!World || !World->OwningGameInstance || World->OwningGameInstance->LocalPlayers.Num() == 0)
        return;

    auto playerController = World->OwningGameInstance->LocalPlayers[0]->PlayerController;
    if (!playerController)
        return;

    auto MyPlayerController = static_cast<SDK::ABP_PlayerController_C*>(playerController);
    if (!MyPlayerController)
        return;
    MyPlayerControllerCached = MyPlayerController;

    auto MyPawn = static_cast<SDK::ABP_PlayerCharacter_C*>(playerController->K2_GetPawn());
    if (!MyPawn)
        return;
    MyPawnCached = MyPawn;

    ///////////////////////////////////////////////////////////
    
    // If DebugMenu destroyed due to level change, reset flag
    if (bModMenuCreated && MyPlayerControllerCached->DebugMenuWidget == nullptr)
    {
        bModMenuCreated = false;
    }

    if (bModMenuEnabled)
    {
        if (!bModMenuCreated)
        {
            CreateModMenu();
        }
        ShowModMenu();
    }
    else
    {
        HideModMenu();
    }
}
