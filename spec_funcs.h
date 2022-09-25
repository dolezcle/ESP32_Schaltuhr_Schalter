void mainhtml1(WiFiClient i_client)
{
	if (gx_debug == "X")
	{
		Serial.println("in mainhtml1");
		for (int ln_count = 0; ln_count < 7; ln_count++)
		{
			Serial.print("gx_off[0..6]");
			Serial.print(" ");
			Serial.println(gx_off[ln_count]);
		}
		delay(1000);
	}
	i_client.println("HTTP/1.1 200 OK");
	i_client.println("Content-type:text/html");
	i_client.println("Connection: close");
	i_client.println();
	// HTML header
	i_client.println("<!DOCTYPE html><html>");
	i_client.println(
		"<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" >");
	i_client.print("<script>function f_submit() {document.getElementById('main_form').submit();}</script>");
	i_client.print(
		"<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
	i_client.print(
		".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
	i_client.print(
		"text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
	i_client.print(
		".input {font-size: 30px; position: absolute; left: 100px; background-color:#FFFFE0;}");
	i_client.print(".button2 {background-color: #555555;}");
	i_client.print("#time {background-color: #c2fc03; width: 50%; border: 1px solid black; margin: auto; text-align: center;}</style>");
	i_client.print("<b>Schaltzeiten einstellen<br></b>");
	i_client.print("Hostname:&nbsp;<a href='http://");
	i_client.print(gs_DHCPhostname);
	i_client.print("/.'>");
	i_client.print(gs_DHCPhostname);
	i_client.print("</a>");
	i_client.print("<br>IP:&nbsp;");
	i_client.print(gs_myip);
	i_client.print("<br>Die Seite wurde aufgerufen:&nbsp;");
	i_client.print(&g_loc_time, "%d.%m.%Y %H:%M:%S");
	i_client.print("<br>");
	if (gi_state == 1) {
	i_client.print("Status: ");
	//if (gi_state == 0) {i_client.print("<span id='state' style='font-size:50px;color:gray'> &#8226;");}
	//if (gi_state == 1) {i_client.print("<span id='state' style='font-size:50px;color:yellow'> &#8226;");}
	i_client.print(gx_imgon);}
	i_client.print("</span></head>");
	// HTML Body
	i_client.print("<body><br><form id='main_form'><span style ='color:green;'>&nbsp;</span><table>");
	i_client.print("<tr style='background-color:silver;'><th>Wochentag</th><th>An</th><th>Aus</th></tr>");
	i_client.print("<tr><td>Montag</td><td><input type='time'  name='on1' value='");
	i_client.print(gx_on[1]);
	i_client.print("'></td><td><input type='time'  name='off1' value='");
	i_client.print(gx_off[1]);
	i_client.print("'></tr>");
	i_client.print("<tr><td>Dienstag</td><td><input type='time'  name='on2' value='");
	i_client.print(gx_on[2]);
	i_client.print("'></td><td><input type='time'  name='off2' value='");
	i_client.print(gx_off[2]);
	i_client.print("'></tr>");
	i_client.print("<tr><td>Mittwoch</td><td><input type='time'  name='on3' value='");
	i_client.print(gx_on[3]);
	i_client.print("'></td><td><input type='time'  name='off3' value='");
	i_client.print(gx_off[3]);
	i_client.print("'></tr>");
	i_client.print("<tr><td>Donnerstag</td><td><input type='time'  name='on4' value='");
	i_client.print(gx_on[4]);
	i_client.print("'></td><td><input type='time'  name='off4' value='");
	i_client.print(gx_off[4]);
	i_client.print("'></tr>");
	i_client.print("<tr><td>Freitag</td><td><input type='time'  name='on5' value='");
	i_client.print(gx_on[5]);
	i_client.print("'></td><td><input type='time'  name='off5' value='");
	i_client.print(gx_off[5]);
	i_client.print("'></tr>");
	i_client.print("<tr><td>Sonnabend</td><td><input type='time'  name='on6' value='");
	i_client.print(gx_on[6]);
	i_client.print("'></td><td><input type='time'  name='off6' value='");
	i_client.print(gx_off[6]);
	i_client.print("'></tr>");
	i_client.print("<tr><td>Sonntag</td><td><input type='time'  name='on0' value='");
	i_client.print(gx_on[0]);
	i_client.print("'></td><td><input type='time'  name='off0' value='");
	i_client.print(gx_off[0]);
	i_client.print("'></tr>");
	i_client.print("</table><input type='submit' value='Speichern'></form></BODY></html>");

	//        i_client.stop();
}

void flipswitch()
{
	cl_switch_man lo_swm;
	g_start_time = g_loc_time; // set current time
	g_end_time = g_loc_time;
	g_end_time.tm_hour = gx_off[g_loc_time.tm_wday].substring(0, 2).toInt(); // calculate scheduled end time for today
	g_end_time.tm_min = gx_off[g_loc_time.tm_wday].substring(3, 5).toInt();
	g_end_time.tm_sec = 0;
	g_start_time.tm_hour = gx_on[g_loc_time.tm_wday].substring(0, 2).toInt(); // calculate scheduled start time for today
	g_start_time.tm_min = gx_on[g_loc_time.tm_wday].substring(3, 5).toInt();
	g_start_time.tm_sec = 0;

	gi_state = 99;

	// if (&g_loc_time > &g_start_time && &g_loc_time < &g_end_time) //does not work -> convert to time_t(int)
	gi_etime = mktime(&g_end_time);
	gi_stime = mktime(&g_start_time);
	gi_ettime = mktime(&g_loc_time);
	if (gi_ettime > gi_stime && gi_ettime < gi_etime)
	{
		gi_state = 1;             //on
		digitalWrite(gpio, LOW);  //switch on
	}
	else
	{
		gi_state = 0;             // off 
		digitalWrite(gpio, HIGH); // switch off
	}
	if (gx_debug == "X")
	{
		Serial.println("------------------Flipswitch: ------------------");
		Serial.print("g_loc_time: ");
		Serial.println(&g_loc_time, "%A, %B %d %Y %H:%M:%S");
		Serial.print("g_start_time: ");
		Serial.println(&g_start_time, "%A, %B %d %Y %H:%M:%S");
		Serial.print("g_end_time: ");
		Serial.println(&g_end_time, "%A, %B %d %Y %H:%M:%S");
		Serial.print("gi_state 0-off, 1-on, 99-undefined: ");
		Serial.println(gi_state);
		for (int ln_count = 0; ln_count < 7; ln_count++)
		{
			Serial.print("gx_on[");
			Serial.print(ln_count);
			Serial.print("] : ");
			Serial.println(gx_on[ln_count]);
			Serial.print("gx_off[");
			Serial.print(ln_count);
			Serial.print("] : ");
			Serial.println(gx_off[ln_count]);
		}
		delay(1500);
	}
}

void saveswitchtimes()
{
	if (gx_debug == "Y")
	{
		Serial.println("------------------Saving switch times: ------------------");
		for (int ln_count = 0; ln_count < 7; ln_count++)
		{
			Serial.print("gx_on[");
			Serial.print(ln_count);
			Serial.print("] : ");
			Serial.println(gx_on[ln_count]);
			Serial.print("gx_off[");
			Serial.print(ln_count);
			Serial.print("] : ");
			Serial.println(gx_off[ln_count]);
		}
		delay(1000);
	}
	g_prefs.begin("switchtimes", false);
	char lx_on[7];
	char lx_off[8];

	for (int ln_count = 0; ln_count < 7; ln_count++)
	{

		//		sprintf(lx_on, "%s%i", lcx_on, ln_count);
		//		sprintf(lx_off, "%s%i", lcx_off, ln_count);
		sprintf(lx_on, "%s%i", "gx_on", ln_count);
		sprintf(lx_off, "%s%i", "gx_off", ln_count);

		g_prefs.putString(lx_on, gx_on[ln_count]);
		g_prefs.putString(lx_off, gx_off[ln_count]);
		if (gx_debug == "Y")
		{
			// Serial.println(ln_count);
			Serial.print("------------------------->putString lx_on: ");
			Serial.print(lx_on);
			Serial.println(gx_on[ln_count]);
			Serial.print("------------------------->putString lx_off: ");
			Serial.print(lx_off);
			Serial.println(gx_off[ln_count]);
			delay(1000);
		}
		if (gx_debug == "Y") {
			gx_on[ln_count] = g_prefs.getString(lx_on, "");
		gx_off[ln_count] = g_prefs.getString(lx_off, "");
		Serial.println("<-------------------- nach dem Put -------------------------->");
		Serial.print(lx_on);
		Serial.println(gx_on[ln_count]);
		Serial.print(lx_off);
		Serial.println(gx_off[ln_count]);
}
	}
	g_prefs.end();
}

void readswitchtimes()
{
	g_prefs.begin("switchtimes", false);
	//	String lcx_on = "gx_on";
	//	String lcx_off = "gx_off";
	char lx_on[7];
	char lx_off[8];
	for (int ln_count = 0; ln_count < 7; ln_count++)
	{
		sprintf(lx_on, "%s%i", "gx_on", ln_count);
		sprintf(lx_off, "%s%i", "gx_off", ln_count);
		gx_on[ln_count] = g_prefs.getString(lx_on, "");
		gx_off[ln_count] = g_prefs.getString(lx_off, "");
		if (gx_debug == "Y")
		{
			Serial.println("---------->>readswitchtimes()");
			Serial.print("------------------------->getString lx_on: ");
			Serial.print(lx_on);
			Serial.println(gx_on[ln_count]);
			Serial.print("------------------------->getString lx_off: ");
			Serial.print(lx_off);
			Serial.println(gx_off[ln_count]);
			delay(1000);
		}
	}
	g_prefs.end();
		
	
}