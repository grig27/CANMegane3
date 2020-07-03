#include <SPI.h>
#include <mcp_can.h>
#include "ssd1306.h"
#include "ssd1306_console.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10

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

void loop()
{
  unsigned char len = 0;
  unsigned char buf[8];
  unsigned long canId = 0;
  if(CAN_MSGAVAIL == CAN.checkReceive()) // check if data coming
  {
    CAN.readMsgBufID(&canId, &len, buf); // read data, len: data length, buf: data buf

    //unsigned int canId = CAN.getCanId();
   if ( canId == 0x5DE)
   {
    console.print(buf[0], HEX); 
   }
  }
}

void loop_()
{
  unsigned char len = 0;
  unsigned char buf[8];
  unsigned long canId;

  if(CAN_MSGAVAIL == CAN.checkReceive()) // check if data coming
  {
    //CAN.readMsgBuf(&len, buf); // read data, len: data length, buf: data buf

    //unsigned long canId = CAN.getCanId();

    CAN.readMsgBufID(&canId, &len, buf);

    Serial.println("-----------------------------");
    Serial.print("Get data from ID: ");
    Serial.println(canId, HEX);

    console.print("ID: ");
    console.print(canId, HEX);
    console.print("\n"); 

  for(int i = 0; i<len; i++) // print the data
  {
    Serial.print(buf[i], HEX);
    console.print(buf[i], HEX); 
    Serial.print("\t");
  }
    Serial.println();
  }
}
