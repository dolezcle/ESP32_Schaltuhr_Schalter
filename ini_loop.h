int ini_loop(String ip_ssid, String ip_password, String ip_host, String ip_myip, short ip_fast_start)
{
	//ip_fast_start = 1;                                            //Test
	Serial.println("<------------------------------->");
	// delay(5000);
	Serial.print("Start ini_loop ");
	Serial.print(ip_ssid);
	Serial.println(ip_password);
	Serial.print("ip_fast_start: ");
	Serial.println(ip_fast_start);
	delay(500);
	uint64_t li_etime_ini;
	uint64_t li_delay_ini;
	//	li_delay_ini = 600000 * 2;									//2 sec max
	if (ip_fast_start == 1) {
		li_delay_ini = 1;
	}
	else {
	li_delay_ini = 600000 * 360; // 3 Min max
	}
	if (gx_debug != "") {
	li_delay_ini = 600000 * 2; // 2 sec max wennn debug
	}
	IPAddress iniIP = WiFi.softAPIP();
	int l_client_started = 0;
	String l_inputval = "INIT";										//empty: complete reset, l_inputval = "OK": data entered
	int i_ret = 4;
	WiFiClient esp_client;
	char l_datar[100];
	Serial.println("Start ini_loop");
	Serial.println("Start ini_loop 2");
	gi_ttime = esp_timer_get_time();
	li_etime_ini = gi_ttime + li_delay_ini;
	while (l_inputval != gc_on)
	{
		gi_ttime = esp_timer_get_time();
		if (round((li_etime_ini - gi_ttime) / 1000000) > 0)
		{
			Serial.print("Restlaufzeit in sec: ");
			Serial.println(round((li_etime_ini - gi_ttime) / 1000000));
			delay(500);
		}
		if (gi_ttime > li_etime_ini)
		{ // exit without change of NVM when setup time is over
			esp_client.stop();
			Serial.print("Ende ini_loop ohne Eingabe: ");
			ip_ssid.toCharArray(p_ssid, ip_ssid.length() + 1);
			ip_password.toCharArray(p_password, ip_password.length() + 1);

			gs_DHCPhostname = ip_host;
			return (0); // return latest after li_delay_ini
			break;
		}
		Serial.print("AP iniIP address: ");
		Serial.println(iniIP);
		delay(500);
		esp_client = esp_server.available();
		Serial.print("esp_client available -->");
		Serial.println(esp_client);
		String lp_ssid;
		lp_ssid = ip_ssid;
		// lp_ssid.replace(' ', '+');
		lp_ssid = "'" + lp_ssid + "'";
		if (esp_client >= 0)
		{ // new client connects
			Serial.println("im ini_loop <------------------");
			delay(500);
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
				for (int ln_count = 0; ln_count < 6; ln_count++) {
				esp_client.println(ga_chipid[ln_count]);
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
				esp_client.println(ip_myip);
				esp_client.println(
					"></label><br><label for='host'>Host:<br><input style='background-color:#FFFFE0' type='text' name='host' value=");
				esp_client.println(ip_host);
				esp_client.println(
					"></label><br><input type='submit' value='mit Netzwerk verbinden'></form></p>");
				esp_client.println(
					"<p><form>Reset <select name='reset'><option></option><option>Reset</option></select><br><br><input type='submit' value='Reset WLAN Anmeldedaten'></form></p>");

				if (esp_client.available())
				{ // client on?
					gx_header = esp_client.readStringUntil('\n');
					Serial.print("gx_header when client available    : ");
					Serial.println(gx_header);
					Serial.print("gx_headerlength: ");
					Serial.println(gx_header.length());
					int li_offset;
					String l_SSID = get_substring(gx_header, "?SSID", "&password", 1);
					Serial.print("l_SSID:");
					Serial.println(l_SSID);

					String l_PWD = get_substring(gx_header, "password=", "&my", 0);
					Serial.print("l_PWD:");
					Serial.println(l_PWD);
					if (l_PWD == "")
					{
						l_PWD = ip_password;
					}; // PWD not enred -> take from NVM
					String l_myip = get_substring(gx_header, "myip=", "&host", 0);
					Serial.print("l_myip:");
					Serial.println(l_myip);
					if (l_myip == "") // if empty add -1 to the length
					{
						li_offset = -1;
					}
					String l_host = get_substring(gx_header, "host=", " HTTP", 0);
					Serial.print("l_host:");
					Serial.println(l_host);
					Serial.print("SSID vor Replace: ");
					Serial.println(l_SSID);
					l_SSID.replace("+", " ");	// replace '+' by space (GET converts space to +?)
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
						l_inputval = "";
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
							gs_myip = l_myip;
						}
						l_inputval = "OK";
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
						strcpy(p_ssid, "");
						strcpy(p_password, "");
					}

					esp_client.println("</body>"); // Ende der Seite
					gx_header.toCharArray(l_datar, 50);
					Serial.printf("%s", l_datar);
					if (l_inputval == "OK")
					{ // wenn "AN" dann verbinde an normales Netzwerk
						Serial.println("Im iniloop vor ini-break");						
						//delay(1000);
						esp_client.stop();
						i_ret = 0;
						return (i_ret); // ini_loop beenden nach Eingabe
						break;
					}
				} // while esp_client.available()
				break;
			} // while (esp_client.connected())
		}	  // if (esp_client)
	}		  // while (l_inputval != "An")
} // ini_loop
