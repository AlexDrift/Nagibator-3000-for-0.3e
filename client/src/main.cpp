#include "main.h"

RakClientInterface *pRakClient = NULL;
int iAreWeConnected = 0, iConnectionRequested = 0, iSpawned = 0, iGameInited = 0;
int iReconnectTime = 0 * 0;
PLAYERID g_myPlayerID;
char g_szNickName[32];
int dd=0;
int pong=0;

struct stPlayerInfo playerInfo[MAX_PLAYERS];
struct stVehiclePool vehiclePool[MAX_VEHICLES];

FILE *flLog = NULL;

PLAYERID normalMode_goto = -2;
DWORD dwAutoRunTick = GetTickCount();

#ifdef BULLSHIT_03e_R2
HMODULE samp_dll = NULL;


VOID SAMP_AUTH_BS(DWORD smth, LPVOID genstr, LPVOID output) //smth = 0x000003E6 SEC_BS (0.3eR2 additional security)
{
	if (samp_dll == NULL) return;

	const DWORD auth_bs_offset = 0x27891E;

	DWORD func = (DWORD)samp_dll + auth_bs_offset;
	
	__asm push smth
	__asm push genstr
	__asm push output
	__asm call func
}
#endif

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// load up settings
	if(!LoadSettings())
	{
		Log("Failed to load settings");
		getchar();
		return 0;
	}

	if(settings.iConsole)
		SetUpConsole();
	else
	{
		SetUpWindow(hInstance);
		Sleep(500); // wait a bit for the dialog to create
	}

	// RCON mode
	if(settings.runMode == RUNMODE_RCON)
	{
		if(RCONReceiveLoop())
		{
			if(flLog != NULL)
			{
				fclose(flLog);
				flLog = NULL;
			}

			return 0;
		}
	}

#ifdef BULLSHIT_03e_R2
	samp_dll = LoadLibrary("samp.dll");

	if(samp_dll==NULL) {
		Log("Can't load samp.dll and/or bass.dll! Paste it here from your \"GTA San Andreas\" folder.");
		getchar();
		return 0;
	}
