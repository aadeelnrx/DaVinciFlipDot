// Command-handling via Serial 1


//-------------------------------------------------------------------------------------------
// Process a command that was received in cmd_buffer and consists of
// comd_buffer_index characters
void handle_command()
{
  cmd_buffer[cmd_buffer_index] = '\0';  // make sure we have a null-byte

  //-----------------------------------------
  if (cmd_buffer[0] == '?')
  {
    printHelp();
  }

  //-----------------------------------------
  // 3<string>  = Text at the current cursor position, use 3x5 font
  // g0,0
  // 3ALEX
  else if (cmd_buffer[0] == '3')
  {
    Serial1.write("\nPrint Text 3x5\r\n");
    for (int i = 1;
         i < min(cmd_buffer_index, 1 + PIXELS_WIDTH / PIXEL_PER_CHAR_3x5);
         i++)
    {
      cursor_x += printLetter3x5(cmd_buffer[i], cursor_x, cursor_y);
    }
  }

  //-----------------------------------------
  // 5<string>  = Text at the current cursor position, use 5x7 font
  // g0,8
  // 5Alex
  else if (cmd_buffer[0] == '5')
  {
    Serial1.write("\nPrint Text\r\n");
    for (int i = 1;
         i < min(cmd_buffer_index, 1 + PIXELS_WIDTH / PIXEL_PER_CHAR_5x7);
         i++)
    {
      cursor_x += printLetter5x7(cmd_buffer[i], cursor_x, cursor_y);
    }
  }

  //-----------------------------------------
  // C  = Clear Display
  // g0,0
  // 5TEST
  // C
  else if (cmd_buffer[0] == 'C')
  {
    Serial1.write("Clear Display\r\n");
    clearDisplay();
    cursor_x = 0;
    cursor_y = 0;
  }

  //-----------------------------------------
  // d  = Print Dimensions  d<int>,<int>
  // d
  else if (cmd_buffer[0] == 'd')
  {
    Serial1.print("d");
    Serial1.print(PIXELS_WIDTH);
    Serial1.print(",");
    Serial1.println(PIXELS_HEIGHT);
  }

  //-----------------------------------------
  // f<x1>,<y1>,<x2>,<y2>  = Fill Rectangle Area
  // C
  // f4,4,15,15
  else if (cmd_buffer[0] == 'f')
  {
    int idx = 1;
    int x1 = readNumber(&idx);
    x1 = constrain(x1, 0, PIXELS_WIDTH - 1);
    idx++;  // to skip the ','
    int y1 = readNumber(&idx);
    y1 = constrain(y1, 0, PIXELS_HEIGHT - 1);
    idx++;  // to skip the ','
    int x2 = readNumber(&idx);
    x2 = constrain(x2, x1, PIXELS_WIDTH - 1);
    idx++;  // to skip the ','
    int y2 = readNumber(&idx);
    y2 = constrain(y2, y1, PIXELS_HEIGHT - 1);
    fillRect(x1, y1, x2, y2, true);
  }

  //-----------------------------------------
  // F<x1>,<y1>,<x2>,<y2>  = Erase Rectangle Area
  // C
  // f4,4,15,15
  // F4,4,15,15
  else if (cmd_buffer[0] == 'F')
  {
    int idx = 1;
    int x1 = readNumber(&idx);
    x1 = constrain(x1, 0, PIXELS_WIDTH - 1);
    idx++;  // to skip the ','
    int y1 = readNumber(&idx);
    y1 = constrain(y1, 0, PIXELS_HEIGHT - 1);
    idx++;  // to skip the ','
    int x2 = readNumber(&idx);
    x2 = constrain(x2, x1, PIXELS_WIDTH - 1);
    idx++;  // to skip the ','
    int y2 = readNumber(&idx);
    y2 = constrain(y2, y1, PIXELS_HEIGHT - 1);
    fillRect(x1, y1, x2, y2, false);
  }

  //-----------------------------------------
  // g<x>,<y>  = GotoXY
  // g8,10
  // 58,10
  // Invalid numbers result in 0
  // Too large or too small numbers are limited to the display size.
  else if (cmd_buffer[0] == 'g')
  {
    int idx = 1;
    int xpos = readNumber(&idx);
    idx++;  // to skip the ','
    int ypos = readNumber(&idx);
    cursor_x = constrain(xpos, 0, PIXELS_WIDTH - 1);
    cursor_y = constrain(ypos, 0, PIXELS_HEIGHT - 1);
    Serial1.print("Goto x = ");
    Serial1.print(cursor_x);
    Serial1.print(", y = ");
    Serial1.println(cursor_y);
  }

  //------------------------------------------
  // I  = Scan I2C bus and print the devices found
  else if (cmd_buffer[0] == 'I')
  {
    checkI2C();
  }

  //------------------------------------------
  // p<x>,<y>  = Set Pixel at x1,y1
  // C
  // p0,0
  // p12,12
  // p13,13
  // p14,14
  else if (cmd_buffer[0] == 'p')
  {
    int idx = 1;
    int xpos = readNumber(&idx);
    idx++;  // to skip the ','
    int ypos = readNumber(&idx);
    cursor_x = constrain(xpos, 0, PIXELS_WIDTH - 1);
    cursor_y = constrain(ypos, 0, PIXELS_HEIGHT - 1);
    setPixel(xpos, ypos, true);
  }

  //------------------------------------------
  // P<x>,<y>  = Clear Pixel at x1,y1
  // C
  // f0,0,20,20
  // P13,13
  // P0,0
  else if (cmd_buffer[0] == 'P')
  {
    int idx = 1;
    int xpos = readNumber(&idx);
    idx++;  // to skip the ','
    int ypos = readNumber(&idx);
    cursor_x = constrain(xpos, 0, PIXELS_WIDTH - 1);
    cursor_y = constrain(ypos, 0, PIXELS_HEIGHT - 1);
    setPixel(xpos, ypos, false);
  }

  //-----------------------------------------
  // r<x1>,<y1>,<x2>,<y2>  = Draw Rectangle (not filled)
  // C
  // r4,4,15,15
  else if (cmd_buffer[0] == 'r')
  {
    int idx = 1;
    int x1 = readNumber(&idx);
    x1 = constrain(x1, 0, PIXELS_WIDTH - 1);
    idx++;  // to skip the ','
    int y1 = readNumber(&idx);
    y1 = constrain(y1, 0, PIXELS_HEIGHT - 1);
    idx++;  // to skip the ','
    int x2 = readNumber(&idx);
    x2 = constrain(x2, x1, PIXELS_WIDTH - 1);
    idx++;  // to skip the ','
    int y2 = readNumber(&idx);
    y2 = constrain(y2, y1, PIXELS_HEIGHT - 1);
    drawRect(x1, y1, x2, y2, true);
  }

  //-----------------------------------------
  // R<x1>,<y1>,<x2>,<y2>  = Erase Rectangle (not filled)
  // C
  // f4,4,15,15
  // R5,5,14,14
  else if (cmd_buffer[0] == 'R')
  {
    int idx = 1;
    int x1 = readNumber(&idx);
    x1 = constrain(x1, 0, PIXELS_WIDTH - 1);
    idx++;  // to skip the ','
    int y1 = readNumber(&idx);
    y1 = constrain(y1, 0, PIXELS_HEIGHT - 1);
    idx++;  // to skip the ','
    int x2 = readNumber(&idx);
    x2 = constrain(x2, x1, PIXELS_WIDTH - 1);
    idx++;  // to skip the ','
    int y2 = readNumber(&idx);
    y2 = constrain(y2, y1, PIXELS_HEIGHT - 1);
    drawRect(x1, y1, x2, y2, false);
  }

  //------------------------------------------
  // s<fontID>  = Set Font ID (3 = 3x5, 5 = 5x7)
  // An illegal font-ID does not change the font-ID
  // C
  // s3
  // t3X5
  // g0,8
  // s5
  // t5x7
  else if (cmd_buffer[0] == 's')
  {
    int idx = 1;
    int id = readNumber(&idx);
    if ((id == 3) || (id == 5))
    {
      fontID = id;
    }
    // otherwise no change
  }

  //-----------------------------------------
  // t<string>  = Text at the current cursor position, use current font
  // C
  // s3
  // tALEX
  // s5
  // g5,10
  // talx
  else if (cmd_buffer[0] == 't')
  {
    for (int i = 1;
         ((i < cmd_buffer_index) && (cursor_x < PIXELS_WIDTH - PIXEL_PER_CHAR_3x5));
         i++)
    {
      if (fontID == 3)
      {
        cursor_x += printLetter3x5(cmd_buffer[i], cursor_x, cursor_y);

      }
      else if (fontID == 5)
      {
        cursor_x += printLetter5x7(cmd_buffer[i], cursor_x, cursor_y);
      }
    }
  }
  //----------------------------------------------
  // w  = Print DS18B20 Temperature at current position and font
  else if (cmd_buffer[0] == 'w')
  {
#ifdef DS18B20
    ds18b20.requestTemperatures();
    float temp = ds18b20.getTempCByIndex(0);
    Serial1.print("Temperature 18B20: ");
    Serial1.print(temp);
    Serial1.println(" *C");
#endif
#ifdef BME
    showTempHum();
#endif
  }
  else if (cmd_buffer[0] == 'W')
  {
    showTimeTempHum();
  }
  Serial1.println();
}


