#include "Subsystems/CSSessionSubsystem.h"

#include "OnlineSubsystemUtils.h"

UCSSessionSubsystem::UCSSessionSubsystem()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted))
	, UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted))
	, StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionCompleted))
	, EndSessionCompleteDelegate(FOnEndSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnEndSessionCompleted))
	, DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionCompleted))
	, FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompleted))
	, JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompleted))
{
}

void UCSSessionSubsystem::CreateSession(int32 NumPublicConnections, bool IsLANMatch)
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();

	if (!sessionInterface.IsValid())
	{
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = IsLANMatch;
	LastSessionSettings->bShouldAdvertise = true;

	LastSessionSettings->Set(SETTING_MAPNAME, FString("Your Level Name"), EOnlineDataAdvertisementType::ViaOnlineService);

	CreateSessionCompleteDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void UCSSessionSubsystem::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();
	if (sessionInterface)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	OnUpdateSessionCompleteEvent.Broadcast(Successful);
}

void UCSSessionSubsystem::UpdateSession()
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();

	if (!sessionInterface.IsValid())
	{
		OnUpdateSessionCompleteEvent.Broadcast(false);
		return;
	}

	TSharedPtr<FOnlineSessionSettings> updatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*LastSessionSettings));
	updatedSessionSettings->Set(SETTING_MAPNAME, FString("Updated Level Name"), EOnlineDataAdvertisementType::ViaOnlineService);

	UpdateSessionCompleteDelegateHandle =
		sessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);

	if (!sessionInterface->UpdateSession(NAME_GameSession, *LastSessionSettings))
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);

		OnUpdateSessionCompleteEvent.Broadcast(false);
	}
	else
	{
		LastSessionSettings = updatedSessionSettings;
	}
}

void UCSSessionSubsystem::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();
	if (sessionInterface)
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
	}

	OnCreateSessionCompleteEvent.Broadcast(Successful);
}

void UCSSessionSubsystem::StartSession()
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();

	if (!sessionInterface.IsValid())
	{
		OnStartSessionCompleteEvent.Broadcast(false);
		return;
	}

	StartSessionCompleteDelegateHandle =
		sessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

	if (!sessionInterface->StartSession(NAME_GameSession))
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

		OnStartSessionCompleteEvent.Broadcast(false);
	}
}

void UCSSessionSubsystem::OnStartSessionCompleted(FName SessionName, bool Successful)
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();
	if (sessionInterface)
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	OnStartSessionCompleteEvent.Broadcast(Successful);
}

void UCSSessionSubsystem::EndSession()
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();

	if (!sessionInterface.IsValid())
	{
		OnEndSessionCompleteEvent.Broadcast(false);
		return;
	}

	EndSessionCompleteDelegateHandle =
		sessionInterface->AddOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegate);

	if (!sessionInterface->EndSession(NAME_GameSession))
	{
		sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);

		OnEndSessionCompleteEvent.Broadcast(false);
	}
}

void UCSSessionSubsystem::OnEndSessionCompleted(FName SessionName, bool Successful)
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();
	if (sessionInterface)
	{
		sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
	}

	OnEndSessionCompleteEvent.Broadcast(Successful);
}

void UCSSessionSubsystem::DestroySession()
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();

	if (!sessionInterface.IsValid())
	{
		OnDestroySessionCompleteEvent.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle =
		sessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!sessionInterface->DestroySession(NAME_GameSession))
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

		OnDestroySessionCompleteEvent.Broadcast(false);
	}
}

void UCSSessionSubsystem::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();
	if (sessionInterface)
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	OnDestroySessionCompleteEvent.Broadcast(Successful);
}

void UCSSessionSubsystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();

	if (!sessionInterface.IsValid())
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	FindSessionsCompleteDelegateHandle =
		sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLANQuery;

	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UCSSessionSubsystem::OnFindSessionsCompleted(bool Successful)
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();
	if (sessionInterface)
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), Successful);
		return;
	}

	OnFindSessionsCompleteEvent.Broadcast(LastSessionSearch->SearchResults, Successful);
}

void UCSSessionSubsystem::JoinGameSession(const FOnlineSessionSearchResult& SessionResult)
{
		const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();

	if (!sessionInterface.IsValid())
	{
		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle =
		sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	if (!sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UCSSessionSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();
	if (sessionInterface)
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	OnJoinGameSessionCompleteEvent.Broadcast(Result);
}

bool UCSSessionSubsystem::TryTravelToCurrentSession()
{
	IOnlineSubsystem* const subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();

	if (!sessionInterface.IsValid())
	{
		return false;
	}

	FString connectString;
	if (!sessionInterface->GetResolvedConnectString(NAME_GameSession, connectString))
	{
		return false;
	}

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(connectString, TRAVEL_Absolute);
	return true;
}