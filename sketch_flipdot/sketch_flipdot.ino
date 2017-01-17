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

#define PANEL_WIDTH     28 // single Panel width in pixel
#define PANEL_HEIGHT    24 // single Panel height in pixel
#define PANEL_NUMBER     3 // Number of connected Panels

//#define TEST2_ALL_ON_OFF
//#define TEST3_RANDOM_PIXELS
//#define TEST4_BOUNCING_BALL
//#define TEST6_SIERPINSKY
//#define TEST7_LETTERS
#define TEST8_LAPCOUNTER

char pixels[PANEL_WIDTH*PANEL_NUMBER][PANEL_HEIGHT];
int px, py;
int dx, dy;
int oldx, oldy;
uint8_t coord_to_row_col[28] = {1,2,3,4,5,6,7,9,10,11,12,13,14,15,17,18,19,20,21,22,23,25,26,27,28,29,30,31};
uint8_t panel_to_bits[8] = {1,2,4,8,16,32,64,128};
uint8_t font5x7[][5] = {
                     {0x3E, 0x45, 0x49, 0x51, 0x3E},  // 0
                     {0x00, 0x21, 0x7F, 0x01, 0x00},  // 1
                     {0x23, 0x45, 0x49, 0x49, 0x31},  // 2
                     {0x22, 0x49, 0x49, 0x49, 0x36},  // 3
                     {0x0C, 0x14, 0x24, 0x7F, 0x04},  // 4
                     {0x72, 0x51, 0x51, 0x51, 0x4E},  // 5
                     {0x1E, 0x29, 0x49, 0x49, 0x06},  // 6
                     {0x40, 0x47, 0x48, 0x50, 0x60},  // 7
                     {0x36, 0x49, 0x49, 0x49, 0x36},  // 8
                     {0x30, 0x49, 0x49, 0x4A, 0x3C},  // 9
                     {0x00, 0x36, 0x36, 0x00, 0x00},  // :
                     {0x00, 0x6D, 0x6E, 0x00, 0x00},  // ;
                     {0x08, 0x14, 0x22, 0x41, 0x00},  // <
                     {0x14, 0x14, 0x14, 0x14, 0x14},  // =
                     {0x00, 0x41, 0x22, 0x14, 0x08},  // >
                     {0x20, 0x40, 0x45, 0x48, 0x30},  // ?
                     {0x3E, 0x41, 0x79, 0x49, 0x38},  // @
                     {0x3F, 0x48, 0x48, 0x48, 0x3F},  // A
                     {0x7F, 0x49, 0x49, 0x49, 0x36},  // B
                     {0x3E, 0x41, 0x41, 0x41, 0x22},  // C
                     {0x7F, 0x41, 0x41, 0x41, 0x3E},  // D
                     {0x7F, 0x49, 0x49, 0x49, 0x41},  // E
                     {0x7F, 0x48, 0x48, 0x48, 0x40},  // F
                     {0x3E, 0x41, 0x45, 0x45, 0x27},  // G
                     {0x7F, 0x08, 0x08, 0x08, 0x7F},  // H
                     {0x00, 0x41, 0x7F, 0x41, 0x00},  // I
                     {0x02, 0x01, 0x01, 0x01, 0x7E},  // J
                     {0x7F, 0x08, 0x14, 0x22, 0x41},  // K
                     {0x7F, 0x01, 0x01, 0x01, 0x01},  // L
                     {0x7F, 0x20, 0x18, 0x20, 0x7F},  // M
                     {0x7F, 0x10, 0x08, 0x04, 0x7F},  // N
                     {0x3E, 0x41, 0x41, 0x41, 0x3E},  // O
                     {0x7F, 0x48, 0x48, 0x48, 0x30},  // P
                     {0x3E, 0x41, 0x45, 0x42, 0x3D},  // Q
                     {0x7F, 0x48, 0x4C, 0x4A, 0x31},  // R
                     {0x32, 0x49, 0x49, 0x49, 0x26},  // S
                     {0x40, 0x40, 0x7F, 0x40, 0x40},  // T
                     {0x7E, 0x01, 0x01, 0x01, 0x7E},  // U
                     {0x70, 0x0C, 0x03, 0x0C, 0x70},  // V
                     {0x7F, 0x02, 0x0C, 0x02, 0x7F},  // W
                     {0x63, 0x14, 0x08, 0x14, 0x63},  // X
                     {0x60, 0x10, 0x0F, 0x10, 0x60},  // Y
                     {0x43, 0x45, 0x49, 0x51, 0x61},  // Z
                     {0x7D, 0x00, 0x00, 0x00, 0x00},  // [  but shows !
};
uint8_t font3x5[][3] = {
                     {0x45, 0x49, 0x51},  // 0
                     {0x09, 0x7F, 0x01},  // 1
                     {0x17, 0x15, 0x1D},  // 2
                     {0x49, 0x49, 0x49},  // 3
                     {0x14, 0x24, 0x7F},  // 4
                     {0x51, 0x51, 0x51},  // 5
                     {0x29, 0x49, 0x49},  // 6
                     {0x47, 0x48, 0x50},  // 7
                     {0x49, 0x49, 0x49},  // 8
                     {0x49, 0x49, 0x4A},  // 9
                     {0x1B, 0x1B, 0x00},  // :
                     {0x6D, 0x6E, 0x00},  // ;
                     {0x14, 0x22, 0x41},  // <
                     {0x14, 0x14, 0x14},  // =
                     {0x41, 0x22, 0x14},  // >
                     {0x40, 0x45, 0x48},  // ?
                     {0x41, 0x79, 0x49},  // @
                     {0x1F, 0x14, 0x1F},  // A
                     {0x49, 0x49, 0x49},  // B
                     {0x41, 0x41, 0x41},  // C
                     {0x41, 0x41, 0x41},  // D
                     {0x49, 0x49, 0x49},  // E
                     {0x48, 0x48, 0x48},  // F
                     {0x41, 0x45, 0x45},  // G
                     {0x08, 0x08, 0x08},  // H
                     {0x41, 0x7F, 0x41},  // I
                     {0x01, 0x01, 0x01},  // J
                     {0x08, 0x14, 0x22},  // K
                     {0x1F, 0x01, 0x01},  // L
                     {0x20, 0x18, 0x20},  // M
                     {0x10, 0x08, 0x04},  // N
                     {0x41, 0x41, 0x41},  // O
                     {0x1F, 0x14, 0x1C},  // P
                     {0x41, 0x45, 0x42},  // Q
                     {0x48, 0x4C, 0x4A},  // R
                     {0x1D, 0x15, 0x17},  // S
                     {0x40, 0x7F, 0x40},  // T
                     {0x01, 0x01, 0x01},  // U
                     {0x0C, 0x03, 0x0C},  // V
                     {0x02, 0x0C, 0x02},  // W
                     {0x14, 0x08, 0x14},  // X
                     {0x10, 0x0F, 0x10},  // Y
                     {0x45, 0x49, 0x51},  // Z
};

