// Functions to show Time, Temperature, Pressure, Humidity
// on screen


//-------------------------------------------------------------------------------------------
// Show Date and Time centralized on screen
void showDateTime(bool compressed)
{
  String dateString = buildDate();
  char dateChar[dateString.length() + 1];
  dateString.toCharArray(dateChar,dateString.length() + 1);
  if (compressed)
  {
//    printString3x5(dateChar, dateString.length(), (PIXELS_WIDTH - (dateString.length()*4) )/2, (PIXELS_HEIGHT - 15)/3 );
    printString3x5(dateChar, dateString.length(), (PIXELS_WIDTH - (dateString.length()*4) )/2, 1 );
  }else
  {
    printString5x7(dateChar, dateString.length(), (PIXELS_WIDTH - (dateString.length()*6) )/2, (PIXELS_HEIGHT - 15)/3 );    
  }

  String timeString = buildTime(true);
  char timeChar[timeString.length() + 1];
  timeString.toCharArray(timeChar,timeString.length() + 1);
//  printString5x7(timeChar, timeString.length(), (PIXELS_WIDTH - (timeString.length()*6))/2, (PIXELS_HEIGHT - 15)/3*2 + 8 );
  printString5x7(timeChar, timeString.length(), (PIXELS_WIDTH - (timeString.length()*6))/2, (PIXELS_HEIGHT/2) - 4 );
}

//-------------------------------------------------------------------------------------------
// Show Time centralized on screen
void showTime(bool compressed)
{
  String timeString = buildTime(!compressed);
  char timeChar[timeString.length() + 1];
  timeString.toCharArray(timeChar,timeString.length() + 1);
  if (compressed)
  {
 // printString3x5(timeChar, timeString.length(), (PIXELS_WIDTH - (timeString.length()*4))/2, (PIXELS_HEIGHT - 5)/2 );
    setPixel( 9, (PIXELS_HEIGHT - 5)/2 + 1, true);  // hour/min separator, upper dot
    setPixel( 9, (PIXELS_HEIGHT - 5)/2 + 3, true);  // hour/min separator, lower dot
    setPixel(19, (PIXELS_HEIGHT - 5)/2 + 1, true);  // min/sec separator, upper dot
    setPixel(19, (PIXELS_HEIGHT - 5)/2 + 3, true);  // min/sec separator, lower dot
    printString3x5(timeChar,     2,  1, (PIXELS_HEIGHT - 5)/2 );  // hour
    printString3x5(timeChar + 2, 2, 11, (PIXELS_HEIGHT - 5)/2 );  // minute
    printString3x5(timeChar + 4, 2, 21, (PIXELS_HEIGHT - 5)/2 );  // second
  }else
  {
    printString5x7(timeChar, timeString.length(), (PIXELS_WIDTH - (timeString.length()*6))/2, (PIXELS_HEIGHT - 7)/2 );    
  }
}

//-------------------------------------------------------------------------------------------
// Build Time String
String buildTime(bool seperator)
{
  DateTime now = rtc.now();

//  Serial1.print(now.hour(), DEC);
//  Serial1.print(':');
//  Serial1.print(now.minute(), DEC);
//  Serial1.print(':');
//  Serial1.print(now.second(), DEC);
//  Serial1.println();

  String timeString;
  DST = checkDST();
  if ((now.hour() + DST) < 10)
  {
    timeString += "0";
    timeString += String(now.hour() + DST);
  }else
  {
    timeString += String(now.hour() + DST);
  }
  if (seperator) timeString += String(":");
  
  if (now.minute() < 10)
  {
    timeString += "0";
    timeString += String(now.minute());
  }else
  {
    timeString += String(now.minute());
  }
  if (seperator) timeString += String(":");

  if (now.second() < 10)
  {
    timeString += "0";
    timeString += String(now.second());
  }else
  {
    timeString += String(now.second());
  }
  return timeString;
}

//-------------------------------------------------------------------------------------------
// Build Date String
String buildDate()
{
  DateTime now = rtc.now();
    
  Serial1.print(now.year(), DEC);
  Serial1.print('/');
  Serial1.print(now.month(), DEC);
  Serial1.print('/');
  Serial1.print(now.day(), DEC);
  Serial1.print(" (");
  Serial1.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial1.print(") ");
  Serial1.print(now.hour(), DEC);
  Serial1.print(':');
  Serial1.print(now.minute(), DEC);
  Serial1.print(':');
  Serial1.print(now.second(), DEC);
  Serial1.println();

  String dateString;
  dateString += daysOfTheWeek[now.dayOfTheWeek()];
  dateString += " ";
  if (now.day()<10){
    dateString += " ";    
  }
  dateString += String(now.day());
  dateString += " ";
  dateString += months[now.month()-1];
  dateString += " ";
  dateString += String(now.year());    
  Serial1.println(dateString);
  return dateString;
}


