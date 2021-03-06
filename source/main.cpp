/*----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#------  This File is Part Of : ----------------------------------------------------------------------------------------#
#------- _  -------------------  ______   _   --------------------------------------------------------------------------#
#------ | | ------------------- (_____ \ | |  --------------------------------------------------------------------------#
#------ | | ---  _   _   ____    _____) )| |  ____  _   _   ____   ____   ----------------------------------------------#
#------ | | --- | | | | / _  |  |  ____/ | | / _  || | | | / _  ) / ___)  ----------------------------------------------#
#------ | |_____| |_| |( ( | |  | |      | |( ( | || |_| |( (/ / | |  --------------------------------------------------#
#------ |_______)\____| \_||_|  |_|      |_| \_||_| \__  | \____)|_|  --------------------------------------------------#
#------------------------------------------------- (____/  -------------------------------------------------------------#
#------------------------   ______   _   -------------------------------------------------------------------------------#
#------------------------  (_____ \ | |  -------------------------------------------------------------------------------#
#------------------------   _____) )| | _   _   ___   ------------------------------------------------------------------#
#------------------------  |  ____/ | || | | | /___)  ------------------------------------------------------------------#
#------------------------  | |      | || |_| ||___ |  ------------------------------------------------------------------#
#------------------------  |_|      |_| \____|(___/   ------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Licensed under the GPL License --------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Copyright (c) Nanni <lpp.nanni@gmail.com> ---------------------------------------------------------------------------#
#- Copyright (c) Rinnegatamante <rinnegatamante@gmail.com> -------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Credits : -----------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Smealum for ctrulib -------------------------------------------------------------------------------------------------#
#- StapleButter for debug font -----------------------------------------------------------------------------------------#
#- Lode Vandevenne for lodepng -----------------------------------------------------------------------------------------#
#- Sean Barrett for stb_truetype ---------------------------------------------------------------------------------------#
#- Special thanks to Aurelio for testing, bug-fixing and various help with codes and implementations -------------------#
#-----------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include "include/luaplayer.h"
#include "include/luaGraphics.h"
#include "index.cpp"

const char *errMsg;
unsigned char *buffer;
char cur_dir[256];
char start_dir[256];
int main(int argc, char **argv)
{
	srvInit();	
	aptInit();
	gfxInit();
	acInit();
	initCfgu();
	ptmInit();
	hidInit(NULL);
	irrstInit(NULL);
	fsInit();
	hbInit();
	Handle fileHandle;
	u64 size;
	u32 bytesRead;
	int restore;
	
	// Set main script
	char path[256];
	if (argc > 0){	
		int latest_slash = 0;
		int i=5;
		while (argv[0][i]  != '\0'){
		if (argv[0][i] == '/'){
		latest_slash = i;
		}
		i++;
		}
		strcpy(path,&argv[0][5]);
		path[latest_slash-5] = 0;
		strcpy(start_dir,path);
		strcpy(cur_dir,path); // Set current dir
		strcat(path,"/index.lua");
	}else{
		strcpy(start_dir,"/");
		strcpy(cur_dir,"/"); // Set current dir for GW Mode
		strcpy(path,"/index.lua");
	}
		
	while(aptMainLoop())
	{
		restore=0;		
		char error[256];
		
		errMsg = runScript((char*)index_lua, true);
		
		if (errMsg != NULL);
                {
				// Fake error to force interpreter shutdown
				if (strstr(errMsg, "lpp_exit_04")){
					break;
				}
				
				strcpy(error,"Error: ");
				strcat(error,errMsg);
				strcat(error,"\n\nPress A to restart\nPress B to exit");
				}

						while (restore==0){
							gspWaitForVBlank();
							RefreshScreen();
							ClearScreen(0);
							ClearScreen(1);
							DebugOutput(error);
							hidScanInput();
							if(hidKeysDown() & KEY_A){
								strcpy(cur_dir,start_dir);
								restore=1;
							}else if(hidKeysDown() & KEY_B){
								restore=2;
							}
							gfxFlushBuffers();
							gfxSwapBuffers();
						}
						if (restore==2){
							break;
						}
	}
	
	fsExit();
	irrstExit();
	hidExit();
	ptmExit();
	hbExit();
	acExit();
	exitCfgu();
	gfxExit();
	aptExit();
	srvExit();

	return 0;
}
