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
  Serial.begin(9600);  delay (100);
  
  scale_istall();
  
P.print("helo");

delay(2000);

  WiFi.begin(ssid, password);   WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS);
  while (WiFi.status() != WL_CONNECTED) { delay(300);Serial.print("."); if(raz_podk++>250){ESP.reset(); }}  
  delay(500);  
  
  HTTP.begin();         // Инициализируем Web-сервер
  delay(100); 
  Serial.println(WiFi.localIP());    
  httpUpdater.setup(&HTTP,  OTAUSER, OTAPASSWORD); //для обновления по вебу 
  
  delay(100);
  udp.begin(localPort);
  //clok();
}
