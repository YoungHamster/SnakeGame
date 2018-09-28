#pragma once

#include "Page.h"
#define MULTIPLAYER 1
#define OPTIONS 2

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
	Connect,
	ChooseRoom,
	VoteForStart,
	MultiplayerMatchStart,
	IncreaseGameSpeed,
	DecreaseGameSpeed
};

struct MenuEvent
{
	MenuEvents Event;
	int AdditionalInfoi; // Should be room id when player chooses room on multiplayer server
	double* AdditionalInfod; // Should be GameSpeed from options
	char* AdditionalInfoc; // Should be server ip when connecting to server
	NetworkEngine* networkengine; // Should be utilized in multiplayer events
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
	std::vector<Page> pages;
	Transition transitions[11 * 8];
	MenuStates CurrentState;
	int NumberOfTransitions = 11 * 8;
	void FillTransition(MenuStates CurrentState, MenuEvents ExpectedEvent, MenuStates TargetState, void(*method)(Menu* menu, MenuEvent Params), int transIdInArray)
	{
		transitions[transIdInArray].CurrentState = CurrentState;
		transitions[transIdInArray].Event = ExpectedEvent;
		transitions[transIdInArray].TargetState = TargetState;
		transitions[transIdInArray].method = method;
	}
public:
	void FillTransitions()
	{
		for (int i = 0; i < NumberOfTransitions; i++)
		{
			transitions[i].method = NULL;
		}
		transitions[0].CurrentState = MainMenu;
		transitions[0].Event = Start;
		transitions[0].TargetState = OneOrTwoPlayersChoosePage;

		transitions[1].CurrentState = OneOrTwoPlayersChoosePage;
		transitions[1].Event = Start1pMode;
		transitions[1].TargetState = Game;

		transitions[2].CurrentState = OneOrTwoPlayersChoosePage;
		transitions[2].Event = Start2pMode;
		transitions[2].TargetState = Game;

		transitions[3].CurrentState = MainMenu;
		transitions[3].Event = GoToMultiplayer;
		transitions[3].TargetState = Multiplayer;

		transitions[4].CurrentState = MainMenu;
		transitions[4].Event = GoToOptions;
		transitions[4].TargetState = Options;

		transitions[5].CurrentState = MainMenu;
		transitions[5].Event = Exit;
		transitions[5].TargetState = ExitGame;

		transitions[6].CurrentState = Multiplayer;
		transitions[6].Event = Connect;
		transitions[6].TargetState = MultiplayerChoosingRoom;
		transitions[6].method = FuncConnectToServer;

		transitions[7].CurrentState = MultiplayerChoosingRoom;
		transitions[7].Event = ChooseRoom;
		transitions[7].TargetState = MultiplayerWaitingInRoom;
		transitions[7].method = FuncChooseRoom;

		transitions[8].CurrentState = MultiplayerWaitingInRoom;
		transitions[8].Event = VoteForStart;
		transitions[8].TargetState = MultiplayerWaitingStart;
		transitions[8].method = FuncVoteForStart;

		transitions[9].CurrentState = MultiplayerWaitingStart;
		transitions[9].Event = MultiplayerMatchStart;
		transitions[9].TargetState = Game;

		transitions[10].CurrentState = ;
		transitions[10].Event = ;
		transitions[10].TargetState = ;

		transitions[11].CurrentState = ;
		transitions[11].Event = ;
		transitions[11].TargetState = ;

		transitions[12].CurrentState = ;
		transitions[12].Event = ;
		transitions[12].TargetState = ;
	}
	void Init(Renderer* rend);
	std::vector<button>& GetButtonsVectorForRenderer();
	//std::vector<button>& GetButtonsVectorForRenderer(int PageID);
	void ChangePage(int PageID);
	void AddPage();
	void AddButton(int PageID, bool Clickable, bool Centered, int x, int y, wchar_t *Text, int Size, Renderer* renderer, int UBID);
	void ChangeButtonText(int PageID, int ButtonID, wchar_t* Text);
	int CheckMouseCollision(POINT mouse);
	void RegisterEvent(MenuEvents Event, int AdditionalInfoi, double* AdditionalInfod, char* AdditionalInfoc);
	void HandleEvent(MenuEvent Event);
	/*void AddButton(int PageID, button button);*/
};

void FuncConnectToServer(Menu* menu, MenuEvent Event)
{

}

void FuncChooseRoom(Menu* menu, MenuEvent Event)
{

}

void FuncVoteForStart(Menu* menu, MenuEvent Event)
{
	if (Event.networkengine.VoteForStart())
	{
		menu->RegisterEvent(MultiplayerMatchStart, 0, NULL, NULL);
	}
}

void FuncChangeGameSpeed(Menu* menu, MenuEvent Event)
{

}