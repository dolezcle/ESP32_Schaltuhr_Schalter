const char *gx_debug = "X";  //S-Set Preferences for WLAN
String gx_header;

long int gi_ttime;          // Laufzeit seit boot absolut (ini_loop)
long int gi_ettime;         // Laufzeit seit boot in lokaler Zeit in sec gemaess ntp
long int gi_etime;          // autooff als int in sec in lokaler Zeit gemaess ntp
long int gi_nooff = 9999999;//no end time
long int gi_stime;          // start time in int
long int gi_autooff;        // autooff nach dem Lesen in ms
struct tm g_loc_time;       // local time as tm
struct tm g_end_time;       // switch-off time as tm
struct tm g_start_time;     // switch-on time as tm
uint8_t ga_chipid[6];       // chip id
int gi_ini;                 // help: ini in mainloop
int gi_maxlogon_tries = 50; // max tries to connect to WLAN -> reset if reached
int gi_logon_tries;         // current no. of tries to connect to WLAN
int gi_newntp;              // get new ntp time - seconds after last try
Preferences g_prefs;
const char *ESP_ssid = "MeinschlauerSchalter";
const char *ESP_pwd = "15061959";
WiFiServer esp_server(80); // setup (standalone AP)
//WiFiServer esp_server;
char p_ssid[100];          // array needed for WLAN credentials
char p_password[50];       // array needed for WLAN credentials

String gs_ssid;         // String for input, NVM
String gs_password;     // String for input, NVM
String gs_DHCPhostname; // String for input, NVM
String gs_myip;         // String for input, NVM
String gc_off = "Aus";
String gc_on = "An";
String gc_col = ":";

int gi_state; // 99-undefined, 0-off, 1-on
int gpio = 26;
int gpio_state; //obsolete?
String gs_pub_staticIP = "192.168.4.1";
// String gs_gw_host = "ESP32Schalter";

IPAddress gx_pub_staticIP;
int gi_ip0;
int gi_ip1;
int gi_ip2;
int gi_ip3;
String gx_on[7];
String gx_off[7];

cl_switch_man go_switch_man;


String gx_imgon = "<img src='data:image/jpeg;base64,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' width='50' height='50'>";