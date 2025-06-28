#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "MyGameInstance.h"

/************************ Widget Initialization ************************/
bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostClicked);
		UE_LOG(LogTemp, Log, TEXT("UMainMenu: HostButton found and its OnClicked delegate bound."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu: HostButton (named 'HostButton' with 'Is Variable' checked in WBP_MainMenu?) NOT FOUND."));
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinClicked);
		UE_LOG(LogTemp, Log, TEXT("UMainMenu: JoinButton found and its OnClicked delegate bound."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu: JoinButton (named 'JoinButton' with 'Is Variable' checked in WBP_MainMenu?) NOT FOUND."));
	}

	if (SinglePlayerButton)
	{
		SinglePlayerButton->OnClicked.AddDynamic(this, &UMainMenu::OnSinglePlayerClicked);
		UE_LOG(LogTemp, Log, TEXT("UMainMenu: SinglePlayerButton found and its OnClicked delegate bound."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu: SinglePlayerButton (named 'SinglePlayerButton' with 'Is Variable' checked in WBP_MainMenu?) NOT FOUND."));
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitClicked);
		UE_LOG(LogTemp, Log, TEXT("UMainMenu: QuitButton found and its OnClicked delegate bound."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu: QuitButton (named 'QuitButton' with 'Is Variable' checked in WBP_MainMenu?) NOT FOUND."));
	}

	if (IPAddressTextBox)
	{
		UE_LOG(LogTemp, Log, TEXT("UMainMenu: IPAddressTextBox found."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu: IPAddressTextBox (named 'IPAddressTextBox' with 'Is Variable' checked in WBP_MainMenu?) NOT FOUND."));
	}

	UE_LOG(LogTemp, Log, TEXT("UMainMenu: Initialize completed."));

	return Success;
}

/************************ GameInstance Reference Setter ************************/
void UMainMenu::SetGameInstanceReference(UMyGameInstance* InGameInstance)
{
	GameInstanceRef = InGameInstance;
	if (GameInstanceRef.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("UMainMenu: GameInstance reference set successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu: Failed to set GameInstance reference. It might be NULL or invalid."));
	}
}

/************************ Button Click Handlers ************************/
void UMainMenu::OnHostClicked()
{
	UE_LOG(LogTemp, Log, TEXT("UMainMenu: Host button clicked."));
	if (GameInstanceRef.IsValid())
	{
		GameInstanceRef->HostGame();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu: GameInstanceRef is invalid when Host button clicked. Cannot call HostGame."));
	}
}

void UMainMenu::OnJoinClicked()
{
	UE_LOG(LogTemp, Log, TEXT("UMainMenu: Join button clicked."));
	if (GameInstanceRef.IsValid() && IPAddressTextBox)
	{
		FString IPAddress = IPAddressTextBox->GetText().ToString();
		if (!IPAddress.IsEmpty())
		{
			GameInstanceRef->JoinGameByIP(IPAddress);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UMainMenu: IPAddressTextBox is empty. Cannot join game."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu: GameInstanceRef or IPAddressTextBox is invalid when Join button clicked. Cannot call JoinGameByIP."));
	}
}

void UMainMenu::OnSinglePlayerClicked()
{
	UE_LOG(LogTemp, Log, TEXT("UMainMenu: Single Player button clicked."));
	if (GameInstanceRef.IsValid())
	{
		GameInstanceRef->PlaySinglePlayer();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu: GameInstanceRef is invalid when Single Player button clicked. Cannot call PlaySinglePlayer."));
	}
}

void UMainMenu::OnQuitClicked()
{
	UE_LOG(LogTemp, Log, TEXT("UMainMenu: Quit button clicked."));
	if (GameInstanceRef.IsValid())
	{
		GameInstanceRef->QuitGameApplication();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu: GameInstanceRef is invalid when Quit button clicked. Cannot call QuitGameApplication."));
	}
}