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
	MultiplayerWaitingStart,
	Options,
	Game,
	GameOver,
	ExitGame,
	MultiplayerGame
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
	const char* AdditionalInfoc; // Should be server ip when connecting to server
	ClientNetworkEngine* networkEngine; // Should be utilized in multiplayer events
};

class Menu;

struct Transition
{
	MenuStates CurrentState;
	MenuEvents Event;
	MenuStates TargetState;
	void(*method)(Menu* menu, MenuEvent Params);
};

class Menu
{
private:
	//std::vector<Page> pages;
	/* Hardcoded number of pages and transitions for performance */
	Page pages[11];
	Transition transitions[23];
	MenuStates CurrentState;
	int NumberOfTransitions = 23;

	double* gSpeed;
	std::wstring* inputString;
	ClientNetworkEngine* networkEngine;

	Console out;

	void FillTransitions();
	void FillTransition(MenuStates CurrentState, MenuEvents ExpectedEvent, MenuStates TargetState, void(*method)(Menu* menu, MenuEvent Params), int transIdInArray);
	void ChangeCurrentState(MenuStates newState);
	void HandleEvent(MenuEvent Event);
public:
	void Init(Renderer* rend, double* gameSpeed, std::wstring* inputString, ClientNetworkEngine* networkEngine);
	std::vector<button>& GetButtonsVectorForRenderer();
	void ChangeButtonText(int PageID, int ButtonID, wchar_t* Text);
	MenuStates HandleMouseClick(POINT p);
	void HandleMouseMovement(POINT p);
	void NetworkAddRoomsButtonsWhenConnectingToServer(int NumberOfRooms, int* Rooms);
	void RegisterEvent(MenuEvents Event, int AdditionalInfoi, double* AdditionalInfod, const char* AdditionalInfoc, ClientNetworkEngine* networkEngine);
	const MenuStates GetMenuState() { return CurrentState; }
	ClientNetworkEngine* GetNetworkEngine() { return networkEngine; }
};

inline void FuncConnectToServer(Menu* menu, MenuEvent Event)
{
	int NumberOfRooms;
	int* RoomsIds = Event.networkEngine->ConnectPart1(Event.AdditionalInfoc, 25565, &NumberOfRooms);
	if (RoomsIds != NULL)
	{
		menu->RegisterEvent(ConnectedToServer, 0, NULL, NULL, menu->GetNetworkEngine());
		menu->NetworkAddRoomsButtonsWhenConnectingToServer(NumberOfRooms, RoomsIds);
	}
	
}

inline void FuncChooseRoom(Menu* menu, MenuEvent Event)
{
	Event.networkEngine->ConnectPart2(Event.AdditionalInfoi);
}

inline void FuncVoteForStart(Menu* menu, MenuEvent Event)
{
	
	if (Event.networkEngine->VoteForStart())
	{
		menu->RegisterEvent(MultiplayerMatchStart, 0, NULL, NULL, menu->GetNetworkEngine());
	}
}

inline void FuncIncreaseGameSpeed(Menu* menu, MenuEvent Event)
{
	*Event.AdditionalInfod += 0.02;
	static std::wstring str;
	str.clear();
	str = std::to_wstring(*Event.AdditionalInfod);
	if (str.size() >= 6)
	{
		try
		{
			str.pop_back();
			str.pop_back();
			str.pop_back();
			str.pop_back();
		}
		catch (std::out_of_range)
		{
			str.clear();
			str = std::to_wstring(*Event.AdditionalInfod);
		}
	}
	menu->ChangeButtonText(Options, 1, const_cast<wchar_t*>(str.c_str()));
}

inline void FuncDecreaseGameSpeed(Menu* menu, MenuEvent Event)
{
	*Event.AdditionalInfod -= 0.02;
	static std::wstring str;
	str.clear();
	str = std::to_wstring(*Event.AdditionalInfod);
	if (str.size() >= 6)
	{
		try
		{
			str.pop_back();
			str.pop_back();
			str.pop_back();
			str.pop_back();
		}
		catch (std::out_of_range)
		{
			str.clear();
			str = std::to_wstring(*Event.AdditionalInfod);
		}
	}
	menu->ChangeButtonText(Options, 1, const_cast<wchar_t*>(str.c_str()));
}

inline void FuncDisconnectFromServer(Menu* menu, MenuEvent Event)
{
	Event.networkEngine->Disconnect();
}