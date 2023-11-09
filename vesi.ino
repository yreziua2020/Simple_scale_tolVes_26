void scale_ves(){
//--------------------------------------------------------------------калибровка----------------------------------------------------
#ifdef kalib
 Serial.print("Калибровка ");
#endif
 //---------------------------------------------------------------------рабочий режим-----------------------------------------------
 #ifdef raboh     
   if ( (millis()-zader_vesi) > 350 ) { zader_vesi=millis();

  //scale.power_up();
  float ounces;
   if (f_clok_D) { ounces = scale.get_units(1);}
  else { ounces = scale.get_units(10);}// получаем значение с датчика, усреднённое по 10 измерениям

 //scale.power_down();
   units = ounces * 0.035274;  // переводим вес из унций в граммы
  //units = units+ 3800; //округление до ближайшего целого и условное тарирование
  units = round(units); //округление до ближайшего целого и условное тарирование
 // units_int =round(units);
   //Serial.print("Вес предыдущий: ");   Serial.print(pred_uint); 
  // Serial.print(" Вес int: ");   Serial.println(units);
  
  if (units >1000)  {
    uint16_t raz= abs(pred_uint-units);
    //Serial.print("raz=");Serial.print(raz);  
    if(raz<100 ) {  //100 
        if (count_sav<5) {count_sav++;  Serial.print("count_sav"); Serial.print(count_sav); 
        } 
        else
        { 
          if (!f_sav) {
            Serial.println(" запись");
            UserID=5;   //при обновлении дислплея выведит значение а ни вес
            if (units>86000&&units<120000){  f_sav=1;  Serial.print("SQL "); UserID=1; bd_sql();}
            if (units>71000&&units<85000) {  f_sav=1;  Serial.print("SQL "); UserID=2; bd_sql();}
            if (units>60000&&units<70000) {  f_sav=1;  Serial.print("SQL "); UserID=3; bd_sql();}
#ifdef pol_ves
            if (units>20000&&units<23000) {  f_sav=1;  Serial.print("SQL "); UserID=4; bd_sql();}
#endif
#ifdef stol_ves
           if (units>1400&&units<1900)   {  f_sav=1;  Serial.print("No SQL ");  UserID=4; /*bd_sql();*/}
         //   if (units>1700&&units<1900)   {  f_sav=1;  Serial.print("SQL ");  UserID=4; bd_sql();}
#endif
            
           
            if (!f_sav) {count_sav=0;}    //если не сработал не один вес то начинаем с начала
          }
        } 
    } 
    else   {count_sav=0; Serial.print("обнулить");}
        pred_uint=units;

  }  else {pred_uint=0; f_sav=0;count_sav=0;/*Serial.print("обнулить2");*/}

 } //if  (millis()-zader_vesi)
// Serial.println("");

#endif
}

void scale_istall(){
  //--------------------------------------------------------------------калибровка----------------------------------------------------
#ifdef kalib 
  Serial.println("Калибровка");
  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale();
  scale.tare();
  Serial.println("You have 10 seconds to set your known load");  delay(10000);  Serial.print("calibration factor: ");
  for (int i = 0; i < z; i++) {
    calibration_value[i] = scale.get_units(1) / (weight_of_standard / conversion_rate);
    calibration_factor += calibration_value[i];
  }
  calibration_factor = calibration_factor / z;
  Serial.println(calibration_factor);
#endif
 //---------------------------------------------------------------------рабочий режим-----------------------------------------------
#ifdef raboh
  Serial.print("Рабочий");
  scale.begin(DT_PIN, SCK_PIN);
  scale.power_up();
  scale.set_scale(calibration_factor);       //Применяем калибровку
  scale.tare();
  scale_ves();
  Serial.print("Ves: ");  Serial.println(units);
#endif
}
