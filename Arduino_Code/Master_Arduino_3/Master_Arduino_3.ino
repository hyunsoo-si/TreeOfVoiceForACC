/////////////////////////////////////////////////////////
// https://forum.arduino.cc/index.php?topic=558963.0
// Arduino UNO
// 10 (SS)
// 11 (MOSI)
// 12 (MISO)
// 13 (SCK)
//
// +5v(if required)
// GND(for signal return)
//
// Arduino Mega
// 53 (SS)
// 50 (MISO)
// 51 (MOSI)
// 52 (SCK)
// https://m.blog.naver.com/PostView.nhn?blogId=yuyyulee&logNo=220331139392&proxyReferer=https%3A%2F%2Fwww.google.com%2F
/////////////////////////////////////////////////////////
// In C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\src: SPI.h constructs SPI object as extern SPIClass SPI

// SPI.cpp defines SPI, and is included as part of the whole program. 
#include <SPI.h>

// The built-in pin number of the slave, which is used within SPI.Begin()
int ss1 = 53; 
int ss2 = 49;
int ss3 = 48; 
int ss4 = 47; 
//int ss5 = 46; 

// A total num of LED = 200; each slave processes 40 LEDs
const int NumPixels1 = 7;
const int NumPixles2 = 5;
const int NumPixels3 = 10;
const int NumPixels4 = 10;

const int group1ByteSize = NumPixels1 * 3;
const int group2ByteSize = NumPixles2 * 3;
const int group3ByteSize = NumPixels3 * 3;
const int group4ByteSize = NumPixels4 * 3;
//const int group5ByteSize = 40 * 3;

const int totalByteSize = group1ByteSize + group2ByteSize + group3ByteSize + group4ByteSize; // 3 bytes for each of 200 LEDS

byte recieveBuffer[SERIAL_RX_BUFFER_SIZE]; 
byte totalRecieveBuffer[totalByteSize]; 
// SERIAL_RX_BUFFER_SIZE == 64; 
// defined in C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino\HardWareSerial.h

byte m_showByte = 0;

int m_currentSize = 0;
int m_currentIndex =0;

void setup (void) {
	// set the Slave Select Pins as outputs:
  pinMode(ss1, OUTPUT);
  pinMode(ss2, OUTPUT);
  pinMode(ss3, OUTPUT);
  pinMode(ss4, OUTPUT);
  //pinMode(ss5, OUTPUT);

  digitalWrite(ss1, HIGH);
  digitalWrite(ss2, HIGH);
  digitalWrite(ss3, HIGH);
  digitalWrite(ss4, HIGH);
  //digitalWrite(ss5, HIGH);

  SPI.begin();
  //To condition the hardware you call SPI.begin () which configures the SPI pins (SCK, MOSI, SS) as outputs.
  //It sets SCK and MOSI low, and SS high. 
  //It then enables SPI mode with the hardware in "master" mode. This has the side-effect of setting MISO as an input.

  // Slow down the master a bit
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  // Sets the SPI clock divider relative to the system clock. 
  // On AVR based boards, the dividers available are 2, 4, 8, 16, 32, 64 or 128. 
	// The default setting is SPI_CLOCK_DIV4, 
	// which sets the SPI clock to one-quarter the frequency of the system clock (4 Mhz for the boards at 16 MHz).
  // SPI.setBitOrder(MSBFIRST);

  Serial.begin(9600);
}
 
// https://arduino.stackexchange.com/questions/8457/serial-read-vs-serial-readbytes
// readBytes() is blocking until the determined length has been read, or it times out (see Serial.setTimeout()).
// Where read() grabs what has come, if it has come in. Hence available is used to query if it has.
//
// This is why you see the Serial.read() inside a while or if Serial.available. 
// Hence I typically employ something like the following: Which emulates readBytes (for the most part).
//
//    #define TIMEOUT = 3000;
//    loop {
//        char inData[20];
//        unsigned long timeout = millis() + TIMEOUT;
//        uint8_t inIndex = 0;
//        while ( ((int32_t)(millis() - timeout) < 0) && (inIndex < (sizeof(inData)/sizeof(inData[0])))) {
//            if (Serial1.available() > 0) {
//                read the incoming byte:
//                inData[inIndex] = Serial.read();
//                if ((c == '\n') || (c == '\r')) {
//                    break;
//                }
//                Serial.write(inData[inIndex++]);
//            }
//        }
//    }
//
	//SO: I will stick with using readBytes() because it seems to produce consistent results 
	//and I can predict the number of bytes I should receive back. –

