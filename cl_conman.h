
class cl_conman
{
public:
    const char *la_debug = "X";
    
    int li_count;
    int li_number;
    int li_maxlogon_tries = 50; // max tries to connect to WLAN -> reset if reached
    int li_logon_tries;         // current no. of tries to connect to WLAN
    String lst_SSID;
    String lst_password;
    String lst_DHCPhostname; // String for input, NVM
    String lst_myip;
    String lst_host;
    String lst_header;
    String lst_PWD;
    String lsc_off = "Aus";
    String lsc_on = "An";
    uint8_t la_chipid[6];       // chip id
    IPAddress l_ip;
//    WiFiServer lo_esp_server(); // setup (standalone AP)
    Preferences lo_prefs;
    const char *Pref_con = "ac_data";

    short m_scan_wlan()                     //returns 1 if no password
    {
        li_number = WiFi.scanNetworks();
        lo_prefs.begin(Pref_con, true); // read only
        for (int li_count = 0; li_count < li_number; ++li_count)
        {
            lst_SSID = WiFi.SSID(li_count);
            Serial.print("WiFi.SSID found: ");
            Serial.println(WiFi.SSID(li_count));

            lst_password = lo_prefs.getString(lst_SSID.c_str(), "?");

            if (lst_password != "?")
            {
                lo_prefs.end();
                return 0;
            }
        }
        // lst_DHCPhostname =

        lo_prefs.end();
        Serial.println("exit m_scan_wlan ");
        return 1;
            
    }

    void m_save_creds()
    {
        lo_prefs.begin("ac_data", false);
        lo_prefs.putString(lst_SSID.c_str(), lst_password);     
        lo_prefs.end();
    }

    void m_save_test_creds()
    {
        /*
        lo_prefs.begin(Pref_con, false);
        lo_prefs.putString("Wurmnetz", "");
        lo_prefs.end();
        lo_prefs.begin(Pref_con, false);
        lo_prefs.putString("WLAN-318841", "");
        lo_prefs.end();
*/
    }

