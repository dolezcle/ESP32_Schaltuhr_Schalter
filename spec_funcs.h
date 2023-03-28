
void mainhtml1(WiFiClient i_client, cl_switch_man lo_switch_man)
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
	// -------------------------------------------------HTML header start
	i_client.println("HTTP/1.1 200 OK");
	i_client.println("Content-type:text/html");
	i_client.println("Connection: close");
	i_client.println();
	// ------------------------------------------------- HTML header end
	i_client.println("<!DOCTYPE html><html>");
	i_client.println(
		"<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" >");
	// -------------------------------------------------js func f_submit
	i_client.print("<script>function f_submit() {document.getElementById('range_form').submit();}</script>");
	// -------------------------------------------------end js func f_submit
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
	// -------------------------------------------------Range element for manual switch: call routine f_submit
	i_client.print("Manuell schalten<br>");
	i_client.print("<br><form id='range_form'><span style ='color:green;'>&nbsp;Aus</span><input type='range' oninput='f_submit()' id='switch' name='switch' min='0' max='1' value='");
	i_client.print(gi_state);
	i_client.print("'><span style ='color:red;'>&nbsp;An</span>");
	i_client.print("<br>Ausschalten in<input style='background-color:#FFFFE0' name='ttl' type='number' min='1' max='999' maxlength='3' size='3' autofocus>Minuten<br><br></p></form>");
	i_client.print("<br><br><br>");
<<<<<<< HEAD

=======
    // -------------------------------------------------Display switchoff time
>>>>>>> 79d8d3f23a5a0ee915813dcb758a688aef72405a
	// timer:gi_etime is set && manual state not set OR manual end time set and status = on
	if (((gi_etime > 0 && lo_switch_man.li_state == 0) || (lo_switch_man.li_etime > 0) && lo_switch_man.li_etime != lo_switch_man.li_nooff) && gi_state == 1)
	{
		i_client.println("<br> <div id='time'> Ausschalten am: ");
		if (gi_etime > 0 && gi_etime < lo_switch_man.li_etime)
		{
			i_client.println(localtime(&gi_etime), "%d.%m.%Y");
			i_client.println(" um: ");
			i_client.println(localtime(&gi_etime), "%H:%M:%S");
		}
		else if (lo_switch_man.li_etime > 0)
			i_client.println(localtime(&lo_switch_man.li_etime), "%d.%m.%Y");
		i_client.println(" um: ");
		i_client.println(localtime(&lo_switch_man.li_etime), "%H:%M:%S");
	}
	//							"%A, %B %d %Y %H:%M:%S");
	i_client.print("</div>");

	// -------------------------------------------------End of Range element
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
	if (gi_state == 1)
	{
		i_client.print("Status: ");
		// if (gi_state == 0) {i_client.print("<span id='state' style='font-size:50px;color:gray'> &#8226;");}
		// if (gi_state == 1) {i_client.print("<span id='state' style='font-size:50px;color:yellow'> &#8226;");}
		i_client.print(gx_imgon);
	}
	i_client.print("</span></head>");
	// HTML Body
	// Main Form
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
	g_start_time = g_loc_time; // set current time
	g_end_time = g_loc_time;
	g_end_time.tm_hour = gx_off[g_loc_time.tm_wday].substring(0, 2).toInt(); // calculate scheduled end time for today
	g_end_time.tm_min = gx_off[g_loc_time.tm_wday].substring(3, 5).toInt();
	g_end_time.tm_sec = 0;
	g_start_time.tm_hour = gx_on[g_loc_time.tm_wday].substring(0, 2).toInt(); // calculate scheduled start time for today
	g_start_time.tm_min = gx_on[g_loc_time.tm_wday].substring(3, 5).toInt();
	g_start_time.tm_sec = 0;

	gi_etime = mktime(&g_end_time);
	gi_stime = mktime(&g_start_time);
	gi_ettime = mktime(&g_loc_time);
	// timer: act time > start time AND act time < end time OR act time < manual end AND manuall state is ON --> switch on
	if (gi_ettime > gi_stime && gi_ettime < gi_etime || (gi_ettime < go_switch_man.li_etime && go_switch_man.li_state == 1))
	{
		gi_state = 1;			 // on
		digitalWrite(gpio, LOW); // switch on
		if (gx_debug == "X")
		{
		Serial.print("-----debugging function: ");
		Serial.println(__FUNCTION__);
		Serial.print("gi_state switch on: ");
		Serial.println(gi_state);
		delay(1000);
		}
	}
	// timer: (act time < start time AND manual on is not set) OR ((act time > man end time AND state = ON)  OR manual switch OFF) --> switch off
	else if ((gi_ettime < gi_stime && go_switch_man.li_state == 0) || ((gi_ettime > go_switch_man.li_etime && gi_state == 1) || go_switch_man.li_state == 0))
	{
		if (gx_debug == "X")
		{
			Serial.print("-----debugging function: ");
			Serial.println(__FUNCTION__);
			Serial.print("gi_ettime: ");
			Serial.println(gi_ettime);
			Serial.print("gi_stime : ");
			Serial.println(gi_stime);
			Serial.print("go_switch_man.li_etime: ");
			Serial.println(go_switch_man.li_etime);
			Serial.print("gi_state: ");
			Serial.println(gi_state);
			Serial.print("go_switch_man.li_state: ");
			Serial.println(go_switch_man.li_state);
			Serial.println("CALLING NOW go_switch_man.set_offtime with 0 xx");
			Serial.println("<----------------------------------------------------------->");
			delay(2000);
		}
		gi_state = 0;			  // off
		digitalWrite(gpio, HIGH); // switch off
								  //		char *intStr;
								  //		itoa(gi_state, intStr, 2);
		String lstr_temp_state;
		lstr_temp_state = gi_state;
		go_switch_man.set_offtime("0", lstr_temp_state);
		if (gx_debug == "X"){
		Serial.print("lstr_temp_state: ");
		Serial.println(lstr_temp_state);
		Serial.print("gi_state switch off: ");
		Serial.println(gi_state);
		delay(1000);
		}
	}

	if (gx_debug == "X")
	{
		Serial.println("------------------Flipswitch: ------------------");
		Serial.print("-----debugging function: ");
		Serial.println(__FUNCTION__);
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
		Serial.print("gi_state: ");
		Serial.println(gi_state);
		Serial.print("-----debugging function: ");
		Serial.print(__FUNCTION__);
		Serial.print("----- E N D E -----");
		delay(1500);
	}
}

void saveswitchtimes()
{
	if (gx_debug == "Y")
	{
		Serial.println("------------------Saving switch times: ------------------");
		Serial.print("-----debugging function: ");
		Serial.println(__FUNCTION__);
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
			Serial.print("-----debugging function: ");
			Serial.println(__FUNCTION__);
			Serial.print("------------------------->putString lx_on: ");
			Serial.print(lx_on);
			Serial.println(gx_on[ln_count]);
			Serial.print("------------------------->putString lx_off: ");
			Serial.print(lx_off);
			Serial.println(gx_off[ln_count]);
			delay(1000);
		}
		if (gx_debug == "Y")
		{
			Serial.print("-----debugging function: ");
			Serial.println(__FUNCTION__);
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
			Serial.print("-----debugging function: ");
			Serial.println(__FUNCTION__);
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