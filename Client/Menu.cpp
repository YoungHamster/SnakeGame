#include "Menu.h"

void Menu::FillTransitions()
	{
		for (int i = 0; i < NumberOfTransitions; i++)
		{
			transitions[i].method = NULL;
		}

		/* FSM transitions table */
		/*             Current menu state         Menu event             Target state               Helping function       Transition number in transitions array */
		FillTransition(MainMenu,                  Start,                 OneOrTwoPlayersChoosePage, NULL,                  0);
		FillTransition(OneOrTwoPlayersChoosePage, Start1pMode,           Game,                      NULL,                  1);
		FillTransition(OneOrTwoPlayersChoosePage, Start2pMode,           Game,                      NULL,                  2);
		FillTransition(MainMenu,                  GoToMultiplayer,       Multiplayer,               NULL,                  3);
		FillTransition(MainMenu,                  GoToOptions,           Options,                   NULL,                  4);
		FillTransition(MainMenu,                  Exit,                  ExitGame,                  NULL,                  5);
		FillTransition(Multiplayer,               TryConnectToServer,    Multiplayer,               FuncConnectToServer,   6);
		FillTransition(Multiplayer,               ConnectedToServer,     MultiplayerChoosingRoom,   NULL,                  7);
		FillTransition(MultiplayerChoosingRoom,   ChooseRoom,            MultiplayerWaitingInRoom,  FuncChooseRoom,        8);
		FillTransition(MultiplayerWaitingInRoom,  VoteForStart,          MultiplayerWaitingStart,   FuncVoteForStart,      9);
		FillTransition(MultiplayerWaitingStart,   MultiplayerMatchStart, Game,		                NULL,                  10);
		FillTransition(Options,                   IncreaseGameSpeed,     Options,                   FuncIncreaseGameSpeed, 11);
		FillTransition(Options,                   DecreaseGameSpeed,     Options,                   FuncDecreaseGameSpeed, 12);
		FillTransition(OneOrTwoPlayersChoosePage, GoBack,                MainMenu,                  NULL,                  13);
		FillTransition(Multiplayer,               GoBack,                MainMenu,                  NULL,                  14);
		FillTransition(MultiplayerChoosingRoom,   GoBack,                Multiplayer,               NULL,                  15);
		FillTransition(MultiplayerWaitingInRoom,  GoBack,                Multiplayer,               NULL,                  16);
		FillTransition(MultiplayerWaitingStart,   GoBack,                Multiplayer,               NULL,                  17);
		FillTransition(Options,                   GoBack,                MainMenu,                  NULL,                  18);
		FillTransition(Game,                      GoBack,                MainMenu,                  NULL,                  19);
		FillTransition(GameOver,                  GoBack,                MainMenu,                  NULL,                  20);
	}

