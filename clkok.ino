void clok() {
  cur_ms       = millis();
  t_cur        = cur_ms / 1000;
  // Каждые 60 секунд считываем время в интернете
  if ( cur_ms < ms2 || (cur_ms - ms2) > 600000 ) { //1 условие (cur_ms < ms2)  / или 1 или 2/2 условие ((cur_ms - ms2) > 600000 ) 
     //Serial.print("cur_ms="); Serial.print(cur_ms);Serial.print(" ");
     //Serial.print("ms2="); Serial.print(ms2);Serial.print(" ");
     //Serial.print("cur_ms-ms2=");Serial.print(cur_ms - ms2);Serial.print(" ");
      err_count++;
    // Делаем три  попытки синхронизации с интернетом
    //Serial.print(err_count);Serial.println(" ");
    if ( GetNTP() ) {
      ms2       = cur_ms;
      err_count = 0;
      t_correct = ntp_time - t_cur;

     //Serial.print("cur_ms="); Serial.print(cur_ms);Serial.print(" ");
    // Serial.print("ms2="); Serial.print(ms2);Serial.print(" ");
     //Serial.println(" ");
    } //else {if (kol_eror_ntp++>2) {kol_eror_ntp=0; ms2 = cur_ms;   }}
    if (err_count>2) {err_count=0; ms2 = cur_ms;   }
    //Serial.print("err_count="); Serial.print(err_count);
  }

  // Каждые 0.5 секунды выдаем время
  if ( cur_ms < ms1 || (cur_ms - ms1) > 500 ) {
    ms1 = cur_ms;
    ntp_time    = t_cur + t_correct;
   DisplayTime();
    points = !points;
  }
  
}


void DisplayTime(void) {
   m = ( ntp_time / 60 ) % 60;
   h = ( ntp_time / 3600 ) % 24;
   s =  ntp_time  % 60;
 
  uint32_t time;
  time = ntp_time / 86400;
  dn = (((time) + 4) % 7) + 1;
 // if (EEPROM.read(0) == h && EEPROM.read(1) == m ) {    Reley(6);  }
  if(_Dclock) {Serial.print(h);  Serial.print(":");  Serial.print(m); Serial.print(":");  Serial.print(s);  Serial.print("-");  Serial.println(dn);}
}

bool GetNTP(void) 
{
  udp.begin(localPort);
 // WiFi.hostByName(ntpServerName, timeServerIP);
 if( WiFi.hostByName(ntpServerName, timeServerIP)) {if(_Dclock) {Serial.print("DNS в ip получение ip выполнено ");} } else  {if(_Dclock) {Serial.print("DNS в ip не получен ");return false;} }
 //Serial.print("IP");for (uint16_t i=0 ; i<4 ; i++ ){   Serial.print(timeServerIP[i]);Serial.print(" ");} Serial.println();
  sendNTPpacket(timeServerIP);
  //delay(1000);
  
  unsigned long timeout = millis() + 1000;
  while ((udp.available() < NTP_PACKET_SIZE) && (millis() < timeout)) {}

  int cb = udp.parsePacket();
  if (!cb) {    Serial.println("No packet yet");    return false;  }
  else {
    if(_Dclock) {Serial.print("packet received, length=");   Serial.println(cb);}
    udp.read(packetBuffer, NTP_PACKET_SIZE);    // Читаем пакет в буфер
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);    // 4 байта начиная с 40-го сождержат таймстамп времени - число секунд    // от 01.01.1900
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;     // Конвертируем два слова в переменную long
    uint64_t secsSince1900_2 = secsSince1900;
    const unsigned long seventyYears = 2208988800UL;    // Конвертируем в UNIX-таймстамп (число секунд от 01.01.1970
       unsigned long epoch = secsSince1900 - seventyYears;
      // unsigned long epoch = secsSince1900 *1000;
    // Делаем поправку на местную тайм-зону
    ntp_time = epoch + TIMEZONE * 3600;
    if(_Dclock) {Serial.print("Unix time = ");  Serial.println(ntp_time);}
    
  }
  return true;
}
//unsigned long sendNTPpacket(IPAddress& address)

void  sendNTPpacket(IPAddress& address)
{
  if(_Dclock) {Serial.println("sending NTP packet...");}
  // Очистка буфера в 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Формируем строку зыпроса NTP сервера
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // Посылаем запрос на NTP сервер (123 порт)
  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
   // Serial.print("packetBuffer=");  for (uint16_t i=0 ; i<NTP_PACKET_SIZE ; i++ ){   Serial.print(packetBuffer[i]);Serial.print(" ");}    Serial.println();
  udp.endPacket();
}

void clok_dis(void) {

  
}
