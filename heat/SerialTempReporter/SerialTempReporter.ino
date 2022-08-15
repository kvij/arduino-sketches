// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Output for debugging
#define SERIAL_DEBUG_LOG false

#define serialPrint(msg) (SERIAL_DEBUG_LOG && Serial.print(msg))
#define serialPrintln(msg) (SERIAL_DEBUG_LOG && Serial.println(msg))

#define continueMainLoop delay(200); return

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

bool started = false;
float notifiedTempC;
const float precision = 0.5;
const int SERIAL_WRITE_BUFFER_SIZE = Serial.availableForWrite();
const int START_COMMAND = 's';

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  // start serial port
  Serial.begin(9600);

  // Start up the library
  sensors.begin();
}


uint8_t* floatToBytes(float toConvert) {
  typedef union
  {
    float number;
    uint8_t bytes[sizeof(float)];
  } FLOATUNION_t;
  
  auto const converter = new FLOATUNION_t;
  converter->number = toConvert;
  return converter->bytes;
}

bool inFlight() {
  return (Serial.availableForWrite() <= SERIAL_WRITE_BUFFER_SIZE - sizeof(float));
}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{  
  // Listen for START_COMMAND
  if (Serial.available() && Serial.read() == START_COMMAND) {
     notifiedTempC = 0; // Ensures that even if a stale temp is in the buffer a fresh one is also send
     started = true;
  } else if (!started) {
     continueMainLoop;
  }

  // Prevent buffer filling up when there are no listeners
  if (inFlight() && !SERIAL_DEBUG_LOG) {
    continueMainLoop;
  }

  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  serialPrint("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  serialPrintln("DONE");

  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    // Only report significant changes
    if ( (tempC < notifiedTempC + precision) && (tempC > notifiedTempC - precision) ) {
      continueMainLoop;
    }

    // Write the result
    serialPrint("Temperature for the device 1 (index 0) is: ");
    serialPrintln(tempC);
    Serial.write(floatToBytes(tempC), sizeof(float));
    notifiedTempC = tempC;
  } 
  else
  {
    serialPrintln("Error: Could not read temperature data");
  }
}
