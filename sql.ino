void bd_sql() {


if (!client.connect(DB_host,Port)) {
    Serial.print(DB_host);
    Serial.println(" - Connection failed.");
    client.stop();
    f_eror_sql=1;
  }
    if (client.connect(DB_host, Port))
    {
    Serial.println("database connection established");  
    client.print( "GET /php/wp-scale2.php?A=Vp3wDWbP&ID=");
    client.print(UserID);
    client.print("&T1=");
    client.print(units); //data
    client.print("&P=");
    client.print(database_pass);
    client.println( " HTTP/1.1");
    client.print( "Host:" );
    client.println(DB_host);
    client.println( "Connection: close" );
    client.println();
    client.println();
    client.stop();
    
    }

  bd_sql_read() ;
}


void bd_sql_read() {
    HTTPClient http_r;  //--> Declare object of class HTTPClient.
    //WiFiClient wifiClient_r;   //client

    int httpCode;     //--> Variables for HTTP return code.
    //........................................ Process to get LEDs data from database to control LEDs.
    postData = "";
    payload = "";
    http_r.begin(client,"http://dev.ibc-composite.com/php/j_max_id1.php");  //--> Specify request destination
    http_r.addHeader("Content-Type", "application/x-www-form-urlencoded");        //--> Specify content-type header
   
    httpCode = http_r.POST(postData); //--> Send the request
    payload = http_r.getString();     //--> Get the response payload
      Serial.print("httpCode : ");
    Serial.println(httpCode); //--> Print HTTP return code
    Serial.print("payload  : ");
    Serial.println(payload);  //--> Print request response payload
     int firstHashIndex   = payload.indexOf(',');                      // получаем индекс первого разделительного хештега
     int secondHashIndex  = payload.indexOf(',', firstHashIndex + 1);  // получаем индекс второго разделительного хештега
     int thirdHashIndex   = payload.indexOf(',', secondHashIndex + 1); // получаем индекс третьего разделительного хештега
     int fourthHashIndex  = payload.indexOf(',', thirdHashIndex + 1);  // получаем индекс четвёртого разделительного хештега
   //  int fifthHashIndex   = payload.indexOf(',', fourthHashIndex + 1); // получаем индекс пятого разделительного хештега
      int date_sql = payload.substring(firstHashIndex + 1, secondHashIndex).toInt(); 
      int id_sql = payload.substring(secondHashIndex + 1, thirdHashIndex).toInt(); 
     int ves_sql =    payload.substring(thirdHashIndex + 1, fourthHashIndex).toFloat(); 
    //  volt_bat =    payload.substring(fourthHashIndex + 1, fifthHashIndex).toFloat(); 


    //int days = 0;
   // unsigned long days;

    int days = 0;
  int g_month = 1, g_day, g_dayOfWeek, g_year;  //месяц, число . день недели год    
/*
    uint32_t time;
    time = date_sql / 86400;
    uint16_t m_sql = ( date_sql / 60 ) % 60;
    uint16_t h_sql = ( date_sql / 3600 ) % 24;
    uint16_t s_sql =  date_sql  % 60;
     g_dayOfWeek = (((time) + 4) % 7) + 1; //dn = (((time) + 4) % 7) + 1;
  */  
 uint32_t time;
 time = (uint32_t)date_sql;
 Serial.print("time=");Serial.println(time);
  uint16_t s_sql  = time % 60;
  time /= 60; // now it is minutes
  uint16_t m_sql = time % 60;
  time /= 60; // now it is hours
  uint16_t h_sql = time % 24;
  time /= 24; // now it is days
  g_dayOfWeek = ((time + 4) % 7) + 1;  // Sunday is day 1
 Serial.print("time2=");Serial.println(time);





    g_dayOfWeek = (((time) + 4) % 7) + 1; //dn = (((time) + 4) % 7) + 1;
    while ((unsigned)(days += (LEAP_YEAR(g_year) ? 366 : 365)) <= time) {  g_year++; } 

    Serial.print("g_year=");Serial.println(g_year);      
    

    days -= LEAP_YEAR(g_year) ? 366 : 365;
    time -= days;
    days = 0;
    g_month = 0;
  
    uint8_t monthLength = 0;
    for (g_month = 0; g_month < 12; g_month++) {
      if (g_month == 1) {
        if (LEAP_YEAR(g_year)) monthLength = 29;
        else monthLength = 28;
      }
      else monthLength = monthDays[g_month];
      if (time >= monthLength) time -= monthLength;
      else break;
    }
    g_month++;
    g_day = time + 1;
    g_year += 1970;
    
    Serial.print("вес=");Serial.println(ves_sql/1000);
    Serial.print(h_sql);Serial.print(":");Serial.print(m_sql);Serial.print(" "); Serial.print(g_day); Serial.print("/");Serial.println(g_month);
    Serial.print("g_year=");Serial.println((String)g_year);   
    Serial.print("g_dayOfWeek=");Serial.println(g_dayOfWeek);    

    http_r.end();  //--> Close connection
      //wifiClient_r.stop();

}
