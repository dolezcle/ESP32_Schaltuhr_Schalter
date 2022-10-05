/*********

 Webserver mit Eingabeformular,
 Initialisierung ueber lokales Netz ESP_ssid / ESP_pwd mit IP 192.168.4.1
 Die Zugangsdaten werden in das NVM geschrieben und koennen auf der Hauptseite zurueckgesetzt werden
 Adresse: http://<gs_DHCPhostname>./
 Die IP-Adresse wird im setup eingegeben, Port 80
 Der Schaltpin fuer das Relais ist gpio
 Credentials für das operative WLAN werden in NVE Namespace "ac_data" gespeichert
 Die Werte werden in NVE namespace "switchtimes" gespeichert


 *********/
#include <Arduino.h>
#include <WiFi.h>
#include <stdbool.h>
//#include <cstdint>
#include <string>
// #include <iostream>
#include <esp_timer.h>
#include <esp_wifi.h>
#include <time.h>
#include <Preferences.h>
#include "cl_switch_man.h"
#include "g_vars.h"
#include "gen_funcs.h"
#include "ini_loop.h"
#include "spec_funcs.h"


//#include <ESPmDNS.h>                            //<---funktioniert nicht mit Android

WiFiServer pub_server(80); // Betrieb
// IPAddress pub_staticIP(192, 168, 179, 20);        //LAN
// IPAddress pub_gateway(192, 168, 179, 1);

