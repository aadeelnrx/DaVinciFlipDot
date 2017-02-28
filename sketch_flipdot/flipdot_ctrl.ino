// Basic functions to control the Flipdot display:
// Pixels, rectangles, circles, text, clear, fill

//--------------------------------------------------------------
// Set or clear a pixel at coordinates (col, row)
void setPixel(uint8_t col, uint8_t row, bool on)
{
  if ((col < 0) || (col > ((PANEL_WIDTH * PANEL_NUMBER) - 1)) || (row < 0) || (row > (PANEL_HEIGHT - 1))) return;
  if (pixels[col][row] == on) return;  //saving time on switching pixels already in the correct state
  pixels[col][row] = on;

  // Translate Column to Panel
  uint8_t panel = col/PANEL_WIDTH; 
  col = col - (panel * PANEL_WIDTH);
  // Translate pixel coordinates to row and column address
  // This is necessary because the FP2800A chip skips the
  // addresses 0, 8, 16, 24
  row = coord_to_row_col[row];
  col = coord_to_row_col[col];
  // Translate the panelnumber into correct bits
  panel = panel_to_bits[panel];
  
  // Set row address
  PORTA = (row & 0x1F);
  // Set column address
  PORTC = (col & 0x1F);
  // Set Data signal
  if (on)
    PORTC |= 0x20;
  else
    PORTC &= 0xDF;
    
  // Wait t_SE  
  delayMicroseconds(50);
  
  // Set Enable
  PORTL = (panel & 0xFF);  // Col_Enable panel x
  if (on)
  {
    PORTA |= 0x40;  // Row_Enable_on
//    PORTC |= 0x40;  // Col_Enable
  }
  else
  {
    PORTA |= 0x80;  // Row_Enable_off
//    PORTC |= 0x40;  // Col_Enable
  }
  
  // Give the dots time to flip
  delayMicroseconds(600);
    
  // Reset Data and Enable lines
  PORTA &= 0x1F;
  PORTC &= 0x1F;
  PORTL = 0x00;
  
  // Wait t_off
  delayMicroseconds(150);
}  


//--------------------------------------------------------
void clearDisplay()
{
  fillRect(0, 0, ((PANEL_WIDTH * PANEL_NUMBER) - 1) , (PANEL_HEIGHT - 1), false);
}


//--------------------------------------------------------
void fillDisplay()
{
  fillRect(0, 0, ((PANEL_WIDTH * PANEL_NUMBER) - 1) , (PANEL_HEIGHT - 1), true);
}


//--------------------------------------------------------
// Print a letter at (x, y) = top-left corner of the letter
// A letter takes up 5x7 pixels. 
// Return the width of the letter in pixels.
int printLetter5x7(char character, int x, int y)
{
  uint8_t* letter = font5x7[character - '!' + 1];
 
  // Go through the 5 columns of the letter
  for (int col = 0;  col < 5;  col++)
  {
    uint8_t bits = letter[col];
    // Go through the pixels of the letter
    for (int row = 6;  row >= 0;  row--)
    {
      uint8_t pixel = bits & 0x01;
      bits >>= 1;
      setPixel(x + col, 6 + y - row, pixel);
    }
  }
  return PIXEL_PER_CHAR_5x7;
}

void printString5x7(char string_buffer[100], uint16_t stringLength, int cursor_x, int cursor_y)
{
  for (int i = 1;
       i < min(stringLength + 1, 1 + PIXELS_WIDTH / PIXEL_PER_CHAR_5x7);
       i++)
      {
        cursor_x += printLetter5x7(string_buffer[i-1], cursor_x, cursor_y);
      }
}

//--------------------------------------------------------
// Print a letter at (x, y) = top-left corner of the letter
// A letter takes up 3x5 pixels.
int printLetter3x5(char character, int x, int y)
{
  // convert lower case to upper case
//  if ((character >= 'a')  && (character <= 'z'))
//  {
//    character -= 'a' + 'A';
//  }
//  character = toupper(character);
  uint8_t* letter = font3x5[character - '!' + 1];
    
  // Go through the 3 columns of the letter
  for (int col = 0;  col < 3;  col++)
  {
    uint8_t bits = letter[col];
    // Go through the pixels of the letter
    for (int row = 4;  row >= 0;  row--)
    {
      uint8_t pixel = bits & 0x01;
      bits >>= 1;
      setPixel(x + col, 4 + y - row, pixel);
    }
  }

  return PIXEL_PER_CHAR_3x5;
}

void printString3x5(char string_buffer[100], uint16_t stringLength, int cursor_x, int cursor_y)
{
  for (int i = 1;
       i < min(stringLength + 1, 1 + PIXELS_WIDTH / PIXEL_PER_CHAR_3x5);
       i++)
      {
        cursor_x += printLetter3x5(string_buffer[i-1], cursor_x, cursor_y);
      }
}


//--------------------------------------------------------
void fillRect(int x1, int y1, int x2, int y2, bool on)
{
  for (int y = y1;  y <= y2;  y++) 
    for (int x = x1;  x <= x2;  x++)
      setPixel(x, y, on);
}


//--------------------------------------------------------
void drawRect(int x1, int y1, int x2, int y2, bool on)
{
  for (int x = x1;  x <= x2;  x++) setPixel(x, y1, on);
  for (int x = x1;  x <= x2;  x++) setPixel(x, y2, on);
  for (int y = y1;  y <= y2;  y++) setPixel(x1, y, on);
  for (int y = y1;  y <= y2;  y++) setPixel(x2, y, on);
}  


//--------------------------------------------------------
void drawRoundedRect(int x1, int y1, int x2, int y2, bool on)
{
  for (int x = x1 + 1;  x <= x2 - 1;  x++) setPixel(x, y1, on);
  for (int x = x1 + 1;  x <= x2 - 1;  x++) setPixel(x, y2, on);
  for (int y = y1 + 1;  y <= y2 - 1;  y++) setPixel(x1, y, on);
  for (int y = y1 + 1;  y <= y2 - 1;  y++) setPixel(x2, y, on);
}  


//--------------------------------------------------------
void drawFilledCircle(int x, int y, bool on)
{
  setPixel(x + 1, y, on);
  setPixel(x + 2, y, on);
  setPixel(x + 3, y, on);
  for (int y1 = y + 1;  y1 < y + 4;  y1++)
    for (int x1 = x;  x1 < x + 5;  x1++)
      setPixel(x1, y1, on);
  setPixel(x + 1, y + 4, on);
  setPixel(x + 2, y + 4, on);
  setPixel(x + 3, y + 4, on);
}  



