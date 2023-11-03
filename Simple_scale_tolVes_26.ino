#include <ESP8266WiFi.h>              // Библиотека для создания Wi-Fi подключения (клиент или точка доступа)
#include <ESP8266WebServer.h>         // Библиотека для управления устройством по HTTP (например из браузера)
#include <ESP8266HTTPUpdateServer.h>  //не работает в хроме
#include <HX711.h>
//#include <SPI.h>
#include <Adafruit_GFX.h>
//#include <Max72xxPanel.h>

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "Fonts.h"

//#include "disp2.h"

#define  MAX_DEVICES 4 
#define CLK_PIN     D5 // or SCK
#define DATA_PIN    D7 // or MOSI
#define CS_PIN      D8 // or SS

MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
#define ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))

String Text;
char buf[256];
int disp=0;
int rnd;
int lp=0;

//test
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
//Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

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
//-------------------------------------------------------------------
// Global data
typedef struct
{
  textEffect_t  effect;   // text effect to display
  char *        psz;      // text string nul terminated
  uint16_t      speed;    // speed multiplier of library default
  uint16_t      pause;    // pause multiplier for library default
} sCatalog;

sCatalog  catalog[] =
{
  { PA_SLICE, "SLICE", 1, 1 },
  { PA_MESH, "MESH", 10, 1 },
  { PA_FADE, "FADE", 20, 1 },
  { PA_WIPE, "WIPE", 5, 1 },
  { PA_WIPE_CURSOR, "WPE_C", 4, 1 },
  { PA_OPENING, "OPEN", 3, 1 },
  { PA_OPENING_CURSOR, "OPN_C", 4, 1 },
  { PA_CLOSING, "CLOSE", 3, 1 },
  { PA_CLOSING_CURSOR, "CLS_C", 4, 1 },
  { PA_BLINDS, "BLIND", 7, 1 },
  { PA_DISSOLVE, "DSLVE", 7, 1 },
  { PA_SCROLL_UP, "SC_U", 5, 1 },
  { PA_SCROLL_DOWN, "SC_D", 5, 1 },
  { PA_SCROLL_LEFT, "SC_L", 5, 1 },
  { PA_SCROLL_RIGHT, "SC_R", 5, 1 },
  { PA_SCROLL_UP_LEFT, "SC_UL", 7, 1 },
  { PA_SCROLL_UP_RIGHT, "SC_UR", 7, 1 },
  { PA_SCROLL_DOWN_LEFT, "SC_DL", 7, 1 },
  { PA_SCROLL_DOWN_RIGHT, "SC_DR", 7, 1 },
  { PA_SCAN_HORIZ, "SCANH", 4, 1 },
  { PA_SCAN_VERT, "SCANV", 3, 1 },
  { PA_GROW_UP, "GRW_U", 7, 1 },
  { PA_GROW_DOWN, "GRW_D", 7, 1 },
};

//-------------------------------------------------------------------------------------------------------------------------------

//bool wrem;
uint32_t wrem_Timer = 0;
uint32_t clok_disp = 0;
uint32_t sql_sav = millis();  //задержка на запись

char name_all[][7] = { { "Serge" }, { "Yrij" }, { "Mami" }, { "Akum" }, { "Save" } };
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
  
   //static unsigned long t_dht22 = millis();  //для отправки даных в sql

   //  if (!f_yark_n && (h>21 || h<6)){f_yark_d=0;f_yark_n=1;matrix.setIntensity(0); Serial.print("яркость 0");  }
  //  if (!f_yark_d && (h>=6 && h<=21)) {f_yark_n=0;f_yark_d=6;matrix.setIntensity(0); Serial.print("яркость 5");}        
    ///if (!f_yark_d && (h>6 || h<20)){f_yark_n=0;f_yark_d=1;matrix.setIntensity(6); Serial.print("яркость 5");}   
   
#ifdef ip_zna
  if (units > -100 && units < 100 ) 
  {
#else 
  if (units > -10 && units < 10 )  
  {
#endif
      if ((millis()-clok_timer) > 8000)  { clok_timer = millis();  P.displayClear();  f_clok_D = 1; /*Serial.print("f_clok_D=1");*/}
      tara_timer = millis();
   } 
   else
   {
       if ( (millis()-tara_timer) > 20000)  
       {
          tara_timer = millis();
          //Serial.print("z_tara=");Serial.println(z_tara);
          if (units <100) {scale.tare();  Serial.print("Тара обнулилась"); } 
          else {Serial.print("Тара обнуления не было");}
         
       }  
     f_clok_D = 0;
     clok_timer = millis(); 
   }

  HTTP.handleClient();
  delay(10);
  scale_ves();
  Display();

  clok(); 
}
