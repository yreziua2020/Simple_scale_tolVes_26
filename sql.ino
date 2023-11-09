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


  
}



void bd_sql_read() {
    HTTPClient http;  //--> Declare object of class HTTPClient.
    WiFiClient wifiClient;

    int httpCode;     //--> Variables for HTTP return code.
    //........................................ Process to get LEDs data from database to control LEDs.
    postData = "";
    payload = "";
    http.begin(wifiClient,"http://dev.ibc-composite.com/php/jsonp_vit.php");  //--> Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");        //--> Specify content-type header
   
    httpCode = http.POST(postData); //--> Send the request
    payload = http.getString();     //--> Get the response payload
      Serial.print("httpCode : ");
    Serial.println(httpCode); //--> Print HTTP return code
    Serial.print("payload  : ");
    Serial.println(payload);  //--> Print request response payload
    /*
      int coma = payload.indexOf(',');
     Serial.print("Coma1=");Serial.print(coma);
        coma = payload.indexOf(',',coma=coma+1); //что ичем  и с какого индекса
         Serial.print("Coma2=");Serial.print(coma);
      coma++; //увеличивае инддекс на 1 чтоб сметить к значению(не копировать запятую) и также чтоб искать потом следующую запятую
      int coma_sled=payload.indexOf(',',coma); //кома исчем запятую с увеличеной позициии. следущая чтобы скопировть число между запятыми 
     vlaga_perem = payload.substring(coma,coma_sled).toInt();
     coma=coma_sled+1;
     coma_sled=payload.indexOf(',',coma);
     Serial.print("Coma3=");Serial.print(coma);
     temp_sql  = payload.substring(coma,coma_sled).toFloat();
    coma=coma_sled+1;
    coma_sled=payload.indexOf(']',coma);
     Serial.print("Coma4");Serial.print(coma);
     volt_bat  = payload.substring(coma,coma_sled).toFloat();
    */
     int firstHashIndex   = payload.indexOf(',');                      // получаем индекс первого разделительного хештега
     int secondHashIndex  = payload.indexOf(',', firstHashIndex + 1);  // получаем индекс второго разделительного хештега
     int thirdHashIndex   = payload.indexOf(',', secondHashIndex + 1); // получаем индекс третьего разделительного хештега
     int fourthHashIndex  = payload.indexOf(',', thirdHashIndex + 1);  // получаем индекс четвёртого разделительного хештега
     int fifthHashIndex   = payload.indexOf(',', fourthHashIndex + 1); // получаем индекс пятого разделительного хештега
      vlaga_perem = payload.substring(secondHashIndex + 1, thirdHashIndex).toInt(); 
      temp_sql =    payload.substring(thirdHashIndex + 1, fourthHashIndex).toFloat(); 
      volt_bat =    payload.substring(fourthHashIndex + 1, fifthHashIndex).toFloat(); 

  
    Serial.print("Влажность=");Serial.println(vlaga_perem);
    Serial.print("Температура=");Serial.println(temp_sql);
    Serial.print("Напряженте=");Serial.println(volt_bat);

    http.end();  //--> Close connection
    wifiClient.stop();

}
