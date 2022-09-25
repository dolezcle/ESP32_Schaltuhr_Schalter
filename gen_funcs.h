String get_substring(String ps_instring, String ps_pattern_from, String ps_pattern_to, int pi_offset)
{
    // extracts and returns a substring out of the input string
    // Pattern_from and pattern_to must be unique in the source - if the pattern can be found in various places of the source the result will not be reliable
    String ls_payload;
    int li_indexfrom = ps_instring.indexOf(ps_pattern_from);
    int li_leftoffset = ps_pattern_from.length();
    // li_leftoffset++;
    li_leftoffset = li_leftoffset + pi_offset; // externally given to adjust invisible \0 etc.
    if (li_indexfrom > 0)
    {
        li_indexfrom = li_indexfrom + li_leftoffset;
        int li_indexto = gx_header.indexOf(ps_pattern_to);
        if (li_indexto > -1)
        {
            ls_payload = ps_instring.substring(li_indexfrom, li_indexto);
        }
        if (li_indexfrom >= li_indexto)
        {
            ls_payload = "";
        }
        if (gx_debug == "X") {
        Serial.print("in get_substring -> ps_instring:  ");
        Serial.println(ps_instring);
        Serial.print("in get_substring -> ps_pattern_from:  ");
        Serial.println(ps_pattern_from);
        Serial.print("in get_substring -> ps_pattern_to:  ");
        Serial.println(ps_pattern_to);
        Serial.print("in get_substring -> : li_indexfrom ");
        Serial.println(li_indexfrom);
        Serial.print("in get_substring -> : li_indexto ");
        Serial.println(li_indexto);
        Serial.print("in get_substring -> ls_payload:  ");
        Serial.println(ls_payload);
        delay(1000);
        }
        return ls_payload;
    }
}

void get_ntptime()
{

    configTime(0, 0, "pool.ntp.org");
    //	  setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);                  //scheint im setup() nicht zu reichen
    //	  getLocalTime(&g_loc_time);
    //	  Serial.print("im get_ntp_time: ");
    //	  Serial.println(&g_loc_time, "%A, %B %d %Y %H:%M:%S");
}

IPAddress str2ipv4(String ps_instring, char pa_separator)
{
    String ls_reststring;
    int li_indexfrom;
    int li_indexto;
    IPAddress la_ip;
    // switch (pa_separator)
    //     {
    //         case ' ': pa_separator = '.';
    //         break;
    //         case '.':
    //         break;
    //         case ',':
    //         break;
    //     }

    ls_reststring = ps_instring;
    li_indexto = ls_reststring.indexOf(pa_separator);
    la_ip[0] = atoi(ls_reststring.substring(0, li_indexto).c_str());
    li_indexfrom = li_indexto + 1;
    li_indexto = ls_reststring.length();
    ls_reststring = ls_reststring.substring(li_indexfrom, li_indexto);
    li_indexto = ls_reststring.indexOf(pa_separator);
    la_ip[1] = atoi(ls_reststring.substring(0, li_indexto).c_str());
    li_indexfrom = li_indexto + 1;
    li_indexto = ls_reststring.length();
    ls_reststring = ls_reststring.substring(li_indexfrom, li_indexto);
    li_indexto = ls_reststring.indexOf(pa_separator);
    la_ip[2] = atoi(ls_reststring.substring(0, li_indexto).c_str());
    li_indexfrom = li_indexto + 1;
    li_indexto = ls_reststring.length();
    ls_reststring = ls_reststring.substring(li_indexfrom, li_indexto);
    li_indexto = ls_reststring.indexOf(pa_separator);
    la_ip[3] = atoi(ls_reststring.substring(0, li_indexto).c_str());
    Serial.print("ps_instring in str2ipv4 :  ");
    Serial.println(ps_instring);
    Serial.print("Ende in str2ipv4 :  ");
    Serial.println(la_ip[0]);
    Serial.println(la_ip[1]);
    Serial.println(la_ip[2]);
    Serial.println(la_ip[3]);
    // delay(5000);
    return la_ip;
}

String ipv42string(IPAddress pa_ip)
{
    String ls_result;
    ls_result = pa_ip[0];
    ls_result = ls_result + '.' + pa_ip[1];
    ls_result = ls_result + '.' + pa_ip[2];
    ls_result = ls_result + '.' + pa_ip[3];
    Serial.print("ls_result in ipv42string :  ");
    Serial.println(ls_result);
    delay(2000);
    return ls_result;
}