//--------------------------------------------------------------------------------------------
// Convert an ASCII-number into an int
int readNumber(int *index)
{
  int number = 0;
  while((*index < cmd_buffer_index)
     && (cmd_buffer[*index] >= '0') && (cmd_buffer[*index] <= '9'))
  {
    number = 10 * number + cmd_buffer[*index] - '0';
    (*index)++;
  }
 
  return number;
}


//-------------------------------------------------------------------------------------------
// Print command help
void printHelp()
 {
    Serial1.println("Commands:");
    Serial1.println("-----------------------------------------------------------------");
    Serial1.println("3<string>             = Text at the current cursor position, use 3x5 font");
    Serial1.println("5<string>             = Text at the current cursor position, use 5x7 font");
    Serial1.println("C                     = Clear Display");
    Serial1.println("d                     = Print Dimensions  d<int>,<int>");
    Serial1.println("f<x1>,<y1>,<x2>,<y2>  = Fill Rectangle Area");
    Serial1.println("F<x1>,<y1>,<x2>,<y2>  = Erase Rectangle Area");
    Serial1.println("g<x>,<y>              = GotoXY");
    Serial1.println("I                     = Scan I2C bus and print devices");
    Serial1.println("p<x>,<y>              = Set Pixel at x1,y1");
    Serial1.println("P<x>,<y>              = Clear Pixel at x1,y1");
    Serial1.println("r<x1>,<y1>,<x2>,<y2>  = Draw Rectangle (not filled)");
    Serial1.println("R<x1>,<y1>,<x2>,<y2>  = Erase Rectangle (not filled)");
    Serial1.println("s<fontID>             = Set Font ID (3 = 3x5, 5 = 5x7)");
    Serial1.println("t<string>             = Text at the current cursor position, use current font");
    Serial1.println("w                     = Print temperature at cursor position, use current font");
    Serial1.println();
}


