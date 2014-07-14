
/*  -------------------------------------------------------------------------------------------
    Output temp & humidity to LCD screen and 10 seconds later Light levels
    By MM with a little help
    -------------------------------------------------------------------------------------------
  
LiquidCrystal Library 
The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 DHT11 Library
 The circuit:
 * DHT11 Pin 1 to Ground
 * DHT11 Pin 2 to digital 7
 * DHT11 Pin 3 to +5V
 
 TSL2561 Library
 The Circuit:
 * TSL2561 pin Vin to 3.3v
 * TSL2561 pin Grd to ground
 * TSL2561 pin SDA to Analogue 4
 * TSL2561 pin SCL to Analogue 5
 
 */

// include the library code:
#include <LiquidCrystal.h>
#include <dht11.h>
#include <TSL2561.h>
#include <Wire.h>

/*-----( Declare objects )-----*/
//! Temp & Humidity sensor configured.
dht11 DHT11;

//! Light sensor configured to default i2c address.
TSL2561 tsl(TSL2561_ADDR_FLOAT); 
//! Buffer for printing light readings to the LCD.
char output_buffer[6];

/*-----( Declare Constants, Pin Numbers )-----*/
#define DHT11PIN 7

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  
  Serial.begin(9600);
 
// Initialize the LCD's number of columns and rows: 
// set the cursor to column 0, line 1
// (note: line 1 is the second row, since counting begins with 0):
    lcd.begin(16, 2);
    lcd.setCursor(0, 1);
    
 // Initialize the DHT11 Temp and Humidity sensor. ----- Change this later for DHT11
  if (tsl.begin()) {
    Serial.println(F("DHT11 Sensor initialized"));
  } else {
    Serial.println(F("DHT11 Sensor failed to initialize"));
  }

// Initialize the TSL2561 light sensor.
  if (tsl.begin()) {
    Serial.println(F("TSL2561 Sensor initialized"));
  } else {
    Serial.println(F("TSL2561 Sensor failed to initialize"));
  }

// Measuring relatively bright light, so gain of 0 is reasonable
   tsl.setGain(TSL2561_GAIN_0X);
// Measuring relatively bright light, so short sampling time is reasonable
   tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);
  
// For low light level, comment out the above configuration and uncomment the below lines
// tsl.setGain(TSL2561_GAIN_16X);
// tsl.setTiming(TSL2561_INTEGRATIONTIME_402MS);


}

void loop() {

  Serial.println("Reading DHT11 sensor:--------------------------------------- ");

// Read DHT11 sensor and report back if fault 
  int chk = DHT11.read(DHT11PIN);
  switch (chk)
  {
    case 0: lcd.println("OK"); break;
    case -1: lcd.println("Checksum error"); break;
    case -2: lcd.println("Time out error"); break;
    default: lcd.println("Unknown error"); break;
  }

 //Display Humidity on LCD (Line 0) and Serial port
  lcd.setCursor(0, 0);
  lcd.print("Humid (%): ");
  lcd.println((float)DHT11.humidity, 2);
  Serial.print("Humid (%): ");
  Serial.println((float)DHT11.humidity);
  
 //Display Temperature in Degrees C on LCD (Line 1)and Serial port
  lcd.setCursor(0, 1);
  lcd.print("Temp degC: ");
  lcd.println((float)DHT11.temperature, 2);
  Serial.print("Temp degC: ");
  Serial.println((float)DHT11.temperature);

// Delay screen refresh 
  delay (10000);
   
 //Display Temperature in Degrees F on Serial port - LCD (Line 0)

  lcd.setCursor(0, 0);
  lcd.print("Temp degF: ");
  lcd.println(Fahrenheit(DHT11.temperature), 2);
  Serial.print("Temperature (oF): ");
  Serial.println(Fahrenheit(DHT11.temperature), 2);

//Display Temperature in Degrees K on Serial port - LCD (Line 1)
  lcd.setCursor(0, 1);
  lcd.print("Temp degK: ");
  lcd.println(Kelvin(DHT11.temperature), 2);
  Serial.print("Temperature (K): ");
  Serial.println(Kelvin(DHT11.temperature), 2);

// Delay screen refresh 
  delay (10000);
 
//Display Dewpoint Degrees C on Serial port - LCD (Line 0)
  lcd.setCursor(0, 0);
  lcd.print("Dew(oC):    ");
  lcd.println(dewPoint(DHT11.temperature, DHT11.humidity), 2);
  Serial.print("Dew Point (oC): ");
  Serial.println(dewPoint(DHT11.temperature, DHT11.humidity));
  
//Display Dewpoint Fast Degrees C on Serial port - LCD (Line 1)
  lcd.setCursor(0, 1);
  lcd.print("DewFast(oC):");
  lcd.println(dewPointFast(DHT11.temperature, DHT11.humidity), 2);
  Serial.print("Dew PointFast (oC): ");
  Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));

// Delay screen refresh 
  delay (10000);
 

// From the Adafruit TSL2561 Example sketch, get both IR and
// full spectrum values in one read.
 Serial.println("Reading TLS2561 sensor: ------------------------------------");
 
  uint32_t full_luminosity = tsl.getFullLuminosity();
  uint16_t ir_spectrum = full_luminosity >> 16;
  uint16_t full_spectrum = full_luminosity & 0xFFFF;

//Display Full Light Lux on LCD and Serial port
  lcd.setCursor(0, 0);
  lcd.print("FullLight:");
  lcd.println((float)full_luminosity, 2);
  Serial.print("Full Light: ");
  Serial.println((float)full_luminosity);
 
//Display InfraRed Light levels on LCD and Serial port
  lcd.setCursor(0, 1);
  lcd.print("IR Light :  ");
  lcd.println((float)ir_spectrum, 2);
  Serial.print("IR Light: ");
  Serial.println((float)ir_spectrum);
  
// Delay screen refresh  
  delay (10000);



}
//-----(end main loop )-----------------------

//-----( Declare User-written Functions )-----

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
	return 1.8 * celsius + 32;
}

//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
	return celsius + 273.15;
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm 
double dewPoint(double celsius, double humidity)
{
	double A0= 373.15/(273.15 + celsius);
	double SUM = -7.90298 * (A0-1);
	SUM += 5.02808 * log10(A0);
	SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
	SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
	SUM += log10(1013.246);
	double VP = pow(10, SUM-3) * humidity;
	double T = log(VP/0.61078);   // temp var
	return (241.88 * T) / (17.558-T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity/100);
	double Td = (b * temp) / (a - temp);
	return Td;
}
