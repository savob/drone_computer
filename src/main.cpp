#include "main.h"
#include "RadioLib.h"

BMP280I2C BMP(BMPAddress);
// nRF24 has the following connections:
// CS pin:    10
// IRQ pin:   2
// CE pin:    3
nRF24 radio = new Module(10,2,3);

void setup()
{
  // Setup pins and turn on red led to indicate power
  setupMCUPins();
  digitalWrite(redLED, LEDOn);
  digitalWrite(greenLED1, LEDOff);
  digitalWrite(greenLED2, LEDOff);

  // Setup USB serial for debugging/config
  SerialUSB.begin(USBbaud);
  digitalWrite(greenLED1, LEDOn);

  BMP.start(BMPAddress); // Start barometer with the preconfigured address

  
  // Done setup
  digitalWrite(greenLED2, LEDOn);
}

void loop()
{
  BMP.updateReadings(); // Update readings of pressure periodically
  digitalWrite(greenLED1, LEDOn);
  digitalWrite(greenLED2, LEDOn);
  delay(200);
  digitalWrite(greenLED1, LEDOff);
  digitalWrite(greenLED2, LEDOff);
  delay(200);
  
  SerialUSB.println(BMP.readPresFloat());
  
}








void setupMCUPins() // Sets up pins based on purpose
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  // KILL SWITCH
  pinMode(killPin, INPUT_PULLUP);

  // Telemetry
  pinMode(teleIntPin, INPUT);
  pinMode(teleCEPin, OUTPUT);
  pinMode(teleCSpin, OUTPUT);

  // Battery Level
  pinMode(batteryPin, INPUT_ANALOG);
}