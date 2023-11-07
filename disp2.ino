
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
     if (UserID == 0) {  //если равен нулю то показываем вес
        P.setTextAlignment(PA_CENTER);
        float tpm ;
       // units=units+10000;
      if (units>10000){
          tpm =units/1000;

          Text=(String)(tpm );  //сначала в int чтобы избавиться от значениий после запятой а потом String
          utf8rus(Text).toCharArray(buf, 256);
           P.write(buf);
           //P.print(tpm,3);

        } 
        else if (units>1000){
          tpm =units/1000;

          Text=(String)(tpm )+"кг";  //сначала в int чтобы избавиться от значениий после запятой а потом String
          utf8rus(Text).toCharArray(buf, 256);
           P.write(buf);
           //P.print(tpm,3);

        } 
        else if (units>-100) {
          Text=(String)((int)units)+"гр";  //сначала в int чтобы избавиться от значениий после запятой а потом String
          //char  tmp5=  "units+";
          //P.print(units,0);  }
          utf8rus(Text).toCharArray(buf, 256);
          //P.print(units,0); 
            P.write(buf);
          
         } 
         else {
          Text=(String)((int)units)+"г";  //сначала в int чтобы избавиться от значениий после запятой а потом String
          utf8rus(Text).toCharArray(buf, 256);
          P.write(buf);
         }


    }  //if (UserID == 0) 
    else {
        if (UserID == 1 || UserID == 2 || UserID == 3 || UserID == 4 || UserID == 5) {  ///4 -это save
           P.setTextAlignment(PA_CENTER);
          // Text =  name_all[UserID-1] +(String)" "+(String)(units/1000)+"кг" ;
           Text =  name_all[UserID-1] ;  //" "+(String)(units/1000)+"кг" ;
           utf8rus(Text).toCharArray(buf, 256);
           P.displayScroll(buf,PA_LEFT, PA_SCROLL_RIGHT, 60);
            //delay(500);
           while (!P.displayAnimate()){delay(1);}; // delay(1) -для того чтоб не сработал WDT
            Text=(String)(units/1000)+"кг"; 
           utf8rus(Text).toCharArray(buf, 256);
           P.write(buf);
           delay(500);//пауза для просмотра веса в бегущей строке

            UserID = 0;
        }
    }  //else {
  }   // else if (f_clok_D) {


    if (f_eror_sql) {
       P.setTextAlignment(PA_CENTER);
      Text =  name_all[5];
      utf8rus(Text).toCharArray(buf, 256);

      P.write(buf);


      delay(2000);
      f_eror_sql=0;
    }


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
