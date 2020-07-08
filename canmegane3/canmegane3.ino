#include <SPI.h>
#include <mcp_can.h>
#include "ssd1306.h"
#include "ssd1306_console.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10

bool engineStarted = false; 
short engineRPM = 0x0000;
byte flashState = 0x00;
byte engineState = 0x00;

const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN); // Set CS pin
Ssd1306Console  console;

void prntex(const char ch[])
{
  Serial.println(ch);  
  console.print(ch); 
  console.print("\n"); 
}

void setup()
{
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  Serial.begin(115200);
  ssd1306_128x64_i2c_init();
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_fillScreen( 0x00 );
  
  while (CAN_OK != CAN.begin(CAN_500KBPS, MCP_8MHz)) // init can bus : baudrate = 500k
  { 
    prntex("CAN BUS Shield init fail");
    delay(100);
  }
  prntex("CAN BUS OK!");

}

//0x5DE = Свет и др.
//<DataItem Name="PositionLightsDisplay" Ref="1" FirstByte="1" BitOffset="5"/>
//<DataItem Name="LowBeamDisplay" Ref="1" FirstByte="1" BitOffset="6"/>
//<DataItem Name="HighBeamDisplay" Ref="1" FirstByte="1" BitOffset="7"/>
//0x186 = Обороты двиг. 0,1 байт 
// unsigned int result = temp[3] | (temp[2] << 8)
//0x212 = тоже вероятно двигатель. 
//<DataItem Name="RearGearEngaged" Ref="1" FirstByte="2" BitOffset="2"/> <DataItem Name="AlternatorLoad" Ref="1" FirstByte="1"/>     

void ProcessCanPackage()
{
  unsigned char len = 0;
  unsigned char buf[8];
  unsigned long canId = 0;
  CAN.readMsgBufID(&canId, &len, buf); // read data, len: data length, buf: data buf
  //len = 8;
  //canId = 0x212;
  //buf[0]=107;
  //buf[1]=25;

  /*Serial.println("-----------------------------");
  Serial.print("Get data from ID: ");
  Serial.println(canId, HEX);

  for(int i = 0; i<len; i++) // print the data
  {
    Serial.print(buf[i], HEX);
    console.print(buf[i], HEX); 
    Serial.print(" ");
  }
  Serial.println();
*/
  if ( canId == 0x5DE)
  {
    flashState = buf[0];
    Serial.print("flashState: ");
    Serial.println(flashState, BIN);
  }
  else if ( canId == 0x186)
  {
    engineRPM = buf[1]| (buf[0] << 8);
    //Serial.print("engineRPM: ");
    //Serial.println(engineRPM, HEX);
  }
  else if ( canId == 0x212)
  {
    engineState = buf[1]; 
    Serial.print("engineState: ");
    Serial.println(engineState, BIN);
  }
}


void ProcessAlgoritm()
{
  if (engineRPM > 0)
  {.0
    if (flashState==B100){  
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW); 
    Serial.println("ENABLE");
    }
    else
    {
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH); 
    Serial.println("DISABLE");
    }
  }
  else
  {
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    Serial.println("DISABLE");
  };     
}

void loop()
{
 
  if(CAN_MSGAVAIL == CAN.checkReceive()) // check if data coming
  {
    ProcessCanPackage();   
    ProcessAlgoritm(); 
  }
}

void loop_()
{
  unsigned char len = 0;
  unsigned char buf[8];
  unsigned long canId;

  if(CAN_MSGAVAIL == CAN.checkReceive()) // check if data coming
  {
    CAN.readMsgBufID(&canId, &len, buf);
    Serial.println("-----------------------------");
    Serial.print("Get data from ID: ");
    Serial.println(canId, HEX);

    //console.print("ID: ");
    //console.print(canId, HEX);
    //console.print("\n"); 

  for(int i = 0; i<len; i++) // print the data
  {
    Serial.print(buf[i], HEX);
    //console.print(buf[i], HEX); 
    Serial.print(" ");
  }
    Serial.println();
  }
}
