// Test program for Flip Dot display LAWO, 28x24 pixels, with
// 3 chips FP2800A: one for the columns, two for the rows
//
// Connection to Arduino MEGA 1280
// PA --- row
// PC --- columns
// PL --- Panel selection
// 
// Arduino MEGA 1280 | Flip Dot Display
// Port Name | Pin-# | Pin-# | Pin-# | Signal FP2800A
//           |       |  old  |  new  |
// ----------+-------|-----------------------
//   PA0     |   22  |    9  |   17  | Row_A0
//   PA1     |   23  |   10  |   19  | Row_A1
//   PA2     |   24  |   11  |   21  | Row_A2
//   PA3     |   25  |   12  |   23  | Row_B0
//   PA4     |   26  |   13  |   25  | Row_B1
//   PA5     |  n.c. |                
//   PA6     |   28  |   22  |   10  | Row_Enable_on 
//   PA7     |   29  |   23  |   12  | Row_Enable_off
//   PC0     |   37  |    4  |    7  | Col_A0
//   PC1     |   36  |    5  |    9  | Col_A1
//   PC2     |   35  |    6  |   11  | Col_A2
//   PC3     |   34  |    7  |   13  | Col_B0
//   PC4     |   33  |    8  |   15  | Col_B1
//   PC5     |   32  |   21  |    8  | Col_Data
//   PC6     |  n.c.                  
//   PC7     |  n.c.                  
//   PL0     |   49  |   31  |   28  | Col_Enable Panel 1
//   PL1     |   48  |   30  |   26  | Col_Enable Panel 2
//   PL2     |   47  |   29  |   24  | Col_Enable Panel 3
//   PL3     |   46  |   28  |   22  | Col_Enable Panel 4
//   PL4     |   45  |   27  |   20  | Col_Enable Panel 5
//   PL5     |   44  |   26  |   18  | Col_Enable Panel 6
//   PL6     |   43  |   25  |   16  | Col_Enable Panel 7
//   PL7     |   42  |   24  |   14  | Col_Enable Panel 8
//           |       | 15,32 | 29,30 | VS ------------ 10 V DC
//           |       | 16,17 | 31,33 | GND------------ GND
//           |       | 33,34 | 32,34 | GND------------ GND
//    5V     |       |  3,20 |  5,6  | Vcc
//   GND     |       |  1,2  |  1,3  | GND
//   GND     |       | 18,19 |  2,4  | GND
//
//   BME280 connected to pin 43/44, digital in/out 20/21, the I2C bus
//   DS3231 connected to pin 43/44, digital in/out 20/21, the I2C bus
//

#define BME
#define RTC
#define DS18B20

#include <stdint.h>
#include "fonts.h"

//---------------------------------------------------------------------------
// BME280 Libraries
// Requires:  Adafruit Unified Driver, Adafruit BME280
#include <Wire.h>
#include <SPI.h>
#include <Metro.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// BME280 Definitions
#define BME280_ADDRESS  (0x76)
//#define BME_SCK 13
//#define BME_MISO 12
//#define BME_MOSI 11
//#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

//---------------------------------------------------------------------------
// DS3231 Definitions
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
// Requires: Adafruit RTClib
#include "RTClib.h"
RTC_DS3231 rtc;

