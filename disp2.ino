
//extern bool f_clok_D;                  //флаг отображение часов на дисплее
//extern uint16_t m, h, s, dn;

void Display() {

  if (f_clok_D) {
     clok_timer = millis();  //чтобы пока часы работают взводем таймер чтоб счет небыл гдето всередине, и небыло стирание дисплея, и не было повторго срабатывания в f_clok_D = 1; ,
     String chis1, chis2, chis3, chis4, chis5, chis6;
     chis1 = String((h / 10) % 10);
     chis2 = String((h % 10));
     chis3 = String((m/ 10) % 10);
     chis4 = String(m % 10);
     chis5 = String((s/ 10) % 10);
     chis6 = String(s % 10);
     //if (disp==0) {rnd = random(0, ARRAY_SIZE(catalog));}
     Text =  chis1 +chis2+  ":"+ chis3+chis4  ;
     if (P.displayAnimate()){  //истина анимации нет
        rnd = random(0, ARRAY_SIZE(catalog));
        utf8rus(Text).toCharArray(buf, 256);
        P.displayText(buf, PA_CENTER, catalog[rnd].speed, 3000, catalog[rnd].effect, catalog[rnd].effect);    
        //delay(10);
      }
  }   //if (f_clok_D) {
  else 
  {
     if (UserID == 0) {  
        P.setTextAlignment(PA_LEFT);
        float tpm ;
        if (units>1000){
          tpm =units/1000;
          P.print(tpm,2);
        } 
        else if (units>100){
          P.print(units,1);
        } 
        else {P.print(units,0);  }
    }  //if (UserID == 0) 
    else {
        if (UserID == 1 || UserID == 2 || UserID == 3 || UserID == 4 || UserID == 5) {  ///4 -это save
           Text =  name_all[UserID-1];
           utf8rus(Text).toCharArray(buf, 256);
           P.write(buf);
           delay(2000);
           UserID = 0;
        }
    }  //else {
  }   // else if (f_clok_D) {


}  //void Display() {
//------------------------------------------------------------------------------------------------------------
String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}
