#include "Menu.h"

inline void FuncConnectToServer(Menu* menu, MenuEvent Event);
inline void FuncJoinRoom(Menu* menu, MenuEvent Event);
inline void FuncCreateRoom(Menu* menu, MenuEvent Event);
inline void FuncVoteForStart(Menu* menu, MenuEvent Event);
inline void FuncIncreaseGameSpeed(Menu* menu, MenuEvent Event);
inline void FuncDecreaseGameSpeed(Menu* menu, MenuEvent Event);
inline void FuncDisconnectFromServer(Menu* menu, MenuEvent Event);
inline void FuncLeaveServerRoom(Menu* menu, MenuEvent Event);
inline void FuncPauseServerGame(Menu* menu, MenuEvent Event);
inline void FuncPauseOfflineGame(Menu* menu, MenuEvent Event);

void Menu::FillTransitions()
{
	/* FSM transitions table */
	/*             Current menu state                 Menu event             Target state                       Helping function          Transition number in transitions array */
	FillTransition(MainMenu,                          Start,                 OneOrTwoPlayersChoosePage,         NULL,                     0);
	FillTransition(OneOrTwoPlayersChoosePage,         Start1pMode,           Game,                              NULL,                     1);
	FillTransition(OneOrTwoPlayersChoosePage,         Start2pMode,           Game,                              NULL,                     2);
	FillTransition(Game,                              PauseGame,             Game,                              FuncPauseOfflineGame,     3);
	FillTransition(MainMenu,                          GoToMultiplayer,       Multiplayer,                       NULL,                     4);
	FillTransition(MainMenu,                          GoToOptions,           Options,                           NULL,                     5);
	FillTransition(MainMenu,                          Exit,                  ExitGame,                          NULL,                     6);
	FillTransition(Multiplayer,                       TryConnectToServer,    Multiplayer,                       FuncConnectToServer,      7);
	FillTransition(Multiplayer,                       ConnectedToServer,     MultiplayerChoosingRoom,           NULL,                     8);
	FillTransition(MultiplayerChoosingRoom,           ChooseRoom,            MultiplayerWaitingInRoom,          FuncJoinRoom,             9);
	FillTransition(MultiplayerChoosingRoom,           CreateNewRoom,         MultiplayerWaitingInRoomAsCreator, FuncCreateRoom,           10);
	FillTransition(MultiplayerWaitingInRoom,          VoteForStart,          MultiplayerWaitingStart,           FuncVoteForStart,         11);
	FillTransition(MultiplayerWaitingInRoomAsCreator, VoteForStart,          MultiplayerWaitingStartAsCreator,  FuncVoteForStart,         12);
	FillTransition(MultiplayerWaitingStart,           MultiplayerMatchStart, MultiplayerGame,		            NULL,                     13);
	FillTransition(MultiplayerWaitingStartAsCreator,  MultiplayerMatchStart, MultiplayerGameAsRoomCreator,      NULL,                     14);
	FillTransition(MultiplayerGameAsRoomCreator,      PauseGame,             MultiplayerGameAsRoomCreator,      FuncPauseServerGame,      15);
	FillTransition(Options,                           IncreaseGameSpeed,     Options,                           FuncIncreaseGameSpeed,    16);
	FillTransition(Options,                           DecreaseGameSpeed,     Options,                           FuncDecreaseGameSpeed,    17);
	FillTransition(OneOrTwoPlayersChoosePage,         GoBack,                MainMenu,                          NULL,                     18);
	FillTransition(Multiplayer,                       GoBack,                MainMenu,                          NULL,                     19);
	FillTransition(MultiplayerChoosingRoom,           GoBack,                Multiplayer,                       FuncDisconnectFromServer, 20);
	FillTransition(MultiplayerWaitingInRoom,          GoBack,                MultiplayerChoosingRoom,           FuncLeaveServerRoom,      21);
	FillTransition(MultiplayerWaitingStart,           GoBack,                MultiplayerChoosingRoom,           FuncLeaveServerRoom,      22);
	FillTransition(Options,                           GoBack,                MainMenu,                          NULL,                     23);
	FillTransition(Game,                              GoBack,                MainMenu,                          NULL,                     24);
	FillTransition(GameOver,                          GoBack,                MainMenu,                          NULL,                     25);
	FillTransition(MultiplayerGame,                   GoBack,                MultiplayerChoosingRoom,           FuncLeaveServerRoom,      26);
	FillTransition(MultiplayerGameAsRoomCreator,      GoBack,                MultiplayerChoosingRoom,           FuncLeaveServerRoom,      27);
	FillTransition(MultiplayerWaitingInRoomAsCreator, GoBack,                MultiplayerChoosingRoom,           FuncLeaveServerRoom,      28);
	FillTransition(MultiplayerWaitingStartAsCreator,  GoBack,                MultiplayerChoosingRoom,           FuncLeaveServerRoom,      29);
}

