class cl_conman
{
public:
    int li_count;
    int li_number;
    int li_maxlogon_tries = 50; // max tries to connect to WLAN -> reset if reached
    int li_logon_tries;         // current no. of tries to connect to WLAN
    String lst_SSID;
    String lst_password;
    String lst_DHCPhostname; // String for input, NVM
    String lst_myip;
    IPAddress l_ip;
    WiFiServer lo_pub_server();
    Preferences lo_prefs;

    short m_scan_wlan()
    {
        li_number = WiFi.scanNetworks();
        lo_prefs.begin("ac_data", true); // read only
        for (int li_count = 0; li_count < li_number; ++li_count)
        {
            lst_SSID = WiFi.SSID(li_count);
            Serial.print(WiFi.SSID(li_count));

            lst_password = lo_prefs.getString(lst_SSID.c_str(), "?");

            if (lst_password != "?")
            {
                return 1;
            }
        }
        // lst_DHCPhostname =

        lo_prefs.end();
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
        lo_prefs.begin("ac_data", false);
        lo_prefs.putString("Wurmnetz", "");
        lo_prefs.end();
        lo_prefs.begin("ac_data", false);
        lo_prefs.putString("WLAN-318841", "");
        lo_prefs.end();
*/
    }
    void m_connect()
    {
        WiFi.mode(WIFI_STA);
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
                Serial.println("R E S T A R T ");
                ESP.restart();
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
        }
    }
};