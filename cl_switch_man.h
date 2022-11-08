class cl_switch_man {
    public:
    tm ltm_start_time;
	tm ltm_local_time;
	long li_acttime;			//actual local time in sec
	long li_etime;          //absolute switchoff time in sec as local time
	long int li_autooff;    // autooff nach dem Lesen in ms
	char la_autooff[10] = "X";
	char la_state[2];       //0 - off, 1 - on from range in HTML control 
	char la_debug[2] = "";
	int li_state;
	struct tm ltm_loc_time;
	struct tm ltm_end_time;
	

void initialize() {
	getLocalTime(&ltm_loc_time);
	li_acttime = mktime(&ltm_loc_time);				 // local time as int from ntp-time
	
	
}

void set_offtime(String lstate, String lstr_autooff) {
	lstate.toCharArray(la_state, 2);
    li_state = atoi(la_state);
	li_autooff = 0;		
	lstr_autooff.toCharArray(la_autooff, 4);
	li_autooff = atoi(la_autooff);
	if (li_autooff > 0)
		{
			li_etime = li_acttime + li_autooff;
		}
		else
		{
			//			gi_etime = gi_ettime + 59940;             //switch off after 999 min 
			//			gi_etime = 0;                             //bei Anmelden eines neuen client darf nicht initialisiert werden
		}	
		if (la_debug == "X") {
			Serial.print("li_etime: ");
			Serial.println(li_etime);
			Serial.print("li_state: ");
			Serial.println(li_state);
			delay(3000);
		}
 }
    
};