#include "CPP_ProjectGameModeBase.h"
#include "Global.h"
#include "Player/CMainPayer.h"
ACPP_ProjectGameModeBase::ACPP_ProjectGameModeBase()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CMainPayer.BP_CMainPayer_C'");
	CHelpers::GetClass<APlayerController>(&PlayerControllerClass, "Blueprint'/Game/Player/BP_CMainPlayerController.BP_CMainPlayerController_C'");
}