//--------------------------------------------------------------
// Set or clear a pixel at coordinates (col, row)
void setPixel(uint8_t col, uint8_t row, bool on)
{
  if ((col < 0) || (col > (PANEL_WIDTH - 1)) || (row < 0) || (row > (PANEL_HEIGHT - 1))) return;
  
  // Translate pixel coordinates to row and column address
  // This is necessary because the FP2800A chip skips the
  // addresses 0, 8, 16, 24
  row = coord_to_row_col[row];
  col = coord_to_row_col[col];
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
  uint8_t panel = panel_to_bits[col/PANEL_WIDTH];
  PORTL = (panel & 0xFF);  // Col_Enable panel x
  
  // Give the dots time to flip
  delayMicroseconds(600);
  
  // Reset Data and Enable lines
  PORTA &= 0x1F;
  PORTC &= 0x1F;
  
  // Wait t_off
  delayMicroseconds(150);
}  


//--------------------------------------------------------
void clearDisplay()
{
  fillRect(0, 0, (PANEL_WIDTH * PANEL_NUMBER - 1) , (PANEL_HEIGHT - 1), false);
}


//--------------------------------------------------------
// Print a letter at (x, y) = top-left corner of the letter
// A letter takes up 5x7 pixels.
void printLetter5x7(char character, int x, int y)
{
  if ((character >= '0')  && (character <= '['))
  {
    uint8_t* letter = font5x7[character - '0'];
    
    // Go through the 5 columns of the letter
    for (int col = 0;  col < 5;  col++)
    {
      uint8_t bits = letter[col];
      // Go through the pixels of the letter
      for (int row = 6;  row >= 0;  row--)
      {
        uint8_t pixel = bits & 0x01;
        bits >>= 1;
        setPixel(x + col, y + row, pixel);
      }
    }
  }
}

