#pragma once

#include "Page.h"
#include "ClientNetworkEngine.h"
#include "Console.h"
enum MenuStates
{
	MainMenu,
	OneOrTwoPlayersChoosePage,
	Multiplayer,
	MultiplayerChoosingRoom,
	MultiplayerWaitingInRoom,
	MultiplayerWaitingInRoomAsCreator,
	MultiplayerWaitingStart,
	MultiplayerWaitingStartAsCreator,
	Options,
	Game,
	GameOver,
	ExitGame,
	MultiplayerGame,
	MultiplayerGameAsRoomCreator
};

enum MenuEvents
{
	Start,
	GoToMultiplayer,
	GoToOptions,
	Exit,
	Start1pMode,
	Start2pMode,
	TryConnectToServer,
	ConnectedToServer,
	ChooseRoom,
	CreateNewRoom,
	VoteForStart,
	MultiplayerMatchStart,
	IncreaseGameSpeed,
	DecreaseGameSpeed,
	GoBack,
	PauseGame,
	UnPauseGame
};

struct MenuEvent
{
	MenuEvents menuEvent;
	int additionalInfoInt; // Should be room id when player chooses room on multiplayer server
	float *additionalInfoFloat; // Should be GameSpeed from options
	const char *additionalInfoChar; // Should be server ip when connecting to server
	std::wstring *additionalInfoWstr; // Should be nickname when connecting to server
	ClientNetworkEngine *networkEngine; // Should be utilized in multiplayer events
	Renderer *rend; // For adding buttons
};

class Menu;

struct Transition
{
	MenuStates CurrentState;
	MenuEvents Event;
	MenuStates TargetState;
	void(*method)(Menu *menu, MenuEvent Params);
};

class Menu
{
private:
	//std::vector<Page> pages;
	/* Hardcoded number of pages and transitions for performance */
	Transition transitions[31];
	MenuStates CurrentState;
	int NumberOfTransitions = 31;

	float *gSpeed;
	std::wstring *inputString;
	std::wstring *nickname;
	std::wstring gameSpeed;
	std::wstring pauseText = L"PAUSE";
	ClientNetworkEngine *networkEngine;
	POINT lastMousePos;
	Renderer *renderer;

	void FillTransitions();
	void FillTransition(MenuStates CurrentState, MenuEvents ExpectedEvent, MenuStates TargetState, void(*method)(Menu *menu, MenuEvent Params), int transIdInArray);
	void HandleEvent(MenuEvent Event);
public:
	Page pages[14];

	void Init(Renderer *rend, float *gameSpeed, std::wstring *inputString, std::wstring *nickname, ClientNetworkEngine *networkEngine);
	std::vector<button>& GetButtonsVectorForRenderer();
	MenuStates HandleMouseClick(POINT p);
	void HandleMouseMovement(POINT p);
	void RegisterEvent(MenuEvents Event, int additionalInfoInt, float *additionalInfoFloat, const char *additionalInfoChar, std::wstring *additionalInfoWstr, ClientNetworkEngine *networkEngine, Renderer *rend);
	const MenuStates GetMenuState() { return CurrentState; }
	ClientNetworkEngine *GetNetworkEngine() { return networkEngine; }
};