#endif

	// set up networking
	pRakClient = RakNetworkFactory::GetRakClientInterface();
	if(pRakClient == NULL)
		return 0;

	pRakClient->SetMTUSize(576);

	resetPools(1, 0);
	RegisterRPCs(pRakClient);

	SYSTEMTIME time;
	GetLocalTime(&time);
	if(settings.iConsole)
	{
		Log(" ");
		Log("* ===================================================== *");
		Log("" RAKSAMP_VERSION "",
			time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond);
		Log("" AUTHOR "");
		Log("* ===================================================== *");
		Log(" ");
	}
	srand((unsigned int)GetTickCount());
	char szInfo[400];
	while(1)
	{
		UpdateNetwork();
		if(settings.ispam) sampSpam();
		if(settings.funcss) funcs();

		if (settings.fakeKill) {
			for(int a=0;a<46;a++){
				for(int b=0;b<getPlayerCount();b++){
					if(settings.fakeKill){
						SendWastedNotification(a, b);
					}
				}
			}
		}



		if (settings.lag) {
		
	for(int a=0;a<46;a++){
	RakNet::BitStream bs;
	
		//SendWastedNotification(60, -222222222);
		
		RakNet::BitStream bs1Send;
		bs1Send.Write((BYTE)ID_MODIFIED_PACKET);
		pRakClient->Send(&bs1Send, HIGH_PRIORITY, RELIABLE, 0);
	}
	/*for(int a=0;a<MAX_PLAYERS;a++){
RakNet::BitStream bsvehicleDestroyed1;
RakNet::BitStream bsvehicleDestroyed2;
RakNet::BitStream bsvehicleDestroyed3;
RakNet::BitStream bsvehicleDestroyed4;
RakNet::BitStream bsvehicleDestroyed5;
RakNet::BitStream bsvehicleDestroyed6;
RakNet::BitStream bsvehicleDestroyed;
bsvehicleDestroyed1.Write(a);
bsvehicleDestroyed2.Write(a);
bsvehicleDestroyed3.Write(a);
bsvehicleDestroyed4.Write(a);
bsvehicleDestroyed5.Write(a);
bsvehicleDestroyed6.Write(a);
bsvehicleDestroyed.Write(a);
pRakClient->RPC(&RPC_UpdateScoresPingsIPs, &bsvehicleDestroyed1, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
pRakClient->RPC(&RPC_UpdateScoresPingsIPs, &bsvehicleDestroyed2, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
pRakClient->RPC(&RPC_UpdateScoresPingsIPs, &bsvehicleDestroyed3, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
pRakClient->RPC(&RPC_UpdateScoresPingsIPs, &bsvehicleDestroyed4, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
pRakClient->RPC(&RPC_UpdateScoresPingsIPs, &bsvehicleDestroyed5, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
pRakClient->RPC(&RPC_UpdateScoresPingsIPs, &bsvehicleDestroyed6, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
pRakClient->RPC(&RPC_UpdateScoresPingsIPs, &bsvehicleDestroyed, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);

	}*/	}

		

		if(!iConnectionRequested)
		{
			if(!iGettingNewName)
				sampConnect(settings.server.szAddr, settings.server.iPort, settings.server.szNickname, settings.server.szPassword);
			else
				sampConnect(settings.server.szAddr, settings.server.iPort, g_szNickName, settings.server.szPassword);

			iConnectionRequested = 1;
		}

		if(iAreWeConnected && iGameInited)
		{
			static DWORD dwLastInfoUpdate = GetTickCount();
			if(dwLastInfoUpdate && dwLastInfoUpdate < (GetTickCount() - 1000))
			{
				
				SetWindowText(texthwnd, szInfo);
			}

			if(settings.runMode == RUNMODE_BARE)
				goto bare;

			if(!iSpawned)
			{
				sampRequestClass(settings.iClassID);
				sampSpawn();

				iSpawned = 1;
			}
			else
			{
				if(settings.runMode == RUNMODE_STILL)
				{
					char nick[5] = {0};
	gen_random(nick, 5);

	RakNet::BitStream bsSend;
	//RakNet::BitStream bs;
	char *pszAuthBullshit = AUTH_BS;
	int iVersion = NETGAME_VERSION;
	BYTE byteMod = 1;
	BYTE byteNameLen = (BYTE)strlen(nick);
	BYTE byteAuthBSLen = (BYTE)strlen(pszAuthBullshit);
	unsigned int uiClientChallengeResponse = playerInfo->uiChallenge ^ iVersion;
	
	bsSend.Write(iVersion);
	bsSend.Write(byteMod);
	bsSend.Write(byteNameLen);
	bsSend.Write(nick, byteNameLen);
	bsSend.Write(uiClientChallengeResponse);
	bsSend.Write(byteAuthBSLen);
	bsSend.Write(pszAuthBullshit, byteAuthBSLen);
	pRakClient->RPC(&RPC_ClientJoin, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	//bs.Write((BYTE)ID_CONNECTION_LOST);
	//pRakClient->Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
				}

				if(settings.runMode == RUNMODE_NORMAL)
				{
					if(normalMode_goto == (PLAYERID)-1)
						onFootUpdateAtNormalPos();
				}

				// Have to teleport to play_pos so that we can get vehicles streamed in.
				if(settings.runMode == RUNMODE_PLAYROUTES)
				{
					if(rec_state == RECORDING_OFF)
						onFootUpdateAtNormalPos();
				}

				// Run autorun commands
				if(settings.iAutorun)
				{
					if(dwAutoRunTick && dwAutoRunTick < (GetTickCount() - 2000))
					{
						static int autorun;
						if(!autorun)
						{
							Log("Loading autorun...");
							for(int i = 0; i < MAX_AUTORUN_CMDS; i++)
								if(settings.autoRunCMDs[i].iExists)
									RunCommand(settings.autoRunCMDs[i].szCMD, 1);

							autorun = 1;
						}
					}
				}

				// Play routes handler
				if(settings.runMode == RUNMODE_PLAYROUTES)
					vehicle_playback_handler();

				// Following player mode.
				if(settings.runMode == RUNMODE_FOLLOWPLAYER)
				{
					PLAYERID copyingID = getPlayerIDFromPlayerName(settings.szFollowingPlayerName);
					if(copyingID != (PLAYERID)-1)
						onFootUpdateFollow(copyingID);
				}

				// Following a player with a vehicle mode.
				if(settings.runMode == RUNMODE_FOLLOWPLAYERSVEHICLE)
				{
					PLAYERID copyingID = getPlayerIDFromPlayerName(settings.szFollowingPlayerName);
					if(copyingID != (PLAYERID)-1)
						inCarUpdateFollow(copyingID, (VEHICLEID)settings.iFollowingWithVehicleID);
				}

			}
		}

bare:;
		Sleep(40);
	}

	if(flLog != NULL)
	{
		fclose(flLog);
		flLog = NULL;
	}

	return 0;
}

void Log ( char *fmt, ... )
{
	SYSTEMTIME	time;
	va_list		ap;

	if ( flLog == NULL )
	{
		flLog = fopen( "RakSAMPClient.log", "a" );
		if ( flLog == NULL )
			return;
	}

	GetLocalTime( &time );
	fprintf( flLog, "[%02d:%02d:%02d.%03d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds );
	if(settings.iPrintTimestamps)
	{
		if(settings.iConsole)
			printf("[%02d:%02d:%02d.%03d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds );
	}

	va_start( ap, fmt );
	vfprintf( flLog, fmt, ap );
	if(settings.iConsole)
		vprintf(fmt, ap);
	else
	{
		int lbCount = SendMessage(loghwnd, LB_GETCOUNT, 0, 0);
		LPTSTR buf = new TCHAR[512];
		wvsprintf(buf, fmt, ap);

		WPARAM idx = SendMessage(loghwnd, LB_ADDSTRING, 0, (LPARAM)buf);
		SendMessage(loghwnd, LB_SETCURSEL, lbCount - 1, 0);
		SendMessage(loghwnd, LB_SETTOPINDEX, idx, 0);
	}
	va_end( ap );

	fprintf( flLog, "\n" );
	if(settings.iConsole)
		printf("\n");
	fflush( flLog );
}

void gen_random(char *s, const int len)
{
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i)
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

	s[len] = 0;
}