//--------------------------------------------------------
// Print a letter at (x, y) = top-left corner of the letter
// A letter takes up 3x5 pixels.
void printLetter3x5(char character, int x, int y)
{
  if ((character >= '0')  && (character <= 'Z'))
  {
    uint8_t* letter = font3x5[character - '0'];
    
    // Go through the 3 columns of the letter
    for (int col = 0;  col < 3;  col++)
    {
      uint8_t bits = letter[col];
      // Go through the pixels of the letter
      for (int row = 4;  row >= 0;  row--)
      {
        uint8_t pixel = bits & 0x01;
        bits >>= 1;
        setPixel(x + col, y + row, pixel);
      }
    }
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


//----------------------------------------------------------
void drawAmpel(int x, int y, bool on)
{
  drawRoundedRect( x, y, x + 4, y + 4, on);
  drawFilledCircle(x, y +  5, on);
  drawFilledCircle(x, y + 10, on);
}


//--------------------------------------------------------
// Pong 
// Helper function
void paddle(int x, int y)
{
  x -= 2;
  if (x < 0)  x = 0;
  if (x > 19) x = 19;
  for (int i = 0;  i < 5;  i++) 
    setPixel(x + i, y, true);
    
  if (x > 0)  setPixel(x - 1, y, false);
  if (x < (PANEL_WIDTH-1)) setPixel(x + 5, y, false);
}

void pong()
{
  setPixel(oldx, oldy, false);
  
  oldx = px;
  oldy = py;
  
  px += dx;
  py += dy;
  if (px == (PANEL_WIDTH - 1)) dx = -dx;
  if (px == 0)  dx = -dx;
  if (py == (PANEL_HEIGHT - 1)) dy = -dy;
  if (py == 0)  dy = -dy;
  
  setPixel(px, py, true);
  paddle(px, 0);
  paddle(px, (PANEL_WIDTH - 1));
  delay(10);
}


//------------------------------------------------
// Sierpinsky triangle
void sier()
{
  for (int x = 0;  x < PANEL_WIDTH;  x++) 
    for (int y = 0;  y < PANEL_HEIGHT;  y++) 
      setPixel(x, y, !(x & y));
}


//--------------------------------------------------------
//--------------------------------------------------------
void setup()
{
  // Port A and C = output
  DDRA = 0xFF;
  DDRC = 0xFF;
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
  
  // Clear the display
  clearDisplay();
}


//--------------------------------------------------------
//-------------------------------------------------
// Main loop
void loop()
{
  // put your main code here, to run repeatedly:
#ifdef TEST1_PIXEL_ON_OFF
  setPixel(1, 1, true);
  setPixel(2, 1, true);
  setPixel(3, 1, true);
  
  delay(2000);

  setPixel(1, 1, false);
  setPixel(2, 1, false);
  setPixel(3, 1, false);
  
  delay(2000);
#endif

#ifdef TEST2_ALL_ON_OFF
  fillRect(0, 0, (PANEL_WIDTH - 1), (PANEL_HEIGHT - 1), true);
  delay(4000);
  fillRect(0, 0, (PANEL_WIDTH - 1), (PANEL_HEIGHT - 1), false);
  delay(4000);
#endif

#ifdef TEST3_RANDOM_PIXELS
  setPixel(random(1, PANEL_WIDTH), random(1, PANEL_HEIGHT), random(0, 2));
#endif

#ifdef TEST4_BOUNCING_BALL
  px += dx;
  py += dy;
  if (px == PANEL_WIDTH) dx = -dx;
  if (px == 0)  dx = -dx;
  if (py == PANEL_HEIGHT-1) dy = -dy;
  if (py == 0)  dy = -dy;
  if (pixels[px][py] == 0)
  {
      pixels[px][py] = 1;
      setPixel(px, py, 1);
  }
  else
  {
      pixels[px][py] = 0;
      setPixel(px, py, 0);
  }   
 delay(5);
#endif

#ifdef TEST5_PONG 
 pong();
#endif

#ifdef TEST6_SIERPINSKY
 sier();
 while(1); 
#endif

#ifdef TEST7_LETTERS
  printLetter5x7('0',  0,  0);
  printLetter5x7('1',  6,  0);
  printLetter5x7('2', 12,  0);
  printLetter5x7('3', 18,  0);
  
  printLetter5x7('4',  0,  8);
  printLetter5x7('5',  6,  8);
  printLetter5x7('6', 12,  8);
  printLetter5x7('7', 18,  8);
  
  printLetter5x7('8',  0, 16);
  printLetter5x7('9',  6, 16);
  printLetter5x7(':', 12, 16);
  printLetter5x7(';', 18, 16);
  
  delay(5000);
  printLetter5x7('<',  0,  0);
  printLetter5x7('=',  6,  0);
  printLetter5x7('>', 12,  0);
  printLetter5x7('?', 18,  0);
  
  fillRect(0, 8, 27, 23, false);
  printLetter5x7('@',  0,  8);
  
  delay(5000);
  
  printLetter5x7('A',  0,  0);
  printLetter5x7('B',  6,  0);
  printLetter5x7('C', 12,  0);
  printLetter5x7('D', 18,  0);
  
  printLetter5x7('E',  0,  8);
  printLetter5x7('F',  6,  8);
  printLetter5x7('G', 12,  8);
  printLetter5x7('H', 18,  8);
  
  printLetter5x7('I',  0, 16);
  printLetter5x7('J',  6, 16);
  printLetter5x7('K', 12, 16);
  printLetter5x7('L', 18, 16);
  
  delay(5000);
  
  printLetter5x7('M',  0,  0);
  printLetter5x7('N',  6,  0);
  printLetter5x7('O', 12,  0);
  printLetter5x7('P', 18,  0);
  
  printLetter5x7('Q',  0,  8);
  printLetter5x7('R',  6,  8);
  printLetter5x7('S', 12,  8);
  printLetter5x7('T', 18,  8);
  
  printLetter5x7('U',  0, 16);
  printLetter5x7('V',  6, 16);
  printLetter5x7('W', 12, 16);
  printLetter5x7('X', 18, 16);
  
  delay(5000);
#endif

#ifdef TEST8_LAPCOUNTER
  delay(5000);

  drawAmpel(0, 5, true);
  delay(1000);
  drawAmpel(6, 5, true);
  delay(1000);
  drawAmpel(12, 5, true);
  delay(1000);
  drawAmpel(17, 5, true);
  delay(1000);
  drawAmpel(23, 5, true);
  delay(2300);
  
  fillRect(0, 5, 27, 20, false);
  printLetter5x7('G',  9, 10);
  printLetter5x7('O', 15, 10);
  printLetter5x7('[', 22, 10);  // is really '!'  
  printLetter5x7('[', 24, 10);  // is really '!'  

  delay(2000);
  clearDisplay();
  
  printLetter5x7('L',  0, 0);
  printLetter5x7('E',  6, 0);
  printLetter5x7('Z', 12, 0);
  
  drawRoundedRect(19, 0, 19+8, 0+8, true);
  printLetter3x5('2', 22, 2);
  
  printLetter5x7('2',   0,  9);
  printLetter5x7('3',   6,  9);
  printLetter3x5('L',  13, 11);
  printLetter3x5('A',  17, 11);
  printLetter3x5('P',  21, 11);
  printLetter3x5('S',  25, 11);
  
  printLetter5x7('1',  1, 17);
  printLetter5x7('7',  6, 17);
  printLetter3x5(':', 12, 18);
  printLetter5x7('6', 15, 17);
  printLetter5x7('2', 21, 17);

  delay(5000);
  
  printLetter5x7('A',  0, 0);
  printLetter5x7('L',  6, 0);
  printLetter5x7('X', 12, 0);
  
  drawRoundedRect(19, 0, 19+8, 0+8, true);
  printLetter3x5('1', 22, 2);
  
  printLetter5x7('2',   0,  9);
  printLetter5x7('5',   6,  9);
  printLetter3x5('L',  13, 11);
  printLetter3x5('A',  17, 11);
  printLetter3x5('P',  21, 11);
  printLetter3x5('S',  25, 11);
  
  printLetter5x7('1',  1, 17);
  printLetter5x7('5',  6, 17);
  printLetter3x5(':', 12, 18);
  printLetter5x7('8', 15, 17);
  printLetter5x7('3', 21, 17);
  
  while(1);
  delay(5000);
#endif

}
