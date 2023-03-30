class cl_switch_man
{
public:
	tm ltm_start_time;
	tm ltm_local_time;
	long int li_acttime;		 // actual local time in sec
	long int li_etime;			 // absolute switchoff time in sec as local time
	long int li_nooff = 9999999; // no end time
	time_t lit_etime;
	long int li_autooff; // autooff nach dem Lesen in ms
	char la_autooff[10] = "";
	char la_state[2]; // 0 - off, 1 - on from range in HTML control
	const char *la_debug = "";
	bool lb_inidone = false;
	int li_state; // 0: manual off, 1: manual on
	struct tm ltm_loc_time;
	struct tm *ltm_end_time;

	void initialize()
	{
		getLocalTime(&ltm_loc_time);
		li_acttime = mktime(&ltm_loc_time); // local time as int from ntp-time
		lb_inidone = true;
		if (la_debug)
		{
			Serial.print("li_acttime in cl_switch_man.initialize: ");
			Serial.println(li_acttime);
			Serial.print("&ltm_loc_time in cl_switch_man.initialize: ");
			Serial.println(&ltm_loc_time);
			delay(3000);
		}
	}
	void set_offtime(String i_lstr_state, String i_lstr_autooff)
	{
		getLocalTime(&ltm_loc_time);
		li_acttime = mktime(&ltm_loc_time); // local time as int from ntp-time
		Serial.print("-----debugging function: ");
		Serial.println(__FUNCTION__);
		if (la_debug == "X")
		{
			Serial.print("----i_lstr_autooff in Start cl_switch_man.set_offtime: ");
			Serial.println(i_lstr_autooff);
			Serial.print("i_lstr_state in Start cl_switch_man.set_offtime: ");
			Serial.println(i_lstr_state);
			delay(3000);
			Serial.print("----i_lstr_autooff in Start cl_switch_man.set_offtime: ");
			Serial.println(i_lstr_autooff);
			Serial.print("i_lstr_state in Start cl_switch_man.set_offtime: ");
			Serial.println(i_lstr_state);
			delay(3000);
		}
		i_lstr_state.toCharArray(la_state, 2);
		li_state = atoi(la_state);
		if (i_lstr_autooff != "")
		i_lstr_state.toCharArray(la_state, 2);
		li_state = atoi(la_state);
		if (i_lstr_autooff != "")
		{
			i_lstr_state.toCharArray(la_state, 2);
			li_state = atoi(la_state);
			li_autooff = 0;
			i_lstr_autooff.toCharArray(la_autooff, 4);
			li_autooff = atoi(la_autooff);
			li_autooff = li_autooff * 60;
			if (li_autooff == 0)
			{
				li_autooff = li_nooff;
			}

			if (li_autooff == 0)
			{
				li_autooff = li_nooff;
			}

			if (li_autooff > 0 && li_state == 1)
			{
				li_etime = li_acttime + li_autooff;
				lit_etime = li_etime;
				if (lit_etime)
				{
					ltm_end_time = localtime(&lit_etime);
				}
			}
			else if (li_autooff == li_nooff)
			{
				li_etime = li_nooff;
			}
		}
		if (i_lstr_state == "0")
		{
			if (la_debug == "X")
			}
			else if (li_autooff == li_nooff)
			{
				li_etime = li_nooff;
			}
		}
		if (i_lstr_state == "0")
		{
			if (la_debug == "X")
			{
			Serial.print("-----debugging function: ");
			Serial.println(__FUNCTION__);
			Serial.print("i_lstr_state beim Ausschalten: ");
			Serial.println(i_lstr_state);
			li_etime = 0;
			li_state = 0;
			li_autooff = 0;
			}
			//			gi_etime = gi_ettime + 59940;             //switch off after 999 min
			//			gi_etime = 0;                             //bei Anmelden eines neuen client darf nicht initialisiert werden
		}

		if (la_debug == "X")
		{
			Serial.println("------------------cl_switch_man.set_offtime: -----------------------");
			Serial.print("-----debugging function: ");
			Serial.println(__FUNCTION__);
			Serial.print("la_debug: ");
			Serial.println(la_debug);
			Serial.print("li_acttime: ");
			Serial.println(li_acttime);
			Serial.print("li_autooff: ");
			Serial.println(li_autooff);
			Serial.print("li_etime: ");
			Serial.println(lit_etime);
			if (li_etime > 0)
			{
				Serial.print("= ltm_end_time: ");
				Serial.println(asctime(ltm_end_time));
			}

			Serial.print("li_state: ");
			Serial.println(li_state);
			delay(3000);
		}
	}
};