//char daysOfTheWeek[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//char months[12][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
char daysOfTheWeek[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

//---------------------------------------------------------------------------
// DS18B20 Temperature sensor
// Requires:  OneWire by Jim Studt
// Requires:  DallasTemperature
#ifdef DS18B20
#include <OneWire.h> 
#include <DallasTemperature.h>

// DS1820B data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 6

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature ds18b20(&oneWire);
#endif

//------------------------------------------------------------------------------
// Panel Definitions
#define PANEL_WIDTH     28 // single Panel width in pixel
#define PANEL_HEIGHT    24 // single Panel height in pixel
#define PANEL_NUMBER     1 // Number of connected Panels
#define PIXELS_WIDTH (PANEL_WIDTH * PANEL_NUMBER)
#define PIXELS_HEIGHT PANEL_HEIGHT

#define LED_PIN         13
#define CLOCK_INTERVAL     1000 // mills between clock uodates
#define BME_INTERVAL      60000 // mills between BME updates
#define WIFI_INTERVAL        10 // mills between WIFI updates


//#define TEST2_ALL_ON_OFF
//#define TEST3_RANDOM_PIXELS
//#define TEST4_BOUNCING_BALL
//#define TEST6_SIERPINSKY
//#define TEST7_LETTERS
//#define TEST8_LAPCOUNTER
#define SERIAL_CMD

// Timers that control the update rates of various things in the main loop:
// Units are milliseconds.
// Install from zip file:
// https://github.com/thomasfredericks/Metro-Arduino-Wiring/archive/master.zip
Metro clock_timer = Metro(CLOCK_INTERVAL);
Metro bme_timer = Metro(BME_INTERVAL);
Metro wifi_timer = Metro(WIFI_INTERVAL);

char pixels[PANEL_WIDTH*PANEL_NUMBER][PANEL_HEIGHT];
int px, py;
int dx, dy;
int oldx, oldy;
#define PIXEL_PER_CHAR_5x7 6
#define PIXEL_PER_CHAR_3x5 4
uint8_t coord_to_row_col[28] = {1,2,3,4,5,6,7,9,10,11,12,13,14,15,17,18,19,20,21,22,23,25,26,27,28,29,30,31};
uint8_t panel_to_bits[8] = {1,2,4,8,16,32,64,128};

int cmd_buffer_max = 100;
int cmd_buffer_index = 0;
char cmd_buffer[100];
int cursor_x = 0;  // x-coordinate to where the next command prints
int cursor_y = 0;  // y-coordinate to where the next command prints
int fontID = 5;    // Font to be used.  3 = 3x5, 5 = 5x7




//--------------------------------------------------------
//--------------------------------------------------------
void setup()
{
  // Port A, C & L = output
  DDRA = 0xFF;
  DDRC = 0xFF;
  DDRL = 0xFF;
  // Set all pins to LOW
  PORTA = 0x00;
  PORTC = 0x00;
  PORTL = 0x00;
  
  // Required for the various effects
  dx = 1;
  dy = 1;
  px = 0;
  py = 2;
  oldx = 0;
  oldy = 0;

  // initialize the digital pin as an output.
  pinMode(LED_PIN, OUTPUT);     

  // Serial 1 is the command port
  Serial1.begin(115200);
  
  // Fill the display
  Serial1.write("Fill Display\r\n");
  fillDisplay();
  // Clear the display
  Serial1.write("Clear Display\r\n");
  clearDisplay();
  
#ifdef BME
  if (!bme.begin(BME280_ADDRESS)) {
    Serial1.println("Could not find a valid BME280 sensor, check wiring!");
//    fillDisplay();
    checkI2C();
    while (1);
  }
#endif

#ifdef RTC
  if (! rtc.begin()) {
    Serial1.println("Couldn't find RTC");
//    fillDisplay();
    checkI2C();
    while (1);
  }

  if (rtc.lostPower()) {
    Serial1.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
#endif

#ifdef DS18B20
  // DS18B20
  ds18b20.begin();
#endif

  Serial1.write("Ready.\r\n");
  
#ifdef TEST9_SERIAL
  printHelp();
#endif

  // Reset metro timers so we start at 0
  clock_timer.reset();
  bme_timer.reset();
  wifi_timer.reset();

}


//--------------------------------------------------------
//-------------------------------------------------
// Main loop
void loop()
{
  // put your main code here, to run repeatedly:

#ifdef RTC
  if (clock_timer.check())
  {
    if (PANEL_NUMBER > 3)
    {
      showDateTime(false);
    }else if (PANEL_NUMBER > 2)
    {
      showDateTime(true);
    }else if (PANEL_NUMBER > 1)
    { 
      showTime(false);    
    }else
    {
      showTime(true); 
    }
  }
#endif

#ifdef BME
  if (bme_timer.check())
  {
//    clearDisplay();
    showTempHum();
  }
#endif

#ifdef TEST1_PIXEL_ON_OFF
test1_pixel_on_off();
#endif

#ifdef TEST2_ALL_ON_OFF
test2_all_on_off();
#endif

#ifdef TEST3_RANDOM_PIXELS
  setPixel(random(0, (PANEL_WIDTH * PANEL_NUMBER)), random(0, PANEL_HEIGHT), random(0, 2));
#endif

#ifdef TEST4_BOUNCING_BALL
test4_bouncing_ball();
#endif

#ifdef TEST5_PONG 
 pong();
#endif

#ifdef TEST6_SIERPINSKY
 sier();
 while(1); 
#endif

#ifdef TEST7_LETTERS
test7_letters();
#endif

#ifdef TEST8_LAPCOUNTER
test8_lapcounter();
#endif


#ifdef SERIAL_CMD
  while (Serial1.available())
  {
    cmd_buffer[cmd_buffer_index] = Serial1.read();

    // Sometimes we get a null-byte --> ignore it
    if (cmd_buffer[cmd_buffer_index] == 0)
    {
      break;
    }

    // Empty line?
    if (((cmd_buffer[cmd_buffer_index] == '\n') || (cmd_buffer[cmd_buffer_index] == '\r'))
       && (cmd_buffer_index == 0))
    {
      // nothing, ignore empty line
    }
    // Command complete?
    else if ((cmd_buffer[cmd_buffer_index] == '\n') || (cmd_buffer[cmd_buffer_index] == '\r'))
    {
      handle_command();

      cmd_buffer_index = 0;
    }
    // Command not complete
    else
    {
      if (cmd_buffer_index < cmd_buffer_max - 1)
      {
        cmd_buffer_index++;
      }
    }
  } // while Serial1.available()
#endif

}


//----------------------------------------------------------------------
// Scan the I2C bus and print the devices that were found
void checkI2C()
{
  byte error, address;
  char addressChar[5];
  String addressString;
  int nDevices;
     
  Serial1.println("Scanning...");
     
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
     
    if (error == 0)
    {
      Serial1.print("I2C device found at address 0x");
      if (address<16)
        Serial1.print("0");
      Serial1.print(address,HEX);
      Serial1.println("  !");
//      addressString = (String) address;
//      addressString.toCharArray(addressChar,addressString.length()+1);
//      printString5x7(addressChar, addressString.length(), 1, (nDevices*8) );
     
      nDevices++;
    }
    else if (error==4)
    {
      Serial1.print("Unknow error at address 0x");
      if (address<16)
        Serial1.print("0");
      Serial1.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial1.println("No I2C devices found\n");
  else
    Serial1.println("done\n");
   
  //delay(5000);           // wait 5 seconds for next scan
}



