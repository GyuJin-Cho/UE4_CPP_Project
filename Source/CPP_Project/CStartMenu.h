#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CStartMenu.generated.h"



UCLASS()
class CPP_PROJECT_API UCStartMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	FORCEINLINE bool GetHidden() { return Hidden; }
	FORCEINLINE void SetHidden(bool hidden) { Hidden = hidden; }
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* StartButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* QuitButton;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
private:
	class ACMainPlayerController* MainController;
	UPROPERTY()
		class ACMainPayer* Player;

	class UCSaveGame* SaveGameInstance;
	bool Hidden = false;
private:
	// ��ư�� ������ ��, ȣ��� ��������Ʈ�� ����� �Լ�
	UFUNCTION(BlueprintCallable)
		void StartButtonCallback();

	UFUNCTION(BlueprintCallable)
		void QuitButtonCallback();
};
