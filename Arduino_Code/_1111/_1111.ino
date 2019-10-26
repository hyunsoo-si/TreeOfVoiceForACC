#include <SPI.h> // SPI.h constructs SPI object as extern SPIClass SPI
                // SPI.cpp defines SPI, and is included as part of the whole program. 

const int slaveSelectPin = 10;
//const int sendSize = 54;	// number of bytes for one slice signal = (number of nozzles -1) / 8
const int sendSize = 260 * 3; // 3 bytes for each of 260 LEDS
byte tempBuffer[SERIAL_RX_BUFFER_SIZE]; 
//SERIAL_RX_BUFFER_SIZE =defined to be 64 in C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino\HardwareSerial.h
byte sendBuffer[sendSize];

int m_index = sendSize - 1; // 

void setup() { 
	pinMode(slaveSelectPin, OUTPUT);
	
	SPI.begin(); // old version; 
	//Sets the SPI clock divider relative to the system clock. On AVR based boards, the dividers available are 2, 4, 8, 16, 32, 64 or 128. 
	//The default setting is SPI_CLOCK_DIV4, 
	//which sets the SPI clock to one-quarter the frequency of the system clock (4 Mhz for the boards at 16 MHz).

	SPI.setBitOrder(MSBFIRST);

	//https://stackoverflow.com/questions/27937916/whats-the-difference-between-com-usb-serial-port
	//(1) Serial port is a type of device that uses an UART chip, a Universal Asynchronous Receiver Transmitter. 
	//(2) COM comes from MS-Dos, it is a device name. Short for "COMmunication port". Computers in the 1980's usually had two serial ports,
	//labeled COM1 and COM2 on the back of the machine. 
	//This name was carried forward into Windows, most any driver that simulates a serial port will create a device with "COM" in its name. 

	//(3) RS-232 was an electrical signaling standard for serial ports. It is the simplest one with very low demands on the device, 
	// supporting just a point-to-point connection. RS-422 and RS-485 were not uncommon, 
	// using a twisted pair for each signal, providing much higher noise immunity and allowing multiple devices connected to each other.

	//(4) USB means Universal Serial Bus. 	//USB requires a complete software protocol stack.. Empowered by the ability to integrate a micro-processor into devices
	//that's a few millimeters in size and costs a few dimes. It replaced legacy devices in the latter 1990s. 
	//It is Universal because it can support many different kinds of devices, from coffee-pot warmers to disk drives to wifi adapters to audio playback. 
	//It is Serial, it only requires 4 wires. And it is a Bus, you can plug a USB device into an arbitrary port

	// (5) The only reason that serial ports are still relevant in on Windows these days is because a USB device requires a custom device driver.
	//Device manufacturers do not like writing and supporting drivers, they often take a shortcut in their driver that makes it emulate a legacy serial port device.
	//So programmers can use the legacy support for serial ports built into the operating system and about any language runtime library.
	//Rather imperfect support btw, these emulators never support plug-and-play well. Discovering the specific serial port to open is very difficult. 
	//And these drivers often misbehave in impossible to diagnose ways when you jerk a USB device while your program is using it.



	//Serial.begin(57600); // equals to the date rate set in  PC; it can go to nearly 500kbaud
	// serial Port of a PC uses UART communication protocol
	Serial.begin(115200);
     Serial.begin(230400); // higher than that, use SPI with 4MHz rate
	
	Reset();

	//
void Reset() {
	memset(sendBuffer, 0, sendSize); // set sendBUffer to zero for sendSize bytes

	//SPI.beginTransaction(SPISettings(12000000, MSBFIRST, SPI_MODE0));  // gain control of SPI bus

	digitalWrite(slaveSelectPin, LOW);

	SPI.transfer(sendBuffer, sendSize); // send all zero data to the device 
	//SPI uses synchronous communication, i.e. the device which originates traffic (the master) sends clock to the slave. 
	//As long as the clock rate is not higher then quarter of sampling frequency at the slave, any frequency can be used.
	// UART-to-SPI card: http://www.cypress.com/file/89161/download

	// Arduino as SPI slave: https://forum.arduino.cc/index.php?topic=52111.0
	// PC as a SPI Master: https://forums.ni.com/t5/LabVIEW/SPI-protocol-on-serial-port/td-p/9934
	//http://www.keterex.com/kxusb910h.php
	//https://www.robotshop.com/forum/how-do-i-make-a-uart-spi-converter-t6740
	digitalWrite(slaveSelectPin, HIGH);

	 // SPI.endTransaction();          // release the SPI bus

}

void loop() {
	
	int count = Serial.available(); 
	// get the number of bytes already received in the receive buffer of the serial port (<=SERIAL_RX_BUFFER_SIZE ( =64 bytes) ??)

	if (count == 0)
		return;

	// Read count bytes from the serail port buffer to a buffer; terminated if the determined number (count) is read or it times out
	// The timeout delay is set by Serial.setTimeout(); default to 1000 ms

	int readCount = Serial.readBytes(tempBuffer, count);

	for (int i = 0; i < count; ++i)
		sendBuffer[ m_index - i] = tempBuffer[i];

	m_index -= count;

	// The arduino has read the determined bytes count from the serial port from PC, and send it the device via SPI 
	if ( m_index == -1) {

	//Note: Best if all 3 settings are constants 
//SPISettings mySettting(speedMaximum, dataOrder, dataMode)
//Note: Best when any setting is a variable'' 
//Parameters
//speedMaximum: The maximum speed of communication. For a SPI chip rated up to 20 MHz, use 20000000. 
//dataOrder: MSBFIRST or LSBFIRST 
//dataMode : SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3 


	   // SPI.beginTransaction(SPISettings(12000000, MSBFIRST, SPI_MODE0));  // gain control of SPI bus

		digitalWrite(slaveSelectPin, LOW);

		//SPI.transfer(sendBuffer, sendSize); 
		// YOu do not specify SPISettings (which contains SPCR and SPSR)
		  // and do not call SPI.beginTransaction(spiSettings) => Problems may arise during SPI
		    // transmisstion

			//https://www.dorkbotpdx.org/blog/paul/spi_transactions_in_arduino
			//The clock speed you give to SPISettings is the maximum speed your SPI device can use, 
			//not the actual speed your Arduino compatible board can create.  
			
			//The SPISettings code automatically converts the max clock to the fastest clock your board can produce, 
			//which doesn't exceed the SPI device's capability.  As Arduino grows as a platform, onto more capable hardware, 
			//this approach is meant to allow SPI-based libraries to automatically use new faster SPI speeds.

		digitalWrite(slaveSelectPin, HIGH);

		 // SPI.endTransaction();          // release the SPI bus

		m_index = sendSize - 1; // init the index to the "last byte" (which is the first come)
	}//if (m_index == -1)
 else {
  // The buffer is not yet filled, so continue to read m_index bytes until the number of bytes in the buffer becomes 
  // count;
 }
}// void loop()