void Menu::Init(Renderer* rend, float* gSpeed, std::wstring* inputString, std::wstring* nickname, ClientNetworkEngine* networkEngine)
{
	this->networkEngine = networkEngine;
	this->gSpeed = gSpeed;
	this->inputString = inputString;
	this->nickname = nickname;
	gameSpeed = std::to_wstring(*gSpeed);

	FillTransitions();
	/*                                         Clickable   Centered   x  y    Changable string Const cstring      Size Renderer  Event               Additional info*/
	pages[MainMenu].AddButton(                 false,      true,      0, 40,  NULL,            L"NICKNAME:",      6,   rend,     -1,                 0);
	pages[MainMenu].AddButton(                 false,      true,	  0, 100, nickname,        std::wstring(),    7,   rend,     -1,                 0);
	pages[MainMenu].AddButton(                 true,       true,	  0, 240, NULL,			   L"START",          8,   rend,	 Start,              0);
	pages[MainMenu].AddButton(                 true,       true,	  0, 370, NULL,			   L"MULTIPLAYER",    8,   rend,	 GoToMultiplayer,    0);
	pages[MainMenu].AddButton(                 true,       true,	  0, 500, NULL,			   L"OPTIONS",        8,   rend,	 GoToOptions,        0);
	pages[MainMenu].AddButton(                 true,       true,	  0, 630, NULL,            L"EXIT",           8,   rend,	 Exit,               0);
	pages[Multiplayer].AddButton(              true,       true,	  0, 200, NULL,			   L"CONNECT",        8,   rend,	 TryConnectToServer, 0);
	pages[Multiplayer].AddButton(              true,       true,      0, 400, inputString,	   std::wstring(),    8,   rend,	 -1,                 0);
	pages[Multiplayer].AddButton(              true,	   true,	  0, 600, NULL,            L"BACK",           7 ,  rend,	 GoBack,             0);
	pages[OneOrTwoPlayersChoosePage].AddButton(true,	   true,	  0, 200, NULL,            L"1 PLAYER MODE",  10,  rend,	 Start1pMode,        0);
	pages[OneOrTwoPlayersChoosePage].AddButton(true,	   true,	  0, 400, NULL,            L"2 PLAYERS MODE", 10,  rend,	 Start2pMode,        0);
	pages[OneOrTwoPlayersChoosePage].AddButton(true,	   true,	  0, 600, NULL,            L"BACK",           7 ,  rend,	 GoBack,             0);
	pages[MultiplayerChoosingRoom].AddButton(  true,	   true,	  0, 120, NULL,            L"NEW ROOM",       10,  rend,	 ChooseRoom,         CREATE_NEW_ROOM_ON_SERVER);
	pages[MultiplayerChoosingRoom].AddButton(  true,	   true,	  0, 600, NULL,            L"BACK",           10,  rend,	 GoBack,             0);
	pages[MultiplayerWaitingInRoom].AddButton( true,	   true,	  0, 400, NULL,            L"VOTE FOR START", 10,  rend,	 VoteForStart,       0);
	pages[MultiplayerWaitingInRoom].AddButton( true,	   true,      0, 600, NULL,            L"BACK",           7 ,  rend,	 GoBack,             0);
	pages[Options].AddButton(                  true,	   true,	  0, 200, NULL,            L"+",              10,  rend,	 IncreaseGameSpeed,  0);
	pages[Options].AddButton(                  false,	   true,	  0, 300, &gameSpeed,      std::wstring(),    10,  rend,	 -1,                 0);
	pages[Options].AddButton(                  true,	   true,	  0, 400, NULL,            L"-",              10,  rend,	 DecreaseGameSpeed,  0);
	pages[Options].AddButton(                  true,	   true,	  0, 600, NULL,            L"BACK",           7,   rend,	 GoBack,             0);
	pages[Game].AddButton(                     true,	   true,	  0, 20,  NULL,            L"BACK",           7,   rend,	 GoBack,             0);
	pages[MultiplayerGame].AddButton(          true,	   true,	  0, 20,  NULL,            L"BACK",           7,   rend,	 GoBack,             0);

	CurrentState = MainMenu;
}

