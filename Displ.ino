void Display() {

  matrix.fillScreen(LOW);
  int y = (matrix.height() - 8) / 2;  // Центрируем текст по Вертикали
  //int y =0;
  String chis1, chis2, chis3, chis4, chis5, chis6;
  //UserID=3;
  int  units_int2= (int)units;

  if (f_clok_D) {
    int y = 0;  // Центрируем текст по Вертикали
    String chis1, chis2, chis3, chis4, chis5, chis6;
    
    //h=12;
    chis1 = String((h / 10) % 10);
    chis2 = String((h % 10));
    chis3 = String((m/ 10) % 10);
    chis4 = String(m % 10);
    chis5 = String((s/ 10) % 10);
    chis6 = String(s % 10);
    
    matrix.drawChar(2, y, chis1[0], HIGH, LOW, 1);
    matrix.drawChar(8, y, chis2[0], HIGH, LOW, 1);
    if (f_sek) {f_sek=!f_sek; matrix.drawChar(14, y, (String(":"))[0], HIGH, LOW, 1);} else  {f_sek=!f_sek;matrix.drawChar(14, y, (String(" "))[0], HIGH, LOW, 1);}
    matrix.drawChar(19, y, chis3[0], HIGH, LOW, 1);
    matrix.drawChar(25, y, chis4[0], HIGH, LOW, 1);
    //matrix.drawChar(22, y, chis5[0], HIGH, LOW, 1);
    //matrix.drawChar(27, y, chis6[0], HIGH, LOW, 1);
   
    f_clok_D=0;
    //matrix.drawChar(7, y, (String(","))[0], HIGH, LOW, 1);
  } else {
    if (f_eror_sql) {
      matrix.drawChar(0, y, (String("E"))[0], HIGH, LOW, 1);
      matrix.drawChar(6, y, (String("R"))[0], HIGH, LOW, 1);
      matrix.drawChar(12, y, (String("R"))[0], HIGH, LOW, 1);
      matrix.drawChar(18, y, (String("O"))[0], HIGH, LOW, 1);
      matrix.drawChar(24, y, (String("R"))[0], HIGH, LOW, 1);
      matrix.drawChar(27, y, (String("!"))[0], HIGH, LOW, 1);
      delay(500);
    }

    if (UserID == 0) {
      if (units_int2 >= 0) {
        if (units_int2 >= 100000) {
          units_int2 /= 10;
          chis2 = String((units_int2 / 10000) % 10);
          chis3 = String((units_int2 / 1000) % 10);
          chis4 = String((units_int2 / 100) % 10);
          chis5 = String((units_int2 / 10) % 10);
          chis6 = String(units_int2 % 10);

          matrix.drawChar(0, y, chis2[0], HIGH, LOW, 1);
          matrix.drawChar(6, y, chis3[0], HIGH, LOW, 1);
          matrix.drawChar(12, y, chis4[0], HIGH, LOW, 1);
          matrix.drawChar(17, y, (String(","))[0], HIGH, LOW, 1);
          matrix.drawChar(21, y, chis5[0], HIGH, LOW, 1);
          matrix.drawChar(27, y, chis6[0], HIGH, LOW, 1);
        } else if (units_int2 >= 10000) {
          units_int2 /= 10;
          chis3 = String((units_int2 / 1000) % 10);
          chis4 = String((units_int2 / 100) % 10);
          chis5 = String((units_int2 / 10) % 10);
          chis6 = String(units_int2 % 10);

          matrix.drawChar(0, y, chis3[0], HIGH, LOW, 1);
          matrix.drawChar(6, y, chis4[0], HIGH, LOW, 1);
          matrix.drawChar(11, y, (String(","))[0], HIGH, LOW, 1);
          matrix.drawChar(15, y, chis5[0], HIGH, LOW, 1);
          matrix.drawChar(21, y, chis6[0], HIGH, LOW, 1);
          matrix.drawChar(27, y, (String("K"))[0], HIGH, LOW, 1);

        } else if (units_int2 >= 1000) {
          //h/=10;

          chis3 = String((units_int2 / 1000) % 10);
          chis4 = String((units_int2 / 100) % 10);
          chis5 = String((units_int2 / 10) % 10);
          chis6 = String(units_int2 % 10);
          matrix.drawChar(2, y, chis3[0], HIGH, LOW, 1);
          matrix.drawChar(7, y, (String(","))[0], HIGH, LOW, 1);
          matrix.drawChar(11, y, chis4[0], HIGH, LOW, 1);
          matrix.drawChar(17, y, chis5[0], HIGH, LOW, 1);
          matrix.drawChar(23, y, chis6[0], HIGH, LOW, 1);
          //matrix.drawChar(27, y, (String("K"))[0], HIGH, LOW, 1);
        } else if (units_int2 >= 100) {
          chis4 = String((units_int2 / 100) % 10);
          chis5 = String((units_int2 / 10) % 10);
          chis6 = String(units_int2 % 10);
          matrix.drawChar(3, y, chis4[0], HIGH, LOW, 1);
          matrix.drawChar(8, y, chis5[0], HIGH, LOW, 1);
          matrix.drawChar(14, y, chis6[0], HIGH, LOW, 1);
          matrix.drawChar(21, y, (String("g"))[0], HIGH, LOW, 1);
          matrix.drawChar(27, y, (String("r"))[0], HIGH, LOW, 1);
        }

        else {
          chis5 = String((units_int2 / 10) % 10);
          chis6 = String(units_int2 % 10);
          matrix.drawChar(7, y, chis5[0], HIGH, LOW, 1);
          matrix.drawChar(13, y, chis6[0], HIGH, LOW, 1);
          matrix.drawChar(21, y, (String("g"))[0], HIGH, LOW, 1);
          matrix.drawChar(27, y, (String("r"))[0], HIGH, LOW, 1);
        }
      } else {
        units_int2 = abs(units_int2);
        chis5 = String((units_int2 / 10) % 10);
        chis6 = String(units_int2 % 10);

        matrix.drawChar(0, y, (String("-"))[0], HIGH, LOW, 1);
        matrix.drawChar(7, y, chis5[0], HIGH, LOW, 1);
        matrix.drawChar(13, y, chis6[0], HIGH, LOW, 1);
        matrix.drawChar(21, y, (String("g"))[0], HIGH, LOW, 1);
        matrix.drawChar(27, y, (String("r"))[0], HIGH, LOW, 1);
      }

    }  //if UserID
    else {
      if (UserID == 1 || UserID == 2 || UserID == 3 || UserID == 4) {  ///4 -это save
        matrix.drawChar(0, y, (String(name_all[UserID - 1][0]))[0], HIGH, LOW, 1);
        matrix.drawChar(6, y, (String(name_all[UserID - 1][1]))[0], HIGH, LOW, 1);
        matrix.drawChar(12, y, (String(name_all[UserID - 1][2]))[0], HIGH, LOW, 1);
        matrix.drawChar(18, y, (String(name_all[UserID - 1][3]))[0], HIGH, LOW, 1);
        matrix.drawChar(24, y, (String(name_all[UserID - 1][4]))[0], HIGH, LOW, 1);
        matrix.drawChar(27, y, (String(name_all[UserID - 1][5]))[0], HIGH, LOW, 1);
        UserID = 0;
      }
    }
  
  }//else   if(f_clok)

  matrix.write();  // Вывод на дисплей

}  


