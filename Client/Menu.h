#pragma once

#include "Page.h"
#include "ClientNetworkEngine.h"
enum MenuStates
{
	MainMenu,
	OneOrTwoPlayersChoosePage,
	Multiplayer,
	MultiplayerChoosingRoom,
	MultiplayerWaitingInRoom,
	MultiplayerWaitingStart,
	Options,
	Game,
	GameOver,
	ExitGame
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
	VoteForStart,
	MultiplayerMatchStart,
	IncreaseGameSpeed,
	DecreaseGameSpeed,
	GoBack
};

struct MenuEvent
{
	MenuEvents Event;
	int AdditionalInfoi; // Should be room id when player chooses room on multiplayer server
	double* AdditionalInfod; // Should be GameSpeed from options
	char* AdditionalInfoc; // Should be server ip when connecting to server
	ClientNetworkEngine* networkengine; // Should be utilized in multiplayer events
};

struct Transition
{
	MenuStates CurrentState;
	MenuEvents Event;
	MenuStates TargetState;
	void (*method)(Menu* menu, MenuEvent Params);
};

class Menu
{
private:
	int CurrentPageID = 0;
	//std::vector<Page> pages;
	/* Hardcoded number of pages and transitions for performance */
	Page pages[10];
	Transition transitions[21];
	MenuStates CurrentState;
	int NumberOfTransitions = 21;
	void FillTransition(MenuStates CurrentState, MenuEvents ExpectedEvent, MenuStates TargetState, void(*method)(Menu* menu, MenuEvent Params), int transIdInArray)
	{
		transitions[transIdInArray].CurrentState = CurrentState;
		transitions[transIdInArray].Event = ExpectedEvent;
		transitions[transIdInArray].TargetState = TargetState;
		transitions[transIdInArray].method = method;
	}
public:
	void FillTransitions();
	void Init(Renderer* rend);
	std::vector<button>& GetButtonsVectorForRenderer();
	void ChangeCurrentState(MenuStates newState);
	//void AddButton(int PageID, bool Clickable, bool Centered, int x, int y, wchar_t *Text, int Size, Renderer* renderer, int UBID);
	void ChangeButtonText(int PageID, int ButtonID, wchar_t* Text);
	int CheckMouseCollision(POINT mouse);
	void RegisterEvent(MenuEvents Event, int AdditionalInfoi, double* AdditionalInfod, char* AdditionalInfoc);
	void HandleEvent(MenuEvent Event);
	void NetworkAddRoomsButtonsWhenConnectingToServer(int NumberOfRooms, int* Rooms);
};

void FuncConnectToServer(Menu* menu, MenuEvent Event)
{
	int NumberOfRooms;
	int* RoomsIds = Event.networkengine->ConnectPart1(Event.AdditionalInfoc, 25565, menu, &NumberOfRooms);
	if (RoomsIds != NULL)
	{
		menu->RegisterEvent(ConnectedToServer, 0, NULL, NULL);
		menu->NetworkAddRoomsButtonsWhenConnectingToServer(NumberOfRooms, RoomsIds);
	}
	
}

void FuncChooseRoom(Menu* menu, MenuEvent Event)
{
	Event.networkengine->ConnectPart2(Event.AdditionalInfoi);
}

void FuncVoteForStart(Menu* menu, MenuEvent Event)
{
	
	if (Event.networkengine->VoteForStart())
	{
		menu->RegisterEvent(MultiplayerMatchStart, 0, NULL, NULL);
	}
}

void FuncIncreaseGameSpeed(Menu* menu, MenuEvent Event)
{
	*Event.AdditionalInfod += 0.02;
}

void FuncDecreaseGameSpeed(Menu* menu, MenuEvent Event)
{
	*Event.AdditionalInfod -= 0.02;
}