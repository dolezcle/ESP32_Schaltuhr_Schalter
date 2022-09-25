class cl_switch_man {
    public:
    tm ltm_start_time;
	tm ltm_local_time;
	long li_ettime;			//actual local time
	long li_etime;          //absolute switchoff time in sec as local time
	long int li_autooff;    // autooff nach dem Lesen in ms
	
	


 

void initialize() {
	getLocalTime(&ltm_loc_time);
	li_ettime = mktime(&ltm_loc_time);				 // lokale Zeit als int aus der ntp-Zeit
	
	
}

void set_offtime(String lstr_autooff) {
	li_autooff = 0;		
	lstr_autooff.toCharArray(la_autooff, 4);
	li_autooff = atoi(la_autooff);
	if (li_autooff > 0)
		{
			li_etime = li_ettime + li_autooff;
		}
		else
		{
			//			gi_etime = gi_ettime + 59940;             //nach 999 min ausschalten
			//			gi_etime = 0;                             //bei Anmelden eines neuen client darf nicht initialisiert werden
		}	
 }
    
};