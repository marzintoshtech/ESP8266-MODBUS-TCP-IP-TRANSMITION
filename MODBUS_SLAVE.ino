#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2 // DS18B20 on NodeMCU pin D4 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
//*********************NETWORK PARAMETERS CONFIGURATION*********************
#define INDIRIZZO_IP 1 //INDIRIZZO_IP=0 PLK LAN, INDIRIZZO_IP = 1 MY LAN
//WIFI Settings
#if INDIRIZZO_IP == 0
//byte ip[]      = { 10, 117, 40, 250};
//byte gateway[] = { 10, 117, 40, 254};
//byte subnet[]  = { 255, 255, 255, 0 };
const char* ssid = "MANUTENZIONE";
const char* password = "Pilkington2016";
#else
//byte ip[]      = { 192, 168, 43, 250};
//byte gateway[] = { 192, 168, 43, 1};
//byte subnet[]  = { 255, 255, 255, 0 };
const char* ssid = "IOT1";
const char* password = "memristore1979";
#endif
int ModbusTCP_port = 502;
//----------------------------"DIGITAL INPUT & VARIABLES CONFIGURATION"----------------------------
int const PreseceBT = 5;  //24Vdc power supply presece OK
int const ServerConnected = 16; //Client Modbus connected to server
int const WifiConnected = 15; //Client Modbus connected to server

//-------------------------------------------------------------------------------------------------

//----------------------------"MODBUS PARAMETERS CONFIGURATION"----------------------------
// Required for Modbus TCP / IP
#define maxInputRegister 20
#define maxHoldingRegister 20
//
#define MB_FC_NONE 0
#define MB_FC_READ_REGISTERS 3 //implemented
#define MB_FC_WRITE_REGISTER 6 //implemented
#define MB_FC_WRITE_MULTIPLE_REGISTERS 16 //implemented
//
// MODBUS Error Codes
//
#define MB_EC_NONE 0
#define MB_EC_ILLEGAL_FUNCTION 1
#define MB_EC_ILLEGAL_DATA_ADDRESS 2
#define MB_EC_ILLEGAL_DATA_VALUE 3
#define MB_EC_SLAVE_DEVICE_FAILURE 4
//
// MODBUS MBAP offsets
//
#define MB_TCP_TID 0
#define MB_TCP_PID 2
#define MB_TCP_LEN 4
#define MB_TCP_UID 6
#define MB_TCP_FUNC 7
#define MB_TCP_REGISTER_START 8
#define MB_TCP_REGISTER_NUMBER 10

//-----------------------------------------------------------------------------------------------------

//----------------------------"VARIUS CONSTANTS & VARIABLES CONFIGURATION"----------------------------
byte ByteArray[460];
unsigned int MBHoldingRegister[maxHoldingRegister];
unsigned int MBInputRegister[maxInputRegister];
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int TempBuffer1 = 0;
int TempBuffer2 = 0;
int TempBuffer3 = 0;
int TempBuffer4 = 0;
int TempBuffer5 = 0;
int TempBuffer6 = 0;
int TempBuffer7 = 0;
unsigned long currentMillis = 0;
unsigned long Thread0 = 0; //will store last time
//-----------------------------------------------------------------------------------------------------
WiFiServer MBServer(ModbusTCP_port);

void setup() {
  pinMode(ServerConnected, OUTPUT);
  pinMode(WifiConnected, OUTPUT);
  pinMode(PreseceBT, OUTPUT);

  Serial.begin(9600);
  delay(100) ;
  WiFi.begin(ssid, password);
  IPAddress ip(192, 168,  1, 222);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);
  delay(100) ;
  Serial.println(".");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  MBServer.begin();
  Serial.println("Connected ");
  Serial.print("ESP8266 Slave Modbus TCP/IP ");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(String(ModbusTCP_port));
  Serial.println("Modbus TCP/IP Online");

}


void loop() {
  //-----------------------Thread Modbus Transmition-----------------------
  currentMillis = millis();
  if (currentMillis - Thread0 > 1000 && WiFi.status() == WL_CONNECTED) {
    digitalWrite(WifiConnected, HIGH);
    TxRx();
    Thread0 = currentMillis; //save the last time
  }
  else {
    digitalWrite(WifiConnected, LOW);
  }
}