void loop (void) {	
	//int countToRead = Serial.available(); // get the number of bytes already received in the receive buffer of the serial port 
	//int HardwareSerial::available(void)
    //    {
    //    return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
    //    }
	// 0<= countToRead < SERIAL_RX_BUFFER_SIZE = 64; countToRead = 0 means  head == tail, that is,  when the buffer is empty or full							
	//int HardwareSerial::available(void)
	//{
	//	return   ( (unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail) ) % SERIAL_RX_BUFFER_SIZE;
	//}

	//if (countToRead == 0)
	//	return;

	// Read countToRead  bytes from the serail port buffer to a buffer; terminated if the determined number (count) is read or it times out
	// The timeout delay is set by Serial.setTimeout(); default to 1000 ms

	//int readCount = Serial.readBytes(receiveBuffer, countToRead); // read count bytes from the tail of the buffer; head == tail when the buffer is empty or full

	//int readCount = Serial.readBytes( recieveBuffer, totalByteSize);

	// terminates if length characters have been read or timeout (see setTimeout)
    // returns the number of characters placed in the buffer (0 means no valid data found)

	// Unless the timeout for reading bytes in the buffer does not happen,  readCount  equals to  totalByteSize

	//https://www.nutsvolts.com/magazine/article/july2011_smileysworkshop; 
	//UART uses a ring buffer where head index is incremented when a new byte is written into the buffer
	//https://arduino.stackexchange.com/questions/11710/does-data-coming-in-on-arduino-serial-port-store-for-some-time
	//What happens if the buffer is full and my PC writes an extra character? Does the PC block until there is buffer space, 
	//is an old character dropped or is the next character dropped? – Kolban Jun 19 '15 at 12:55
	//2. The next(incoming) character is dropped.– Majenko♦ Jun 19 '15 at 13:17
	// SUM: Yes. The receive ring buffer is 64 bytes and will discard anything past that until the program reads them out of the buffer.
	
	// check if the totalBytesSize has been read; that is check if time out occurred. If so, continue to read until the totalBytesSize
	// has been read.

	//while ( readCount <  totalByteSize) 
	//{ 
	//  int newReadCount =  Serial.readBytes( &recieveBuffer[readCount], totalByteSize - readCount);
	//  readCount += newReadCount; 
	//}

	//m_currentSize += readCount;

	//for (i=0; i < readCount; i++) {
	
	// totalReceiveBuffer[m_currentIndex + i ]= ReceiveBuffer[i];

	 //}

	// m_currentIndex += readCount;


	// If the arduino has read the total number of bytes for LED data from the serial port from PC , send them the master device via SPI 
	//if ( m_currentSize == totalByteSize) {

	
		// SPISettings (which contains SPCR and SPSR)
		 //SPISettings mySettting(speedMaximum, dataOrder, dataMode)
  
       //Parameters
          //speedMaximum: The maximum speed of communication. For a SPI chip rated up to 20 MHz, use 20,000000. 
		  //Arduino will automatically use the best speed that is equal to or less than the number you use with SPISettings. 

       //dataOrder: MSBFIRST or LSBFIRST : Byte transfer from the most significant bit (MSB) Transfer?
	
       //dataMode : SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3 

		  //  call SPI.beginTransaction(spiSettings) 
	
	// set random color values to totalRecieveBuffer
	//https://gamedev.stackexchange.com/questions/32681/random-number-hlsl

  //for test LED
	for (int i = 0; i < totalByteSize/3; i++) {
		totalRecieveBuffer[3 * i] = (byte) random(10, 255); // from 10 to 254
		totalRecieveBuffer[3 * i +1] = (byte)random(10, 255);
		totalRecieveBuffer[3 * i +2] = (byte)random(10, 255);

	}
	  
	  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	

			//https://www.dorkbotpdx.org/blog/paul/spi_transactions_in_arduino
			//The clock speed you give to SPISettings is the maximum speed your SPI device can use, 
			//not the actual speed your Arduino compatible board can create.  
			
		//The SPISettings code automatically converts the max clock to the fastest clock your board can produce, 
			//which doesn't exceed the SPI device's capability.  As Arduino grows as a platform, onto more capable hardware, 
			//this approach is meant to allow SPI-based libraries to automatically use new faster SPI speeds.

	
	   // send the first group of data to the first slave:

	   

      digitalWrite(ss1, LOW); // select the first SS line
      digitalWrite(ss2, HIGH);
      digitalWrite(ss3, HIGH);
      digitalWrite(ss4, HIGH);
      //digitalWrite(ss5, HIGH);

      SPI.transfer( &totalRecieveBuffer[0], group1ByteSize);
      digitalWrite(ss1, HIGH);

	  SPI.endTransaction();
	  
	   // send the second group of data to the second slave:
	  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	   
      digitalWrite(ss1, HIGH);
      digitalWrite(ss2, LOW); // select the second SS Line
      digitalWrite(ss3, HIGH);
      digitalWrite(ss4, HIGH);
      //digitalWrite(ss5, HIGH);

	    SPI.transfer( &totalRecieveBuffer[group1ByteSize], group2ByteSize);
      digitalWrite(ss2, HIGH);

	  SPI.endTransaction();

	    // send the third group of data to the third slave:
	  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	   
      digitalWrite(ss1, HIGH);
      digitalWrite(ss2, HIGH);
      digitalWrite(ss3, LOW); // select the third SS line
      digitalWrite(ss4, HIGH);
      //digitalWrite(ss5, HIGH);
      
	    SPI.transfer( &totalRecieveBuffer[group1ByteSize + group2ByteSize], group3ByteSize);   
      digitalWrite(ss3, HIGH);

	  SPI.endTransaction();

	    // send the fourth group of data to the fourth slave:
	  //On Mega, default speed is 4 MHz (SPI clock divisor at 4). Max is 8 MHz (SPI clock divisor at 2).
	  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));

      digitalWrite(ss1, HIGH);
      digitalWrite(ss2, HIGH);
      digitalWrite(ss3, HIGH);
      digitalWrite(ss4, LOW);   // select the fourth SS line
      //digitalWrite(ss5, HIGH);

      SPI.transfer( &totalRecieveBuffer[group1ByteSize + group2ByteSize  + group3ByteSize ], group4ByteSize);
      digitalWrite(ss4, HIGH);

	  SPI.endTransaction();
	   
	    // send the fifth group of data to the fifth slave: 
   //   digitalWrite(ss1, HIGH);
   //   digitalWrite(ss2, HIGH);
   //   digitalWrite(ss3, HIGH);
   //   digitalWrite(ss4, HIGH);
   //   //digitalWrite(ss5, LOW);

	  //SPI.transfer( &totalRecieveBuffer[ group1ByteSize + group2ByteSize  + group3ByteSize  + group4ByteSize ], 
	  //              group5ByteSize);
   //   digitalWrite(ss5, HIGH);

	  //SPI.endTransaction();          // release the SPI bus

     // If other libraries use SPI from interrupts, 
	 // they will be prevented from accessing SPI until you call SPI.endTransaction(). 

	  m_currentSize = 0; // init the index to the "last byte" (which is the first come)
	  m_currentIndex =0;

	  // send "show" command to all the slaves: // "-1"


	 // send show to the first slave:
   SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
   digitalWrite(ss1, LOW);
   digitalWrite(ss2, HIGH);
   digitalWrite(ss3, HIGH);
   digitalWrite(ss4, HIGH);
   //digitalWrite(ss5, HIGH);
   
   SPI.transfer(m_showByte);
   Serial.println(m_showByte+"1"); //for test
   digitalWrite(ss1, HIGH);
   SPI.endTransaction(); 


	 // send show to the second slave:
   SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
   digitalWrite(ss1, HIGH);
   digitalWrite(ss2, LOW);
   digitalWrite(ss3, HIGH);
   digitalWrite(ss4, HIGH);
   //digitalWrite(ss5, HIGH);
   
   SPI.transfer( m_showByte);
   Serial.println(m_showByte+"2"); //for test
   digitalWrite(ss2, HIGH);
   SPI.endTransaction(); 
   

   // send show to the third slave:
   SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
   digitalWrite(ss1, HIGH);
   digitalWrite(ss2, HIGH);
   digitalWrite(ss3, LOW);
   digitalWrite(ss4, HIGH);
   //digitalWrite(ss5, HIGH);
   
   SPI.transfer( m_showByte);
   Serial.println(m_showByte+"3"); //for test
   digitalWrite(ss3, HIGH);
   SPI.endTransaction();
   

	 // send show to the fourth slave:
   SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
   digitalWrite(ss1, HIGH);
   digitalWrite(ss2, HIGH);
   digitalWrite(ss3, HIGH);
   digitalWrite(ss4, LOW);
   //digitalWrite(ss5, HIGH);
   
   SPI.transfer( m_showByte );
   Serial.println(m_showByte+"4"); //for test
   digitalWrite(ss4, HIGH);
   SPI.endTransaction();
	

   // send show to the fifth slave:
//   SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
//   digitalWrite(ss1, HIGH);
//   digitalWrite(ss2, HIGH);
//   digitalWrite(ss3, HIGH);
//   digitalWrite(ss4, HIGH);
//   digitalWrite(ss5, LOW);
//   
//   SPI.transfer( m_showByte);
//   Serial.println(m_showByte+"5"); //for test
//   digitalWrite(ss5, HIGH);
//   SPI.endTransaction();

	  delay (10); // delay between LED activation; at least 1 ms

	  // write back the received bytes for testing
	// Serial.write(totalRecieveBuffer, totalByteSize);
	  //If the transmit buffer is full then Serial.write() will block until there is enough space in the buffer. 
	  //To avoid blocking calls to Serial.write(), you can first check the amount of free space in the transmit buffer using availableForWrite().
	//}//if (m_currentSize == totalByteSize)

 //else {
 // // The buffer is not yet filled, so continue to read in the next iteration of loop()
 //}


 // delay (10);
}
