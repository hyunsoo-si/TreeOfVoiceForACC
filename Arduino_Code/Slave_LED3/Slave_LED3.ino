#include <SPI.h>

//static const uint8_t SS   = PIN_SPI_SS;
//static const uint8_t MOSI = PIN_SPI_MOSI;
//static const uint8_t MISO = PIN_SPI_MISO;
//static const uint8_t SCK = PIN_SPI_SCK;

//Slave_LED3.ino uses MEGA
#include "SoftwareSerial.h"
#include "Adafruit_Pixie.h"

#define SS 53
#define NUMPIXELS3 10 // Number of Pixies in the strip

#define PIXIEPIN  6 // Pin number for SoftwareSerial output to the LED chain
SoftwareSerial pixieSerial(-1, PIXIEPIN);
Adafruit_Pixie strip = Adafruit_Pixie(NUMPIXELS3, &pixieSerial);

const int bufferSize = NUMPIXELS3 * 3;
byte buf[bufferSize];
volatile byte pos = 0;

volatile boolean process_LEDSignals = false;
 
void setup() {
  //Serial.begin(9600);
  // have to send on master in, *slave out*

  pixieSerial.begin(115200); // Pixie REQUIRES this baud rate
  //strip.setBrightness(200);  // Adjust as necessary to avoid blinding

  pinMode(SS, INPUT);

  // Master In, Slave Out
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  // SPI통신에서 슬레이브로 동작하도록 설정
  SPCR &= ~_BV(MSTR);
  // SPI 통신으로 문자가 수신될 경우 인터럽트 발생을 허용
  SPCR |= _BV(SPIE);
  
  // now turn on interrupts
//  SPI.attachInterrupt();
 // SPI.setClockDivider(SPI_CLOCK_DIV16);
 
  pinMode(PIXIEPIN, OUTPUT);
}
 
 
// SPI interrupt routine
ISR (SPI_STC_vect) {

  byte c = SPDR;  // grab byte from SPI Data Register


  Serial.println(c);

  if(pos < sizeof(buf)){
    buf[pos++]=c;
  }

  //check if the "show" command, which tells the LED signals so far arrived to be executed, arrived

  if ( pos == sizeof(buf) ) 
  //if( c == (byte)(255)  )
  {
    process_LEDSignals = true;
	Serial.println("show  com");
  }
}
 
void loop() {

  if(process_LEDSignals){

	// SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); // disable interrupt

    for(int i=0; i<NUMPIXELS3; i++) { //NUMPIXELS

      strip.setPixelColor (i, buf[i*3+0], buf[i*3+1], buf[i*3+2] );

      Serial.println( buf[i*3+0]);
      Serial.println( buf[i*3+1]);
      Serial.println( buf[i*3+2]);

      }

     strip.show(); // show command has been  recieved after all the LED signals per frame on the chain

    pos = 0;
    process_LEDSignals = false; 

	//SPI.endTransaction();
  } // if

 // delay(10);
}
