#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/************************ Forward Declarations ************************/
class UMainMenu;

/************************ Class Definition ************************/
UCLASS()
class DUCKDUCKKAZOO_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/************************ Constructor & Init ************************/
	UMyGameInstance();
	virtual void Init() override;

	/************************ Core Game Functions ************************/
	UFUNCTION(BlueprintCallable, Category = "Game|Gameplay")
	void PlaySinglePlayer();

	UFUNCTION(BlueprintCallable, Category = "Game|Menu")
	void QuitGameApplication();

	/************************ Menu Management ************************/
	UFUNCTION(BlueprintCallable, Category = "Game|Menu")
	void ShowMainMenu();

	/************************ Multiplayer Functions ************************/
	UFUNCTION(BlueprintCallable, Category = "Game|Multiplayer")
	void HostGame();

	UFUNCTION(BlueprintCallable, Category = "Game|Multiplayer")
	void JoinGameByIP(const FString& IP_Address);

public:
	/************************ Map Properties ************************/
	UPROPERTY(EditDefaultsOnly, Category = "Game|Maps")
	FName GameplayMapName;

	UPROPERTY(EditDefaultsOnly, Category = "Game|Maps")
	FName LobbyMapName;

private:
	/************************ Internal Menu Properties ************************/
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY()
	UMainMenu* MainMenuWidgetInstance;
};