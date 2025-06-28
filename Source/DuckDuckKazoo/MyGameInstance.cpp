#include "MyGameInstance.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "MainMenu.h"

/************************ Constructor & Init ************************/
UMyGameInstance::UMyGameInstance() : MainMenuWidgetInstance(nullptr)
{
	UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::UMyGameInstance() constructor executed."));

	GameplayMapName = TEXT("/Game/myFolder/Maps_Levels/GameMapLevel");
	LobbyMapName = TEXT("/Game/myFolder/Maps_Levels/LobbyMap");

	static ConstructorHelpers::FClassFinder<UMainMenu> MenuBPClassFinder(TEXT("/Game/myFolder/Level_Assets/UI/WBP_MainMenu"));

	if (MenuBPClassFinder.Class != nullptr)
	{
		MainMenuClass = MenuBPClassFinder.Class;
		UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: MainMenuClass (WBP_MainMenu) loading successful."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMyGameInstance: MainMenuClass (WBP_MainMenu) is NULL and loading failed. Check path /Game/myFolder/Level_Assets/UI/WBP_MainMenu."));
	}
}

void UMyGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::Init() function executed."));
}

/************************ Core Game Functions ************************/
void UMyGameInstance::PlaySinglePlayer()
{
	UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: Starting Single Player Game. Loading map: %s"), *GameplayMapName.ToString());
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Starting Single Player: %s"), *GameplayMapName.ToString()));

	if (MainMenuWidgetInstance && MainMenuWidgetInstance->IsInViewport())
	{
		MainMenuWidgetInstance->RemoveFromParent();
		if (APlayerController* PlayerController = GetFirstLocalPlayerController())
		{
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = false;
		}
	}
	UGameplayStatics::OpenLevel(GetWorld(), GameplayMapName, true);
}

void UMyGameInstance::QuitGameApplication()
{
	UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: Quitting Game Application."));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Quitting Game!"));

	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		PlayerController->ConsoleCommand("quit");
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMyGameInstance: PlayerController is NULL, cannot execute quit command."));
		FGenericPlatformMisc::RequestExit(false);
	}
}

/************************ Menu Management ************************/
void UMyGameInstance::ShowMainMenu()
{
	UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: Entering ShowMainMenu."));

	if (!MainMenuClass)
	{
		UE_LOG(LogTemp, Error, TEXT("UMyGameInstance: MainMenuClass is NULL, cannot show main menu."));
		return;
	}

	if (!MainMenuWidgetInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: MainMenuWidgetInstance is NULL, attempting to create."));
		UMainMenu* CreatedMenu = CreateWidget<UMainMenu>(this, MainMenuClass);

		if (CreatedMenu)
		{
			MainMenuWidgetInstance = CreatedMenu;
			CreatedMenu->SetGameInstanceReference(this);
			UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance: MainMenuWidgetInstance (UMainMenu) created and GameInstanceRef set."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMyGameInstance: Failed to create UMainMenu widget. CreateWidget returned NULL."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: MainMenuWidgetInstance already exists, skipping creation."));
	}

	if (MainMenuWidgetInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: MainMenuWidgetInstance is valid for display operations."));

		if (!MainMenuWidgetInstance->IsInViewport())
		{
			MainMenuWidgetInstance->AddToViewport();
			UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: MainMenuWidgetInstance added to viewport successfully."));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: MainMenuWidgetInstance already in viewport."));
		}

		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			PlayerController->SetInputMode(InputModeData);
			PlayerController->bShowMouseCursor = true;

			UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: PlayerController input mode set to UI Only and mouse cursor shown."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMyGameInstance: PlayerController is NULL, cannot set input mode or show cursor."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMyGameInstance: MainMenuWidgetInstance is NULL after creation attempt (this should not happen!)."));
	}
	UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: Exiting ShowMainMenu."));
}

/************************ Multiplayer Functions ************************/
void UMyGameInstance::HostGame()
{
	UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: HostGame: Attempting to host listen server on map: %s"), *GameplayMapName.ToString());
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hosting Listen Server on: %s"), *GameplayMapName.ToString()));

	if (MainMenuWidgetInstance && MainMenuWidgetInstance->IsInViewport())
	{
		MainMenuWidgetInstance->RemoveFromParent();
		if (APlayerController* PlayerController = GetFirstLocalPlayerController())
		{
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = false;
		}
	}

	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel(GameplayMapName.ToString() + "?listen", true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMyGameInstance: HostGame: World is NULL, cannot perform ServerTravel."));
	}
}

void UMyGameInstance::JoinGameByIP(const FString& IP_Address)
{
	UE_LOG(LogTemp, Log, TEXT("UMyGameInstance: JoinGameByIP: Attempting to join: %s"), *IP_Address);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Attempting to join: %s"), *IP_Address));

	if (MainMenuWidgetInstance && MainMenuWidgetInstance->IsInViewport())
	{
		MainMenuWidgetInstance->RemoveFromParent();
		if (APlayerController* Controller = GetFirstLocalPlayerController())
		{
			FInputModeGameOnly InputMode;
			Controller->SetInputMode(InputMode);
			Controller->bShowMouseCursor = false;
		}
	}

	APlayerController* Controller = GetFirstLocalPlayerController();
	if (Controller)
	{
		Controller->ClientTravel(IP_Address, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMyGameInstance: JoinGameByIP: PlayerController is NULL, cannot client travel."));
	}
}