//modified from dallas-temperature-library examples
//http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 12

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress insideThermometer, outsideThermometer;

int count=0;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Read all DS18b20 sensors and print their values and addresses every minute");

  // Start up the library
  sensors.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  count=sensors.getDeviceCount();
  Serial.print(count, DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  DeviceAddress deviceAddress;

  // set the resolution to 12 bit
  for (int i=0;i<count;i++) {
    if (!sensors.getAddress(deviceAddress, i)) {
      Serial.print("Unable to find address for Device "); 
      Serial.println(i);
    }
    sensors.setResolution(deviceAddress, TEMPERATURE_PRECISION);
    Serial.print("Device ");
    Serial.print(i);
    Serial.print(" Resolution: ");
    Serial.print(sensors.getResolution(deviceAddress), DEC); 
    Serial.println();
  }
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  DeviceAddress deviceAddress;
  float tempC;
  
  for (int i=0;i<count;i++) {
    if (!sensors.getAddress(deviceAddress, i)) {
      Serial.print("Unable to find address for Device "); 
      Serial.println(i);
    }
    tempC = sensors.getTempC(deviceAddress);
    // print the device information
    printAddress(deviceAddress);
    Serial.print(" Temp C: ");
    Serial.println(tempC,4);
  }

  delay(60000);
}

