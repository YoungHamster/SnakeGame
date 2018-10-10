#include "Menu.h"

void Menu::FillTransitions()
	{
		for (int i = 0; i < NumberOfTransitions; i++)
		{
			transitions[i].method = NULL;
		}

		/* FSM transitions table */
		/*             Current menu state         Menu event             Target state               Helping function          Transition number in transitions array */
		FillTransition(MainMenu,                  Start,                 OneOrTwoPlayersChoosePage, NULL,                     0);
		FillTransition(OneOrTwoPlayersChoosePage, Start1pMode,           Game,                      NULL,                     1);
		FillTransition(OneOrTwoPlayersChoosePage, Start2pMode,           Game,                      NULL,                     2);
		FillTransition(MainMenu,                  GoToMultiplayer,       Multiplayer,               NULL,                     3);
		FillTransition(MainMenu,                  GoToOptions,           Options,                   NULL,                     4);
		FillTransition(MainMenu,                  Exit,                  ExitGame,                  NULL,                     5);
		FillTransition(Multiplayer,               TryConnectToServer,    Multiplayer,               FuncConnectToServer,      6);
		FillTransition(Multiplayer,               ConnectedToServer,     MultiplayerChoosingRoom,   NULL,                     7);
		FillTransition(MultiplayerChoosingRoom,   ChooseRoom,            MultiplayerWaitingInRoom,  FuncChooseRoom,           8);
		FillTransition(MultiplayerWaitingInRoom,  VoteForStart,          MultiplayerWaitingStart,   FuncVoteForStart,         9);
		FillTransition(MultiplayerWaitingStart,   MultiplayerMatchStart, MultiplayerGame,		    NULL,                     10);
		FillTransition(Options,                   IncreaseGameSpeed,     Options,                   FuncIncreaseGameSpeed,    11);
		FillTransition(Options,                   DecreaseGameSpeed,     Options,                   FuncDecreaseGameSpeed,    12);
		FillTransition(OneOrTwoPlayersChoosePage, GoBack,                MainMenu,                  NULL,                     13);
		FillTransition(Multiplayer,               GoBack,                MainMenu,                  NULL,                     14);
		FillTransition(MultiplayerChoosingRoom,   GoBack,                Multiplayer,               FuncDisconnectFromServer, 15);
		FillTransition(MultiplayerWaitingInRoom,  GoBack,                Multiplayer,               FuncDisconnectFromServer, 16);
		FillTransition(MultiplayerWaitingStart,   GoBack,                Multiplayer,               FuncDisconnectFromServer, 17);
		FillTransition(Options,                   GoBack,                MainMenu,                  NULL,                     18);
		FillTransition(Game,                      GoBack,                MainMenu,                  NULL,                     19);
		FillTransition(GameOver,                  GoBack,                MainMenu,                  NULL,                     20);
		FillTransition(MultiplayerGame,           GoBack,                Multiplayer,               FuncDisconnectFromServer, 21);
	}

