void setup() {
  pinMode (Knop_PIN, INPUT_PULLUP); //кнопка

 // Install_Disp();
  P.begin();
  P.setInvert(false);
  P.setFont(fontBG);
//  P.shutdown(0,false);
  P.setIntensity(0);
  P.setZoneEffect(0,true,PA_FLIP_UD);
  P.setZoneEffect(0,true,PA_FLIP_LR);
 // P.setSpeed(1000) ;

{ //P.getSpeed()=10 ВСЕГДА
  for (uint8_t i=0; i<ARRAY_SIZE(catalog); i++)
  {
    Serial.print("P.getSpeed()="); Serial.println(P.getSpeed());
    catalog[i].speed *= P.getSpeed();
    catalog[i].pause *= 500;
  }
}





  Serial.begin(115200);  delay (100);
  
 /////////// scale_istall();
  
//P.print("helo");

delay(1000);

  WiFi.begin(ssid, password);   WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS);
  while (WiFi.status() != WL_CONNECTED) { delay(300);Serial.print("."); if(raz_podk++>250){ESP.reset(); }}  
  delay(500);  
  
  HTTP.begin();         // Инициализируем Web-сервер
  delay(100); 
  Serial.println(WiFi.localIP());    
  httpUpdater.setup(&HTTP,  OTAUSER, OTAPASSWORD); //для обновления по вебу 
  
  delay(100);
  udp.begin(localPort);
 // bd_sql_read();
  //clok();
  bd_sql_read();
}