void setup()
{
	pinMode(gpio, OUTPUT);
	Serial.begin(115200);
	esp_efuse_read_mac(ga_chipid);
	// nvs_flash_erase(); // erase the NVS partition and...
	// nvs_flash_init();  // initialize the NVS partition.
	digitalWrite(gpio, HIGH);
	g_prefs.begin("ac_data", false);
	String ls_ssid_ini = g_prefs.getString("SSID", "");
	String ls_password_ini = g_prefs.getString("PWD", ""); //<-----------------------------NVM initial GET zur Uebergabe an ini_loop
	String ls_host = g_prefs.getString("gs_DHCPhostname", "");
	String ls_myip = g_prefs.getString("gs_myip", "");
	short li_fast_start = g_prefs.getShort("faststart", 0);
	g_prefs.end();
	Serial.print("g_prefs-SSID im setup ls_ssid_ini  vor ini_loop: ");
	Serial.println(ls_ssid_ini);
	Serial.print("g_prefs-PWD im setup ls_password_ini  vor ini_loop: ");
	Serial.println(ls_password_ini);
	Serial.print("g_prefs-host im setup ls_host  vor ini_loop: ");
	Serial.println(ls_host);
	Serial.print("g_prefs-myip im setup ls_myip  vor ini_loop: ");
	Serial.println(ls_myip);
	if (gx_debug == "X")
	{
		delay(500);
	}

	WiFi.persistent(false);
	Serial.print("Setting AP (Access Point): ");
	WiFi.softAP(ESP_ssid, ESP_pwd);
	esp_server.begin();
	Serial.print("esp_server im setup: ");
	Serial.println(esp_server);
	if (gx_debug == "X")
	{
		delay(500);
	}

	int li_rc = ini_loop(ls_ssid_ini, ls_password_ini, ls_host, ls_myip, li_fast_start); //<-----------------------------ini_loop

	esp_server.stop();
	if (li_rc == 0)
	{ //<-----------------------------ok aus ini_loop
		g_prefs.begin("ac_data", false);
		g_prefs.putString("SSID", p_ssid); //<-----------------------------NVM put aus ini_loop
		// g_prefs.putString("PWD", p_password);
		g_prefs.putString("PWD", p_password);

		g_prefs.putString("gs_DHCPhostname", gs_DHCPhostname);
		g_prefs.putString("gs_myip", gs_myip);
		g_prefs.putShort("faststart", 0);                         			   //set normal start mode
		Serial.print("g_prefs G E S E T Z T nach ini_loop. p_ssid: ");
		Serial.print(p_ssid);
		Serial.print(" p_password: ");
		Serial.println(p_password);
		Serial.print(" gs_DHCPhostname: ");
		Serial.println(gs_DHCPhostname);
		delay(1000);
	}

	String ls_ssid = g_prefs.getString("SSID", ""); //<-----------------------------NVM finaler get
	String ls_password = g_prefs.getString("PWD", "");
	gs_DHCPhostname = g_prefs.getString("gs_DHCPhostname", "");
	gs_myip = g_prefs.getString("gs_myip", "");
	Serial.print("g_prefs Neu Gelesen, ls_ssid: ");
	Serial.println(ls_ssid);
	Serial.print("g_prefs Neu Gelesen, ls_password: ");
	Serial.println(ls_password);
	Serial.print("g_prefs Neu Gelesen, gs_DHCPhostname: ");
	Serial.println(gs_DHCPhostname);
	WiFi.disconnect(); //<---------------------------Netz ruecksetzen?
	WiFi.mode(WIFI_OFF);
	Serial.println("WIFI_OFF");
	delay(1000);

	IPAddress ls_ip;
	IPAddress gateway(192, 168, 178, 1); // Wurmnetz
	IPAddress subnet(255, 255, 255, 0);
	int li_indexfr;
	int li_indextt = gs_myip.indexOf(0, '.');
	Serial.print("gs_myip: ");
	Serial.println(gs_myip);
	ls_ip = str2ipv4(gs_myip, '.');
	if (ls_ip[0] == 0 || ls_ip[1] == 0 || ls_ip[2] == 0 || ls_ip[3] == 0)
	{
		ls_ip = INADDR_NONE;
	}
	WiFi.config(ls_ip, gateway, subnet); //<-----------------------------Config
	// WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
	//	MDNS.begin(gs_DHCPhostname.c_str());
	WiFi.setHostname(gs_DHCPhostname.c_str());
	WiFi.mode(WIFI_STA);
	WiFi.begin(ls_ssid.c_str(), ls_password.c_str()); //<-----------------------------WiFi START operativ mit gerade gelesenen Preferences
	gs_ssid = ls_ssid;
	gs_password = ls_password;
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.println("Mit WiFi verbinden im setup.. ");
		Serial.println(ls_ssid.c_str());
		Serial.println(ls_password.c_str());
		Serial.print("WiFi.status() ");
		switch (WiFi.status())
		{
		case 1:
			Serial.println("1 - No SSID (WL_NO_SSID_AVAIL)");
			break;
		case 3:
			Serial.println("3 - Connected (WL_CONNECTED)");
			break;
		case 4:
			Serial.println("4 - Connect failed (WL_CONNECT_FAILED)");
			break;
		case 5:
			Serial.println("5 - Connection lost (WL_CONNECTION_LOST)");
			break;
		case 6:
			Serial.println("6 - Disconnected (WL_DISCONNECTED)");
			break;
		default:
			Serial.print("Other");
			Serial.println(WiFi.status());
			break;
		}
		// Serial.println(WiFi.status());
		if (gi_maxlogon_tries < gi_logon_tries)
		{
			Serial.println("R E S T A R T ");
			ESP.restart();
		}
		gi_logon_tries++;
	}
	if (WiFi.status() == WL_CONNECTED)
	{
		if (gx_debug == "X")
		{
			delay(500);
		}
		Serial.print("vor get_ntptime im setup ");
		get_ntptime();
		gs_myip = ipv42string(WiFi.localIP());
		g_prefs.putString("gs_myip", gs_myip);
		g_prefs.end();
		readswitchtimes();
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void loop()
{
	String l_inputval; //(Raw) Value of Range 0 or 1 (Off, On)
	String lstr_autooff = "";
	char la_autooff[10] = "X";
	
	if (gx_debug == "X")
	{
		Serial.println("Im Hauptloop");
		delay(100);
	}
	
	// gi_ttime = esp_timer_get_time();
	getLocalTime(&g_loc_time);
	setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1); // scheint im setup() nicht zu reichen
	gi_ettime = mktime(&g_loc_time);				 // lokale Zeit als int aus der ntp-Zeit
	go_switch_man.initialize();
	if (WiFi.status() == WL_CONNECTED)
	{
		if (!pub_server)
		{
			pub_server.begin();
		}
	}
	if (WiFi.status() != WL_CONNECTED)
	{
		WiFi.disconnect();
		//		WiFi.reconnect();
		WiFi.begin(gs_ssid.c_str(), gs_password.c_str());
		while (WiFi.status() != WL_CONNECTED)
		{
			Serial.print("Wiederverbinden... ");
			Serial.println(gs_ssid.c_str());
			Serial.println(gs_password.c_str());
			Serial.print("WiFi.status() ");
			Serial.println(WiFi.status());
			if (gx_debug == "X")
			{
				delay(500);
			}
			if (gi_maxlogon_tries < gi_logon_tries)
			{
				g_prefs.begin("ac_data", false);
				g_prefs.putShort("faststart", 1); // set faststart to speed up
				g_prefs.end();
				Serial.println("R E S T A R T ");
				ESP.restart();
			}
			gi_logon_tries++;
		}
	}
	//	Serial.print("gi_ettime: ");
	//	Serial.print(gi_ettime);
	if (gi_etime < 0)
	{
		gi_etime = 0;
	}
	//	Serial.print("    gi_etime vor pub_client: ");
	//	Serial.println(gi_etime);
	//	delay(1000);
	if (gi_ettime > gi_etime && gi_etime > 0 && gpio_state == 1)
	{
		Serial.println("A U S G E S C H A L T E T");
		digitalWrite(gpio, HIGH);
		gpio_state = 0;
		gi_etime = 0; // reset end time
	}

	WiFiClient pub_client; // initialisierung
	pub_client = pub_server.available();
	if (gi_ini < 3)
	{ // 3x IP ausgeben
		Serial.print("IP: ");
		Serial.println(WiFi.localIP());
		Serial.print("gs_DHCPhostname: ");
		Serial.println(gs_DHCPhostname);
		Serial.print("pub_client");
		Serial.println(pub_client);
		gi_ini++;
		Serial.print("gi_ini: ");
		Serial.println(gi_ini);
		Serial.print("im Hauptloop: ");
		Serial.println(&g_loc_time, "%A, %B %d %Y %H:%M:%S");
	}

	gi_autooff = 0; // darf bei refresh nicht geloescht werden???
	//	gi_etime = 0;
	gx_header = "";
	while (pub_client.connected())
	{
		Serial.print("im pub_client.connected ");
		// delay(1000);
		//		Serial.println(WiFi.localIP());
		gx_header = pub_client.readStringUntil('\n');
		Serial.print("gx_header: ");
		Serial.println(gx_header);
		if (gx_debug == "X")
		{
			delay(500);
		}
		if (gx_header.indexOf("GET /?on") > -1)		{
			// Serial.println(gx_header.indexOf("GET / HTTP/1.1"));
			// delay(1000);

			gx_on[1] = get_substring(gx_header, "on1=", "&off1=", 0);
			gx_off[1] = get_substring(gx_header, "off1=", "&on2=", 0);
			gx_on[2] = get_substring(gx_header, "on2=", "&off2=", 0);
			gx_off[2] = get_substring(gx_header, "off2=", "&on3=", 0);
			gx_on[3] = get_substring(gx_header, "on3=", "&off3=", 0);
			gx_off[3] = get_substring(gx_header, "off3=", "&on4=", 0);
			gx_on[4] = get_substring(gx_header, "on4=", "&off4=", 0);
			gx_off[4] = get_substring(gx_header, "off4=", "&on5=", 0);
			gx_on[5] = get_substring(gx_header, "on5=", "&off5=", 0);
			gx_off[5] = get_substring(gx_header, "off5=", "&on6=", 0);
			gx_on[6] = get_substring(gx_header, "on6=", "&off6=", 0);
			gx_off[6] = get_substring(gx_header, "off6=", "&on0=", 0);
			gx_on[0] = get_substring(gx_header, "on0=", "&off0=", 0);
			gx_off[0] = get_substring(gx_header, "off0=", " HTTP", 0);
		}
		for (int ln_count = 0; ln_count < 7; ln_count++)
		{
			gx_on[ln_count].replace("%3A", gc_col);
			gx_off[ln_count].replace("%3A", gc_col);
		}

		saveswitchtimes();
		if (gx_header.indexOf("GET /?switch") > -1){
			go_switch_man.set_offtime(get_substring(gx_header, "switch=", "&", 0), get_substring(gx_header, "ttl=", " HTTP", 0));
			if (gx_debug == "X")
		{Serial.print("Direktes Schalten: ");
		
		Serial.print("go_switch_man.li_autooff: ");
		Serial.println(go_switch_man.li_autooff);
		delay(1000);
		}
		}

		if (gx_debug == "X")
		{
			Serial.println("Werte nach Lesen des GET <-------------------");
			for (int ln_count = 0; ln_count < 7; ln_count++)
			{
			Serial.print("gx_off[xxx]: ");			
			Serial.println(gx_off[ln_count]);			
			}
		}
		mainhtml1(pub_client, go_switch_man);

		pub_client.stop();
	}
	flipswitch();
}
