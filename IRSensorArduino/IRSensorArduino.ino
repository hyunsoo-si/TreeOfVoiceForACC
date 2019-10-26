
				
#include <SharpIR.h>

//20 to 150 cm GP2Y0A02YK0F use 20150
SharpIR sensor1( SharpIR::GP2Y0A02YK0F, A0 );
SharpIR sensor2( SharpIR::GP2Y0A02YK0F, A1 );



int m_distances[2];

byte m_byteArray[4]; // an int is a 16 bit; two int's are 4 bytes

unsigned long m_lastTime = 0;
unsigned long m_currentTime = 0;
unsigned  long m_deltaTime = 20;


const int totalByteSize = 3; // 3 bytes with one byte for each IR sensor

byte WriteBuffer[SERIAL_TX_BUFFER_SIZE]; 

//#define SERIAL_TX_BUFFER_SIZE 64 //defined in C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino\HardWareSerial.h
//// SERIAL_RX_BUFFER_SIZE== 64; 

byte totalWriteBuffer[ totalByteSize]; 

void  computeAvgDist(int distances[]);


void setup () {

m_lastTime  = millis();

//''[Time]
//Description
//Returns the number of milliseconds passed since the Arduino board began running the current program. 
//This number will overflow (go back to zero), after approximately 50 days.


}
 
 //
void loop() {

	computeAvgDist( m_distances ); // compute the average distances of the two sensors during m_deltaTime


	m_lastTime = millis();


	// convert an integer into a byte array with four elements


	m_byteArray[0] = (byte) (m_distances[0] >> 8);
	m_byteArray[1] = (byte)m_distances[0];
	m_byteArray[2] = (byte) (m_distances[1] >> 8);
	m_byteArray[3] = (byte)m_distances[1];




	//long dist; most significant, first in array:
	//byte[] b = new byte[]  {
	//      (byte) dist >> 24, 
	//       (byte) (dist >> 16,,
	//      (byte) (dist >> 8),
	 //      (byte) (dist >> 0),
	 //     }

	// long dist = 
	 //       ( (int) b[3] & 0xff) << 0
	 //      | ((long) b[2] & 0xff) << 8
	 //      | ((long) b[1] & 0xff) << 16
	  //     | ((long) b[0] & 0xff) << 24;


	//Serial.write(buf, len);Writes binary data to the serial port. This data is sent as a byte or series of bytes
	//Get the number of bytes (characters) available for writing in the serial buffer without blocking the write operation.


	//https://forum.arduino.cc/index.php?topic=509527.0:
	//You can indeed use AvailableForWrite to stop the sampling if there is not enough space in the buffer.

	int countToWrite;

	countToWrite = Serial.availableForWrite(); // get the number of bytes available for writing 

	if (countToWrite == 0)
		return;

	// Write countToWrite  bytes from the serail port buffer to a buffer; terminated if the determined number (count) is written
	//  it times out
	// The timeout delay is set by Serial.setTimeout(); default to 1000 ms

	//If there is no room in the buffer, the function blocks until there is room. On 1.0, the buffer size is 64 bytes. 

	int byteLength = 8;
	int writeCount = Serial.write(m_byteArray, 8);

	// terminates if length characters have been read or timeout (see setTimeout)
	// returns the number of characters placed in the buffer (0 means no valid data found)

	// Unless the timeout for reading bytes in the buffer does not happen, writeCount is equal to byteLength;

	if (writeCount < byteLength) {

		int newWriteCount = Serial.write(&m_byteArray[writeCount], byteLength - writeCount);
		writeCount += newWriteCount;
	}

} // loop()

	//https://www.nutsvolts.com/magazine/article/july2011_smileysworkshop; 
	//UART uses a ring buffer where head index is incremented when a new byte is written into the buffer
	//https://arduino.stackexchange.com/questions/11710/does-data-coming-in-on-arduino-serial-port-store-for-some-time
	//What happens if the buffer is full and my PC writes an extra character? Does the PC block until there is buffer space, 
	//is an old character dropped or is the next character dropped? – Kolban Jun 19 '15 at 12:55
	//2. The next(incoming) character is dropped.– Majenko♦ Jun 19 '15 at 13:17
	// SUM: Yes. The receive ring buffer is 64 bytes and will discard anything past that until the program reads them out of the buffer.
	
	
	//The type of a in int a[10] is int[10]. What you can say is arrays "decay" into pointers to their first element.
	//(This is an implicit array-to-pointer conversion.) 



//int fillarr(int arr[])
//Is kind of just syntactic sugar. You could really replace it with this and it would still work:

//int fillarr(int* arr)

void  computeAvgDist( int distances[] )
{

 
//uint8_t getDistance( bool avoidBurstRead = true ) => read immediately 

// int dis1=sensor1.getDistance(false);  => delay 20 milliseconds to read the sensor

  
  // Sharp IR code for Robojax.com 20181201
  int numReadings = 0;
  
  int total1 = 0;
  int total2 = 0;

  int average1 = 0;
  int average2 = 0;

  m_currentTime = millis();

  while ( m_currentTime < m_lastTime + m_deltaTime )
  
    { 
	  // accumulate the distances to compute the average distance
	  int dist1, dist2;
	  dist1=sensor1.getDistance();  // this returns the distance for sensor 1
      dist2=sensor2.getDistance();  // this returns the distance for sensor 2

      total1 = total1 + dist1;
      total2= total2  + dist2;
	  numReadings += 1;

	  m_currentTime = millis();
	 }
 // update the last time

  m_lastTime =  millis(); // set the current time as the last time for the future


		   
	average1=total1/ numReadings;
    average2=total2/ numReadings;
	
  distances[0] = average1;
  distances[1] = average2;



  Serial.print("Distance (1): ");
  Serial.print(average1);
  Serial.println("cm");
  
  Serial.print("Distance (2): ");
  Serial.print(average2);
  Serial.println("cm");
     // Sharp IR code for Robojax.com


}//void computeAvgDist()