void wrem_disp() {
  matrix.fillScreen(LOW);
  int y = 0;  // Центрируем текст по Вертикали
              //int y = (matrix.height() - 8) / 2;
  String chis1, chis2, chis3, chis4, chis5, chis6;
  matrix.drawChar(0, y, (String(name_all[0][0]))[0], HIGH, LOW, 1);
  matrix.drawChar(6, y, (String(name_all[0][1]))[0], HIGH, LOW, 1);
  matrix.drawChar(12, y, (String(name_all[0][2]))[0], HIGH, LOW, 1);
  matrix.drawChar(18, y, (String(name_all[0][3]))[0], HIGH, LOW, 1);
  matrix.drawChar(24, y, (String(name_all[0][4]))[0], HIGH, LOW, 1);
  matrix.drawChar(30, y, (String(name_all[0][5]))[0], HIGH, LOW, 1);
  UserID = 0;

  matrix.write();  // Вывод на дисплей
}


void disp_chi(int numm) {
  matrix.fillScreen(LOW);
  int y = 0;  // Центрируем текст по Вертикали
  String chis1, chis2, chis3, chis4, chis5, chis6;
  chis3 = String((numm / 1000) % 10);
  chis4 = String((numm / 100) % 10);
  chis5 = String((numm / 10) % 10);
  chis6 = String(numm % 10);
  matrix.drawChar(2, y, chis3[0], HIGH, LOW, 1);
  matrix.drawChar(7, y, (String(","))[0], HIGH, LOW, 1);
  matrix.drawChar(11, y, chis4[0], HIGH, LOW, 1);
  matrix.drawChar(17, y, chis5[0], HIGH, LOW, 1);
  matrix.drawChar(23, y, chis6[0], HIGH, LOW, 1);

  matrix.write();  // Вывод на дисплей
}




void Install_Disp() {
  matrix.setIntensity(0);  // Яркость матрицы от 0 до 15
  // начальные координаты матриц 8*8
  matrix.setRotation(0, 1);  // 1 матрица
  matrix.setRotation(1, 1);  // 2 матрица
  matrix.setRotation(2, 1);  // 3 матрица
  matrix.setRotation(3, 1);  // 4 матрица
}