    short m_get_conn_data()
    {

    l_ip = WiFi.localIP();


    }
    int m_connect()                   //returns 4 = not connected, 0 = connected
    {
        WiFi.mode(WIFI_STA);
         Serial.println("vor WIFI Setup...");
        WiFi.begin(lst_SSID.c_str(), lst_password.c_str());
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.println("Mit WiFi verbinden im setup.. ");
            Serial.println(lst_SSID.c_str());
            Serial.println(lst_password.c_str());
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
            if (li_maxlogon_tries < li_logon_tries)
            {
                //                Serial.println("R E S T A R T ");
                //                ESP.restart();
                return 4;
            }
            li_logon_tries++;
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            /*
                    if (gx_debug == "X")
                    {
                        delay(500);
                    }

                    gs_myip = ipv42string(WiFi.localIP());
                    lo_prefs.putString("gs_myip", gs_myip);
            */
           return 0;
        }
    }

    void m_dialog_credentials()
    {
        
    WiFiServer lo_esp_server;
        WiFiClient esp_client;

        String llx_inputval = "INIT";              // empty: complete reset, llx_inputval = "OK": data entered
        uint64_t lli_delay_ini = 600000 * 360;     // 3 Min max
        long lli_ttime = esp_timer_get_time();     // Start
        uint64_t lli_etime_ini = lli_ttime + lli_delay_ini; // max time to enter creds before restart
        char llx_datar[100];
        IPAddress iniIP = WiFi.softAPIP();
        int l_client_started = 0;
        lo_esp_server.begin(80);
        while (llx_inputval != lsc_on)
        {
            esp_client = lo_esp_server.available();
            Serial.print("esp_client available -->");
            Serial.println(esp_client);

            String lp_ssid = lst_SSID;
            lp_ssid = "'" + lp_ssid + "'";

            while (esp_client.connected())
            { // loop while the client's connected
                // send HTML form for SSID & password & fixed ip & hostname
                esp_client.println("HTTP/1.1 200 OK");
                esp_client.println("Content-type:text/html");
                esp_client.println("Connection: close");
                esp_client.println();
                esp_client.println("<!DOCTYPE html><html>");
                esp_client.println(
                    "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" >");
                esp_client.println(
                    "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}");
                esp_client.println(
                    ".input { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                esp_client.println(
                    "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                esp_client.println(
                    ".button2 {background-color: #555555;}</style></head>");
                // Web Page Header end, Body starts
                esp_client.println("<body>");
                esp_client.println("Chip ID: ");
                for (int ln_count = 0; ln_count < 6; ln_count++)
                {
                    esp_client.println(la_chipid[ln_count]);
                }
                esp_client.println("<h1>WLAN W&auml;hlen</h1>");
                //        esp_client.println("<p><form><label for='SSID'>SSID:<input type='text' name='SSID' value='Wurmnetz'></label><br><label for='password'>Password:<input type='text' name='password' value='0005021970'></label><br><input type='submit' value='mit Wurmnetz verbinden'></form></p>");
                esp_client.println(
                    "<p><form><label for='SSID'>SSID:<br><input style='background-color:#FFFFE0' type='text' name='SSID' value=");
                esp_client.println(lp_ssid);
                esp_client.println(
                    "></label><br><label for='password'>Password:<br><input style='background-color:#FFFFE0' type='password' name='password' value=");
                // esp_client.println(ip_password);                                         //password is not prefilled anymore
                esp_client.println(
                    "></label><br><label for='myip'>IP der letzten Verbindung:<br><input style='background-color:#FFFFE0' type='text' name='myip' value=");
                esp_client.println(lst_myip);
                esp_client.println(
                    "></label><br><label for='host'>Host:<br><input style='background-color:#FFFFE0' type='text' name='host' value=");
                esp_client.println(lst_host);
                esp_client.println(
                    "></label><br><input type='submit' value='mit Netzwerk verbinden'></form></p>");
                esp_client.println(
                    "<p><form>Reset <select name='reset'><option></option><option>Reset</option></select><br><br><input type='submit' value='Reset WLAN Anmeldedaten'></form></p>");

                if (esp_client.available())
                { // client on?
                    lst_header = esp_client.readStringUntil('\n');
                    Serial.print("lst_header when client available    : ");
                    Serial.println(lst_header);
                    Serial.print("lst_headerlength: ");
                    Serial.println(lst_header.length());
                    int li_offset;
                    String l_SSID = get_substring(lst_header, "?SSID", "&password", 1);
                    Serial.print("l_SSID:");
                    Serial.println(l_SSID);

                    String l_PWD = get_substring(lst_header, "password=", "&my", 0);
                    Serial.print("l_PWD:");
                    Serial.println(l_PWD);
                    if (l_PWD == "")
                    {
                        l_PWD = lst_PWD;
                    }; // PWD not enred -> take from NVM
                    String l_myip = get_substring(lst_header, "myip=", "&host", 0);
                    Serial.print("l_myip:");
                    Serial.println(l_myip);
                    if (l_myip == "") // if empty add -1 to the length
                    {
                        li_offset = -1;
                    }
                    String l_host = get_substring(lst_header, "host=", " HTTP", 0);
                    Serial.print("l_host:");
                    Serial.println(l_host);
                    Serial.print("SSID vor Replace: ");
                    Serial.println(l_SSID);
                    l_SSID.replace("+", " ");   // replace '+' by space (GET converts space to +?)
                    l_SSID.replace("%21", "!"); // replace space by !
                    Serial.print("SSID nach Replace: ");
                    Serial.println(l_SSID);
                    Serial.print("  PWD: ");
                    Serial.println(l_PWD);
                    Serial.print("l_host: ");
                    Serial.println(l_host);
                    if (l_SSID.indexOf("HTTP") > 0)
                    {
                        l_SSID = "";
                    }
                    if (l_PWD.indexOf("GET") > 0)
                    {
                        l_PWD = "";
                    }
                    if (l_SSID.indexOf("Reset") > 0)
                    {
                        Serial.print("Im iniloop R E S E T nach l_SSID: <------------------");
                        Serial.print(l_SSID);
                        delay(500);
                        l_SSID = "";
                        l_PWD = "";
                        g_prefs.clear(); //-------kompletter reset des NVM
                        llx_inputval = "";
                        Serial.println("prefs C L E A R E D");
                    }
                    if (l_SSID != "" && l_PWD != "")
                    {
                        if (l_PWD > "")
                        {
                            l_PWD.toCharArray(p_password, l_PWD.length() + 1);
                        }
                        l_SSID.toCharArray(p_ssid, l_SSID.length() + 1);
                        Serial.print("p_password in ini_loop: ");
                        Serial.println(p_password);
                        Serial.print("  p_ssid: ");
                        Serial.println(p_ssid);
                        gs_DHCPhostname = l_host;
                        if (l_myip > " ")
                        {
                            lst_myip = l_myip;
                        }
                        llx_inputval = "OK";
                        Serial.print("p_ssid erfolgreich: ");
                        Serial.print(p_ssid);
                        Serial.print("  p_password erfolgreich: ");
                        Serial.println(p_password);
                        // Serial.print("  p_myip erfolgreich: ");
                        // Serial.println(p_myip);
                        Serial.print("  gs_DHCPhostname: ");
                        Serial.println(gs_DHCPhostname);
                    }
                    else
                    {
//                        strcpy(lst_SSID, "");
//                        strcpy(lst_PWD, "");
                    lst_SSID = "";
                    lst_PWD = "";
                    }

                    esp_client.println("</body>"); // Ende der Seite
//                    lst_header.toCharArray(l_datar, 50);
//                    Serial.printf("%s", l_datar);
                    if (llx_inputval == "OK")
                    { // wenn "AN" dann verbinde an normales Netzwerk
                        Serial.println("Im iniloop vor ini-break");
                        // delay(1000);
                        esp_client.stop();
//                        i_ret = 0;
//                        return (i_ret); // ini_loop beenden nach Eingabe
                        break;
                    }
                } // while esp_client.available()
                break;
            } // while (esp_client.connected())
        }     // while (llx_inputval != "An")
    }
};