void Menu::Init(Renderer* rend, double* gameSpeed, std::wstring* inputString)
{
	this->gameSpeed = gameSpeed;
	this->inputString = inputString;

	FillTransitions();
	pages[MainMenu].AddButton(                 true, true, 0, 80,  const_cast<wchar_t*>(L"START"),          10, rend, Start,              0);
	pages[MainMenu].AddButton(                 true, true, 0, 290, const_cast<wchar_t*>(L"MULTIPLAYER"),    10, rend, GoToMultiplayer,    0);
	pages[MainMenu].AddButton(                 true, true, 0, 470, const_cast<wchar_t*>(L"OPTIONS"),        10, rend, GoToOptions,        0);
	pages[MainMenu].AddButton(                 true, true, 0, 630, const_cast<wchar_t*>(L"EXIT"),           10, rend, Exit,               0);
	pages[Multiplayer].AddButton(              true, true, 0, 200, const_cast<wchar_t*>(L"CONNECT"),        10, rend, TryConnectToServer, 0);
	pages[Multiplayer].AddButton(              true, true, 0, 200, const_cast<wchar_t*>(L"BACK"),           10, rend, GoBack,             0);
	pages[OneOrTwoPlayersChoosePage].AddButton(true, true, 0, 200, const_cast<wchar_t*>(L"1 PLAYER MODE"),  10, rend, Start1pMode,        0);
	pages[OneOrTwoPlayersChoosePage].AddButton(true, true, 0, 200, const_cast<wchar_t*>(L"2 PLAYERS MODE"), 10, rend, Start2pMode,        0);
	pages[OneOrTwoPlayersChoosePage].AddButton(true, true, 0, 200, const_cast<wchar_t*>(L"BACK"),           10, rend, GoBack,             0);
	pages[MultiplayerChoosingRoom].AddButton(  true, true, 0, 200, const_cast<wchar_t*>(L"NEW ROOM"),       10, rend, ChooseRoom,         CREATE_NEW_ROOM_ON_SERVER);
	pages[MultiplayerChoosingRoom].AddButton(  true, true, 0, 200, const_cast<wchar_t*>(L"BACK"),           10, rend, GoBack,             0);
	pages[MultiplayerWaitingInRoom].AddButton( true, true, 0, 200, const_cast<wchar_t*>(L"VOTE FOR START"), 10, rend, VoteForStart,       0);
	pages[MultiplayerWaitingInRoom].AddButton( true, true, 0, 200, const_cast<wchar_t*>(L"BACK"),           10, rend, GoBack,             0);
	pages[Options].AddButton(                  true, true, 0, 200, const_cast<wchar_t*>(L"+"),              10, rend, IncreaseGameSpeed,  0);
	pages[Options].AddButton(                  true, true, 0, 200, const_cast<wchar_t*>(L"-"),              10, rend, DecreaseGameSpeed,  0);
	pages[Options].AddButton(                  true, true, 0, 200, const_cast<wchar_t*>(L"BACK"),           10, rend, GoBack,             0);



	//AddButton(1, false, true, 0, 80, const_cast <wchar_t*>(L"MULTIPLAYER"), 10, rend, -1);
	////AddButton(1, false, true, 640, 320, const_cast <wchar_t*>(L"COMING"), 6, rend, 5);
	//AddButton(1, true, true, 0, 360, const_cast <wchar_t*>(L"NEW SERVER"), 6, rend, 18);
	////AddButton(1, false, true, 640, 400, const_cast <wchar_t*>(L"SOON"), 6, rend, 6);
	//AddButton(1, true, true, 0, 600, const_cast <wchar_t*>(L"BACK"), 8, rend, 7);

	//AddButton(2, false, true, 0, 40, const_cast <wchar_t*>(L"OPTIONS"), 10, rend, -1);

	//AddButton(2, true, true, 0, 300, const_cast <wchar_t*>(L"+"), 4, rend, 9);
	//AddButton(2, false, true, 0, 360, const_cast <wchar_t*>(L"SPEED - 1.0"), 6, rend, -1);
	//AddButton(2, true, true, 0, 420, const_cast <wchar_t*>(L"-"), 4, rend, 11);

	//AddButton(2, true, true, 0, 600, const_cast <wchar_t*>(L"BACK"), 8, rend, 7);

	//AddButton(3, true, true, 0, 5, const_cast <wchar_t*>(L"BACK TO MAIN MENU"), 4, rend, 7);
	//AddButton(3, false, true, 0, 45, const_cast <wchar_t*>(L"1ST BLUE"), 3, rend, -1);
	//AddButton(3, false, true, 0, 75, const_cast <wchar_t*>(L"2ND GREEN"), 3, rend, -1);
	//AddButton(3, false, true, 0, 105, const_cast <wchar_t*>(L"3RD YELLOW"), 3, rend, -1);
	//AddButton(3, false, true, 0, 135, const_cast <wchar_t*>(L"4TH VIOLET"), 3, rend, -1);

	//AddButton(4, true, true, 0, 5, const_cast <wchar_t*>(L"BACK TO MAIN MENU"), 4, rend, 7);
	//AddButton(4, false, true, 0, 50, const_cast <wchar_t*>(L"UUID - UNKNOWN"), 4, rend, -1);
	//AddButton(4, false, true, 0, 100, const_cast <wchar_t*>(L"URID - UNKNOWN"), 4, rend, -1);
	//AddButton(4, true, true, 0, 150, const_cast <wchar_t*>(L"VOTE FOR START"), 5, rend, 20);

	//AddButton(5, true, true, 0, 275, const_cast <wchar_t*>(L"1 PLAYER"), 4, rend, 12);
	//AddButton(5, true, true, 0, 325, const_cast <wchar_t*>(L"2 PLAYERS"), 4, rend, 13);
	//AddButton(5, true, true, 0, 600, const_cast <wchar_t*>(L"BACK"), 8, rend, 7);

	//AddButton(6, true, true, 0, 50, const_cast <wchar_t*>(L"CONNECT"), 4, rend, 19);
	//AddButton(6, false, true, 0, 215, const_cast <wchar_t*>(L"IP"), 4, rend, 7);
	//AddButton(6, false, true, 0, 250, const_cast <wchar_t*>(L""), 4, rend, 7);
	//AddButton(6, true, true, 0, 600, const_cast <wchar_t*>(L"BACK"), 8, rend, 7);

	//AddButton(7, true, true, 0, 50, const_cast <wchar_t*>(L"NEW ROOM"), 4, rend, 2147483647);
	///*AddButton(7, true, true, 0, 200, const_cast <wchar_t*>(L"IP"), 4, rend, 7);
	//AddButton(7, true, true, 0, 250, const_cast <wchar_t*>(L""), 4, rend, 7);*/
	//AddButton(7, true, true, 0, 600, const_cast <wchar_t*>(L"BACK"), 8, rend, 7);
	/*AddButton(4, false, true, 0, 45, const_cast <wchar_t*>(L"1ST BLUE"), 3, rend, -1);
	AddButton(4, false, true, 0, 75, const_cast <wchar_t*>(L"2ND GREEN"), 3, rend, -1);
	AddButton(4, false, true, 0, 105, const_cast <wchar_t*>(L"3RD YELLOW"), 3, rend, -1);
	AddButton(4, false, true, 0, 135, const_cast <wchar_t*>(L"4TH VIOLET"), 3, rend, -1);*/

	/*AddButton(2, true, true, 0, 220, const_cast <wchar_t*>(L"+"), 3, rend, 20);
	AddButton(2, false, true, 0, 245, const_cast <wchar_t*>(L"1ST SNAKE LENGHT - 5"), 3, rend, -1);
	AddButton(2, true, true, 0, 270, const_cast <wchar_t*>(L"-"), 3, rend, 21);

	AddButton(2, true, true, 0, 300, const_cast <wchar_t*>(L"+"), 3, rend, 22);
	AddButton(2, false, true, 0, 325, const_cast <wchar_t*>(L"2ND SNAKE LENGHT - 5"), 3, rend, -1);
	AddButton(2, true, true, 0, 350, const_cast <wchar_t*>(L"-"), 3, rend, 23);

	AddButton(2, true, true, 0, 370, const_cast <wchar_t*>(L"+"), 3, rend, 24);
	AddButton(2, false, true, 0, 395, const_cast <wchar_t*>(L"3RD SNAKE LENGHT - 5"), 3, rend, -1);
	AddButton(2, true, true, 0, 440, const_cast <wchar_t*>(L"-"), 3, rend, 25);

	AddButton(2, true, true, 0, 470, const_cast <wchar_t*>(L"+"), 3, rend, 26);
	AddButton(2, false, true, 0, 495, const_cast <wchar_t*>(L"4TH SNAKE LENGHT - 5"), 3, rend, -1);
	AddButton(2, true, true, 0, 520, const_cast <wchar_t*>(L"-"), 3, rend, 27);*/
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
	out.Write("HandledMouseClick");
	int buttonid = pages[CurrentState].CheckMouseCollisions(mouse);
	const char* AdditionalInfoc = NULL;
	double* AdditionalInfod = NULL;
	int AdditionalInfoi = 0;
	if (pages[CurrentState].buttons[buttonid].UBID == TryConnectToServer)
	{
		std::string ip(inputString->begin(), inputString->end());
		AdditionalInfoc = ip.c_str()/* pointer to string with server ip */;
	}
	if (pages[CurrentState].buttons[buttonid].UBID == ChooseRoom)
	{
		AdditionalInfoi = pages[CurrentState].buttons[buttonid].AdditionalInfo;
	}
	if (pages[CurrentState].buttons[buttonid].UBID == IncreaseGameSpeed || pages[CurrentState].buttons[buttonid].UBID == DecreaseGameSpeed)
	{
		AdditionalInfod = gameSpeed/* pointer to GameSpeed*/;
	}
	if (pages[CurrentState].buttons[buttonid].UBID >= 0 && pages[CurrentState].buttons[buttonid].UBID <= 13)
	{
		RegisterEvent((MenuEvents)pages[CurrentState].buttons[buttonid].UBID, AdditionalInfoi, AdditionalInfod, AdditionalInfoc);
	}
	return CurrentState;
}
void Menu::RegisterEvent(MenuEvents Event, int AdditionalInfoi, double* AdditionalInfod, const char* AdditionalInfoc)
{
	out.Write("Registered event\n");
	MenuEvent NewEvent;
	NewEvent.Event = Event;
	NewEvent.AdditionalInfoi = AdditionalInfoi;
	NewEvent.AdditionalInfod = AdditionalInfod;
	NewEvent.AdditionalInfoc = AdditionalInfoc;
	HandleEvent(NewEvent);
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
	out.Write("HandledMouseMovement\n");
	pages[CurrentState].CheckMouseCollisions(p);
}