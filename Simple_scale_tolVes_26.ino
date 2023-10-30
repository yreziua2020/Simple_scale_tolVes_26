#include <ESP8266WiFi.h>              // Библиотека для создания Wi-Fi подключения (клиент или точка доступа)
#include <ESP8266WebServer.h>         // Библиотека для управления устройством по HTTP (например из браузера)
#include <ESP8266HTTPUpdateServer.h>  //не работает в хроме
#include <HX711.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

const byte  _Dclock = 0, _Dwifi = 0 ;

//#define  ip_zna  //без комента 100 наполььные  с коментом 101
//#define  kalib  //ели роз кометировать то калибруем

/*
NodeMCU    -> Matrix
MOSI-D7-GPIO13  -> DIN
CLK-D5-GPIO14   -> Clk
GPIO0-D8 pinCS  -> LOAD
*/
uint16_t UserID = 0;  //1  -сергей 2- юоий 3- мама
///-----------------------------------------------------------------------------------------------
///-----------------------------------------------------------------------------------------------
const char* DB_host = "dev.ibc-composite.com";  //host_name
const int Port = 80;
const char* database_pass = "Kvw1scUvg97Y";  //пароль пользователя базы данных
uint16_t f_eror_sql;                         //флаг ошибки sql
///-----------------------------------------------------------------------------------------------
///-----------------------------------------------------------------------------------------------
WiFiClient client;
#ifdef ip_zna
IPAddress ip(192, 168, 1, 100);
#else
IPAddress ip(192, 168, 1, 101);
#endif
IPAddress gateway(192, 168, 1, 41);
IPAddress primaryDNS(192, 168, 1, 41);  // опционально
IPAddress subnet(255, 255, 255, 0);
IPAddress secondaryDNS(8, 8, 8, 8);  // опционально
const char* ssid = "home";
const char* password = "30011980";
int raz_podk;  //сколько раз пробуем подключиться до прпезапуска
///-----------------------------------------------------------------------------------------------
///-----------------------------------------------------------------------------------------------
uint16_t m, h, s, dn;
//uint8_t kol_eror_ntp; //количество ошибочных ответов от сервера врмени чтобы не долбило постоянно и не зповисало все
unsigned int localPort = 2390;  // local port to listen for UDP packets
unsigned long ntp_time = 0;
unsigned long ntp_time2 = 0;
long t_correct = 0;
unsigned long cur_ms = 0;
unsigned long ms1 = 0;
unsigned long ms2 = 10000000UL;
unsigned long t_cur = 0;
bool points = true;          //для мигании точки я не использую
unsigned int err_count = 0;  //количество ошибочных ответов от сервера врмени чтобы не долбило постоянно и не зповисало все
#define TIMEZONE 2           //3 //2
// Для работы NTP
IPAddress timeServerIP;
//const char* ntpServerName = "132.163.96.6";
const char* ntpServerName = "62.149.0.30";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
WiFiUDP udp;
///-----------------------------------------------------------------------------------------------
///-----------------------------------------------------------------------------------------------
int pinCS = 15;                      //2 Подключение пина CS
int numberOfHorizontalDisplays = 4;  // Количество светодиодных матриц по Горизонтали
int numberOfVerticalDisplays = 1;    // Количество светодиодных матриц по Вертикали
//String decodedMsg;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
//#define MAX_DIGITS 16
//byte dig[MAX_DIGITS]={0};
//byte digold[MAX_DIGITS]={0};
//byte digtrans[MAX_DIGITS]={0};
///-----------------------------------------------------------------------------------------------
///-----------------------------------------------------------------------------------------------
//экземпляр весов и назначение пинов
HX711 scale;
const int DT_PIN = 4;   //12;  //D6
const int SCK_PIN = 5;  //14; //D5
#ifdef kalib
  //калибровочные дела (раскомментить для настройки)
float weight_of_standard = 2206.0;  // эталонный вес
float conversion_rate = 0.035274;   //коэффициент перевода из унций в граммы
const int z = 100;                  // количество измерений, по которым будет найдено среднее значение
float calibration_value[z];         // массив для хранения считанных значений
float calibration_factor = 0;
float units;
#else
  //float calibration_factor =0.7846986701;
#ifdef ip_zna
float calibration_factor = 0.80;  //для на польных -13.77
#else
float calibration_factor = -13.77;  //для на польных -13.77
#endif
float units;
#endif
///-----------------------------------------------------------------------------------------------
///-----------------------------------------------------------------------------------------------
const int Knop_PIN = 0;  //для кнопки
bool flag = false;
uint32_t btnTimer = 0;
///-----------------------------------------------------------------------------------------------
///-----------------------------------------------------------------------------------------------
#define OTAUSER "admin"      // Логин для входа в OTA
#define OTAPASSWORD "admin"  // Пароль для входа в ОТА
///-----------------------------------------------------------------------------------------------
ESP8266WebServer HTTP(80);
ESP8266HTTPUpdateServer httpUpdater;

//bool wrem;
uint32_t wrem_Timer = 0;
uint32_t clok_disp = 0;
uint32_t sql_sav = millis();  //задержка на запись

char name_all[][7] = { { "Serge" }, { "Yrij" }, { "Mami" }, { "Gant" }, { "Save" } };
int32_t pred_uint;              //предыдущие значения веса
uint16_t count_sav;             //счетчик до записи в SQL
bool f_sav;                     //флаг записи

int32_t clok_timer = millis();  //задержка для отображение часов
uint16_t z_clok;                  //  задержка отображение часов
uint16_t z_tara;                  //  задержка обнуленин тары
int32_t tara_timer = millis();  //задержка для отображение часов
bool f_clok_D;                  //флаг отображение часов на дисплее
bool f_sek;                  //флаг отображение двое точее мигания на дисплее


bool f_yark_d, f_yark_n;




void loop() {
  
   static unsigned long t_dht22 = millis();  //для отправки даных в sql

     if (!f_yark_n && (h>21 || h<6)){f_yark_d=0;f_yark_n=1;matrix.setIntensity(0); Serial.print("яркость 0");  }
    if (!f_yark_d && (h>=6 && h<=21)) {f_yark_n=0;f_yark_d=1;matrix.setIntensity(0); Serial.print("яркость 5");}        
    ///if (!f_yark_d && (h>6 || h<20)){f_yark_n=0;f_yark_d=1;matrix.setIntensity(6); Serial.print("яркость 5");}   
   
#ifdef ip_zna
   if (units > -100 && units < 100 ) {
#else 
  if (units > -10 && units < 10 )  {
#endif
       if ((z_clok >= 10) || ( (millis()-clok_timer) > 1000) ) {
       clok_timer = millis();
       if (z_clok <= 100) {  z_clok++;  } else { f_clok_D = 1; }

       z_tara=0;Serial.print("z_tara=0");
     }
   } else {
       if ( (millis()-tara_timer) > 1000)  
       {
          tara_timer = millis();
          Serial.print("z_tara=");Serial.println(z_tara);
          if (z_tara++>50)    
                {z_tara=0; 
                if (units <100) {scale.tare();  Serial.print("Тара обнулилась"); } 
                else {Serial.print("Тара обнуления не было");}
                }
       }  
     z_clok = 0;//f_clok_D = 0;
   }

  HTTP.handleClient();
  delay(10);
  scale_ves();
  Display();

  clok(); 
}