//-------------------------------------------------------------------------------------------
// Check Daylight Savings Time
bool checkDST()
{
  DateTime now = rtc.now();

  if (now.month()>3 && now.month()<10)
  {
    return true;
  }
  if (now.month() == 10 && now.day() < 25)
  {
    return true;
  }
  
//  if (now.dayOfTheWeek() == 7 && mo == 10 && d >= 25 && d <=31 && h == 3 && DST==1)
//  {
//    setclockto 2 am;
//    DST=0;
//  }
//
//  if (dow == 7 && mo == 3 && d >= 25 && d <=31 && h ==2 && DST==0)
//  {
//    setclockto 3 am;
//    DST=1;
//  }
  return false;
}

//-------------------------------------------------------------------------------------------
// Display Time, Temperature, Humidity, optimized for a single panel
void showTimeTempHum()
{
  char temp[30];
  char hum[30];
  String hhmm;

#ifdef DS18B20
  float tempF = ds18b20.getTempCByIndex(0);
  dtostrf(tempF, 4, 1, temp);
#else
  dtostrf(bme.readTemperature(), 4 , 1, temp);
#endif

  dtostrf(bme.readHumidity(), 2, 0, hum);
  hhmm = buildTime(true);
  Serial1.println(temp);
  Serial1.println(hum);
  Serial1.println(hhmm);

#ifdef ALEX
  // Temp, Big letters
  int y = 0;
  printLetter5x7(temp[0], 0, y);
  printLetter5x7(temp[1], 6, y);
  setPixel(12, y+6, true);
  printLetter5x7(temp[3], 14, y);

  setPixel(21, y+0, true);
  setPixel(22, y+0, true);
  setPixel(21, y+1, true);
  setPixel(22, y+1, true);
  printLetter3x5('C', 24, y);

  // Temp. Small letters
  y = 19;
  printLetter3x5(temp[0], 0, y);
  printLetter3x5(temp[1], 4, y);
  setPixel(8, y+4, true);
  printLetter3x5(temp[3], 10, y);
  setPixel(15, y+0, true);
  setPixel(16, y+0, true);
  setPixel(15, y+1, true);
  setPixel(16, y+1, true);
  printLetter3x5('C', 18, y);
#endif

  // Humidity, Big letters
  int y = 0;
  printLetter5x7(hum[0], 0, y);
  printLetter5x7(hum[1], 6, y);
  printLetter5x7('%', 13, y);

  // Humidity. Small letters
  y = 19;
  printLetter3x5(hum[0], 0, y);
  printLetter3x5(hum[1], 4, y);
  printLetter3x5('%', 9, y);
  
}

//-------------------------------------------------------------------------------------------
// Display Temperature and Humidity
void showTempHum()
{
  Serial1.print("Temperature = ");
  Serial1.print(bme.readTemperature());
  Serial1.println(" *C");

  Serial1.print("Pressure = ");

  Serial1.print(bme.readPressure() / 100.0F);
  Serial1.println(" hPa");

  Serial1.print("Approx. Altitude = ");
  Serial1.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial1.println(" m");

  Serial1.print("Humidity = ");
  Serial1.print(bme.readHumidity());
  Serial1.println(" %");

  Serial1.println();
  
  String tempHumString;
  char tempHumChar[28*3];

#ifdef RTC
  tempHumString += String(bme.readTemperature());
  tempHumString += "*C";

  tempHumString += String(bme.readHumidity());
  tempHumString += "%";

  tempHumString += String(bme.readPressure() / 100.0F);
  tempHumString += "hPa";

  tempHumString.toCharArray(tempHumChar,tempHumString.length() + 1);
  printString3x5(tempHumChar, tempHumString.length(), 1, 18 );     
#else
  tempHumString += "Temp = ";
  tempHumString += String(bme.readTemperature());
  tempHumString += " *C";
//  char tempHumChar[tempHumString.length() + 1];
  tempHumString.toCharArray(tempHumChar,tempHumString.length() + 1);
  printString3x5(tempHumChar, tempHumString.length(), 1, 1 );
  
  tempHumString = "";
  tempHumString += "Pres = ";
  tempHumString += String(bme.readPressure() / 100.0F);
  tempHumString += " hPa";
//  char tempHumChar[tempHumString.length() + 1];
  tempHumString.toCharArray(tempHumChar,tempHumString.length() + 1);
  printString3x5(tempHumChar, tempHumString.length(), 1, 7 );
  
  tempHumString = "";
  tempHumString += "Alt = ";
  tempHumString += String(bme.readAltitude(SEALEVELPRESSURE_HPA));
  tempHumString += " m";
//  char tempHumChar[tempHumString.length() + 1];
  tempHumString.toCharArray(tempHumChar,tempHumString.length() + 1);
  printString3x5(tempHumChar, tempHumString.length(), 1, 13 );
  
  tempHumString = "";
  tempHumString += "Hum = ";
  tempHumString += String(bme.readHumidity());
  tempHumString += " %";
//  char tempHumChar[tempHumString.length() + 1];
  tempHumString.toCharArray(tempHumChar,tempHumString.length() + 1);
  printString3x5(tempHumChar, tempHumString.length(), 1, 19 );
#endif 
}