void Menu::Init(Renderer* rend, double* gameSpeed, std::wstring* inputString, ClientNetworkEngine* networkEngine)
{
	this->networkEngine = networkEngine;
	gSpeed = gameSpeed;
	this->inputString = inputString;

	FillTransitions();
	pages[MainMenu].AddButton(                 true, true, 0, 80,  const_cast<wchar_t*>(L"START"),             10, rend, Start,              0);
	pages[MainMenu].AddButton(                 true, true, 0, 290, const_cast<wchar_t*>(L"MULTIPLAYER"),       10, rend, GoToMultiplayer,    0);
	pages[MainMenu].AddButton(                 true, true, 0, 470, const_cast<wchar_t*>(L"OPTIONS"),           10, rend, GoToOptions,        0);
	pages[MainMenu].AddButton(                 true, true, 0, 630, const_cast<wchar_t*>(L"EXIT"),              10, rend, Exit,               0);
	pages[Multiplayer].AddButton(              true, true, 0, 200, const_cast<wchar_t*>(L"CONNECT"),           10, rend, TryConnectToServer, 0);
	pages[Multiplayer].AddButton(              true, true, 0, 400, const_cast<wchar_t*>(inputString->c_str()), 10, rend, -1, 0);
	pages[Multiplayer].AddButton(              true, true, 0, 600, const_cast<wchar_t*>(L"BACK"),              10, rend, GoBack,             0);
	pages[OneOrTwoPlayersChoosePage].AddButton(true, true, 0, 200, const_cast<wchar_t*>(L"1 PLAYER MODE"),     10, rend, Start1pMode,        0);
	pages[OneOrTwoPlayersChoosePage].AddButton(true, true, 0, 400, const_cast<wchar_t*>(L"2 PLAYERS MODE"),    10, rend, Start2pMode,        0);
	pages[OneOrTwoPlayersChoosePage].AddButton(true, true, 0, 600, const_cast<wchar_t*>(L"BACK"),              10, rend, GoBack,             0);
	pages[MultiplayerChoosingRoom].AddButton(  true, true, 0, 200, const_cast<wchar_t*>(L"NEW ROOM"),          10, rend, ChooseRoom,         CREATE_NEW_ROOM_ON_SERVER);
	pages[MultiplayerChoosingRoom].AddButton(  true, true, 0, 600, const_cast<wchar_t*>(L"BACK"),              10, rend, GoBack,             0);
	pages[MultiplayerWaitingInRoom].AddButton( true, true, 0, 400, const_cast<wchar_t*>(L"VOTE FOR START"),    10, rend, VoteForStart,       0);
	pages[MultiplayerWaitingInRoom].AddButton( true, true, 0, 600, const_cast<wchar_t*>(L"BACK"),              10, rend, GoBack,             0);
	pages[Options].AddButton(                  true, true, 0, 200, const_cast<wchar_t*>(L"+"),                 10, rend, IncreaseGameSpeed,  0);
	pages[Options].AddButton(                  false,true, 0, 300, const_cast<wchar_t*>(L"1.0"),               10, rend, -1,                 0);
	pages[Options].AddButton(                  true, true, 0, 400, const_cast<wchar_t*>(L"-"),                 10, rend, DecreaseGameSpeed,  0);
	pages[Options].AddButton(                  true, true, 0, 600, const_cast<wchar_t*>(L"BACK"),              10, rend, GoBack,             0);
	pages[Game].AddButton(                     true, true, 0, 20,  const_cast<wchar_t*>(L"BACK"),              4,  rend, GoBack,             0);
	pages[MultiplayerGame].AddButton(                     true, true, 0, 20,  const_cast<wchar_t*>(L"BACK"),   4,  rend, GoBack,             0);

	CurrentState = MainMenu;
}

std::vector<button>& Menu::GetButtonsVectorForRenderer()
{
	return pages[CurrentState].buttons;
}

void Menu::ChangeButtonText(int PageID, int ButtonID, wchar_t* Text)
{
	/*if (pages[PageID].buttons[ButtonID].Text)
	{
		delete[] pages[PageID].buttons[ButtonID].Text;
	}*/
	pages[PageID].buttons[ButtonID].Text = Text;
}

MenuStates Menu::HandleMouseClick(POINT mouse)
{
	int buttonid = pages[CurrentState].CheckMouseCollisions(mouse);
	if (buttonid < 0 || buttonid > pages[CurrentState].buttons.size())
	{
		return CurrentState;
	}
	const char* AdditionalInfoc = NULL;
	double* AdditionalInfod = NULL;
	int AdditionalInfoi = 0;
	if (pages[CurrentState].buttons[buttonid].UBID == TryConnectToServer)
	{
		static std::string ip;
		ip.clear();
		ip = std::string(inputString->begin(), inputString->end());
		AdditionalInfoc = ip.c_str()/* pointer to string with server ip */;
	}
	if (pages[CurrentState].buttons[buttonid].UBID == ChooseRoom)
	{
		AdditionalInfoi = pages[CurrentState].buttons[buttonid].AdditionalInfo;
	}
	if (pages[CurrentState].buttons[buttonid].UBID == IncreaseGameSpeed || pages[CurrentState].buttons[buttonid].UBID == DecreaseGameSpeed)
	{
		AdditionalInfod = gSpeed/* pointer to GameSpeed*/;
	}
	if (pages[CurrentState].buttons[buttonid].UBID >= 0 && pages[CurrentState].buttons[buttonid].UBID <= 13)
	{
		RegisterEvent((MenuEvents)pages[CurrentState].buttons[buttonid].UBID, AdditionalInfoi, AdditionalInfod, AdditionalInfoc, networkEngine);
	}
	return CurrentState;
}
void Menu::RegisterEvent(MenuEvents Event, int AdditionalInfoi, double* AdditionalInfod, const char* AdditionalInfoc, ClientNetworkEngine* networkEngine)
{
	//out.Write("Registered event\n");
	MenuEvent NewEvent;
	NewEvent.Event = Event;
	NewEvent.AdditionalInfoi = AdditionalInfoi;
	NewEvent.AdditionalInfod = AdditionalInfod;
	NewEvent.AdditionalInfoc = AdditionalInfoc;
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
		if (transitions[i].CurrentState == CurrentState && transitions[i].Event == Event.Event)
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