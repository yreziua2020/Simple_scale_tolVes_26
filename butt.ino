 void Button_cl(){
  bool btnState = !digitalRead(Knop_PIN);
  if (btnState && !flag && millis() - btnTimer > 30) {
    flag = true;
    btnTimer = millis();
    Serial.println("press");

    if (units>86000&&units<110000){    Serial.print("SQL "); UserID=1; bd_sql();}
    if (units>71000&&units<85000) {    Serial.print("SQL "); UserID=2; bd_sql();}
    if (units>60000&&units<70000) {    Serial.print("SQL "); UserID=3; bd_sql();}
  }
  if (btnState && flag && millis() - btnTimer > 500) {
    btnTimer = millis();
    scale.tare();
    Serial.println("press hold");
  }
  if (!btnState && flag && millis() - btnTimer > 500) {
    flag = false;
    btnTimer = millis();
    //Serial.println("release");
  }

 }//функция Button_cl(