std::vector<button>& Menu::GetButtonsVectorForRenderer()
{
	return pages[CurrentState].buttons;
}

MenuStates Menu::HandleMouseClick(POINT mouse)
{
	int buttonid;
	if (mouse.x == -1)
	{
		buttonid = pages[CurrentState].CheckMouseCollisions(lastMousePos);
	}
	else
	{
		buttonid = pages[CurrentState].CheckMouseCollisions(mouse);
	}
	if (buttonid < 0 || buttonid > pages[CurrentState].buttons.size())
	{
		return CurrentState;
	}
	const char* AdditionalInfoc = NULL;
	float* AdditionalInfof = NULL;
	int AdditionalInfoi = 0;
	std::wstring *additionalInfoWstr = NULL;
	if (pages[CurrentState].buttons[buttonid].UBID == TryConnectToServer)
	{
		static std::string ip;
		ip.clear();
		ip = std::string(inputString->begin(), inputString->end());
		AdditionalInfoc = ip.c_str()/* pointer to string with server ip */;
		additionalInfoWstr = nickname;
	}
	if (pages[CurrentState].buttons[buttonid].UBID == ChooseRoom)
	{
		AdditionalInfoi = pages[CurrentState].buttons[buttonid].AdditionalInfo;
	}
	if (pages[CurrentState].buttons[buttonid].UBID == IncreaseGameSpeed || pages[CurrentState].buttons[buttonid].UBID == DecreaseGameSpeed)
	{
		AdditionalInfof = gSpeed/* pointer to GameSpeed*/;
		additionalInfoWstr = &gameSpeed;
	}
	if (pages[CurrentState].buttons[buttonid].UBID >= 0 && pages[CurrentState].buttons[buttonid].UBID <= 15)
	{
		RegisterEvent((MenuEvents)pages[CurrentState].buttons[buttonid].UBID, AdditionalInfoi, AdditionalInfof, AdditionalInfoc, additionalInfoWstr, networkEngine);
	}
	return CurrentState;
}

void Menu::RegisterEvent(MenuEvents Event, int additionalInfoInt, float* additionalInfoFloat, const char* additionalInfoChar, std::wstring* additionalInfoWstr, ClientNetworkEngine* networkEngine)
{
	//out.Write("Registered event\n");
	MenuEvent NewEvent;
	NewEvent.menuEvent = Event;
	NewEvent.additionalInfoInt = additionalInfoInt;
	NewEvent.additionalInfoFloat = additionalInfoFloat;
	NewEvent.additionalInfoChar = additionalInfoChar;
	NewEvent.additionalInfoWstr = additionalInfoWstr;
	NewEvent.networkEngine = networkEngine;
	//out.Write(std::string("Prev state ") + std::to_string((int)CurrentState) + std::string("\n"));
	//out.Write(std::string("Event ") + std::to_string((int)Event) + std::string("\n"));
	HandleEvent(NewEvent);
	//out.Write(std::string("New state ") + std::to_string((int)CurrentState) + std::string("\n"));
}

