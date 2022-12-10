#include "CStartMenu.h"
#include "Global.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "GameFramework/PlayerController.h"
#include "CMainPlayerController.h"
#include "Player/CMainPayer.h"
#include "Misc/Paths.h"
#include "Save/CSaveGame.h"
void UCStartMenu::NativeConstruct()
{
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitButton")));
	
	StartButton->OnClicked.AddDynamic(this, &UCStartMenu::StartButtonCallback);
	QuitButton->OnClicked.AddDynamic(this, &UCStartMenu::QuitButtonCallback);
	MainController = Cast<ACMainPlayerController>(GetWorld()->GetFirstPlayerController());
	MainController->ShowCursor();

	Player = Cast<ACMainPayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	Super::NativeConstruct();

}

void UCStartMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCStartMenu::StartButtonCallback()
{
	//레벨이 바뀌는 함수 나중에 수정해야함
	SaveGameInstance = NewObject<UCSaveGame>();
	SaveGameInstance->bStartMenu = true;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "Save", 0);

	MainController->OffShowCursor();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level1"));
	
}

void UCStartMenu::QuitButtonCallback()
{
	// 게임 종료 함수
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
