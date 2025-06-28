#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/************************ Forward Declarations ************************/
class UMyGameInstance;

/************************ Class Definition ************************/
UCLASS()
class DUCKDUCKKAZOO_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	/************************ GameInstance Reference Setter ************************/
	UFUNCTION(BlueprintCallable, Category = "UI|GameInstance")
	void SetGameInstanceReference(UMyGameInstance* InGameInstance);

protected:
	/************************ Widget Initialization ************************/
	virtual bool Initialize() override;

private:
	/************************ UI References ************************/
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SinglePlayerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IPAddressTextBox;

	/************************ Event Handlers ************************/
	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnJoinClicked();

	UFUNCTION()
	void OnSinglePlayerClicked();

	UFUNCTION()
	void OnQuitClicked();

private:
	/************************ Internal GameInstance Reference ************************/
	UPROPERTY()
	TWeakObjectPtr<UMyGameInstance> GameInstanceRef;
};