void Menu::HandleEvent(MenuEvent Event)
{
	for (int i = 0; i < NumberOfTransitions; i++)
	{
		if (transitions[i].CurrentState == CurrentState && transitions[i].Event == Event.menuEvent)
		{
			CurrentState = transitions[i].TargetState;
			if (transitions[i].method != NULL)
			{
				transitions[i].method(this, Event);
			}
		}
	}
}

void Menu::FillTransition(MenuStates CurrentState, MenuEvents ExpectedEvent, MenuStates TargetState, void(*method)(Menu* menu, MenuEvent Params), int transIdInArray)
{
	transitions[transIdInArray].CurrentState = CurrentState;
	transitions[transIdInArray].Event = ExpectedEvent;
	transitions[transIdInArray].TargetState = TargetState;
	transitions[transIdInArray].method = method;
}

void Menu::NetworkAddRoomsButtonsWhenConnectingToServer(int NumberOfRooms, int* Rooms)
{
	return;
}

void Menu::HandleMouseMovement(POINT p)
{
	pages[CurrentState].CheckMouseCollisions(p);
}

inline void FuncConnectToServer(Menu* menu, MenuEvent Event)
{
	if (Event.networkEngine->ConnectToServer(Event.additionalInfoWstr, Event.additionalInfoChar, 25565))
	{
		menu->RegisterEvent(ConnectedToServer, 0, NULL, NULL, NULL, menu->GetNetworkEngine());
	}
}

inline void FuncJoinRoom(Menu* menu, MenuEvent Event)
{
	Event.networkEngine->JoinExistingRoom(Event.additionalInfoInt);
}

inline void FuncCreateRoom(Menu* menu, MenuEvent Event)
{
	Event.networkEngine->CreateNewRoom();
}

inline void FuncVoteForStart(Menu* menu, MenuEvent Event)
{

	if (Event.networkEngine->VoteForStart())
	{
		menu->RegisterEvent(MultiplayerMatchStart, 0, NULL, NULL, NULL, menu->GetNetworkEngine());
	}
}

inline void FuncIncreaseGameSpeed(Menu* menu, MenuEvent Event)
{
	*Event.additionalInfoFloat += 0.02;
	Event.additionalInfoWstr->clear();
	*Event.additionalInfoWstr = std::to_wstring(*Event.additionalInfoFloat);
	if (Event.additionalInfoWstr->size() >= 6)
	{
		try
		{
			Event.additionalInfoWstr->pop_back();
			Event.additionalInfoWstr->pop_back();
			Event.additionalInfoWstr->pop_back();
			Event.additionalInfoWstr->pop_back();
		}
		catch (std::out_of_range)
		{
			Event.additionalInfoWstr->clear();
			Event.additionalInfoWstr = &std::to_wstring(*Event.additionalInfoFloat);
		}
	}
}

inline void FuncDecreaseGameSpeed(Menu* menu, MenuEvent Event)
{
	*Event.additionalInfoFloat -= 0.02;
	Event.additionalInfoWstr->clear();
	*Event.additionalInfoWstr = std::to_wstring(*Event.additionalInfoFloat);
	if (Event.additionalInfoWstr->size() >= 6)
	{
		try
		{
			Event.additionalInfoWstr->pop_back();
			Event.additionalInfoWstr->pop_back();
			Event.additionalInfoWstr->pop_back();
			Event.additionalInfoWstr->pop_back();
		}
		catch (std::out_of_range)
		{
			Event.additionalInfoWstr->clear();
			Event.additionalInfoWstr = &std::to_wstring(*Event.additionalInfoFloat);
		}
	}
}

inline void FuncDisconnectFromServer(Menu* menu, MenuEvent Event)
{
	Event.networkEngine->Disconnect();
}

inline void FuncLeaveServerRoom(Menu* menu, MenuEvent Event)
{
	Event.networkEngine->LeaveRoom();
}

inline void FuncPauseServerGame(Menu* menu, MenuEvent Event)
{
	Event.networkEngine->PauseMatch();
}

inline void FuncPauseOfflineGame(Menu* menu, MenuEvent Event)
{

}