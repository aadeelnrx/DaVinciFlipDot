// Demos that show off the display
// They are enabled by defining the various TEST* pre-processor #defines
// in the main file (sketch_flipdot)


//----------------------------------------------------------
void drawAmpel(int x, int y, bool on)
{
  drawRoundedRect( x, y, x + 4, y + 4, on);
  drawFilledCircle(x, y +  5, on);
  drawFilledCircle(x, y + 10, on);
}


//------------------------------------------------
// Sierpinsky triangle
void sier()
{
  for (int x = 0;  x < (PANEL_WIDTH * PANEL_NUMBER);  x++) 
    for (int y = 0;  y < PANEL_HEIGHT;  y++) 
      setPixel(x, y, !(x & y));
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


//-------------------------------------------------------
#ifdef TEST1_PIXEL_ON_OFF
void test1_pixel_on_off()
{
  setPixel(1, 1, true);
  setPixel(2, 1, true);
  setPixel(3, 1, true);
  
  delay(2000);

  setPixel(1, 1, false);
  setPixel(2, 1, false);
  setPixel(3, 1, false);
  
  delay(2000);
}
#endif


//--------------------------------------------------------
#ifdef TEST2_ALL_ON_OFF
void test2_all_on_off()
{
  // Fill the display
  fillDisplay();
  digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(4000);
  // Clear the display
  clearDisplay();
  digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(4000);
}
#endif


//--------------------------------------------------------
#ifdef TEST4_BOUNCING_BALL
void test4_bouncing_ball()
{
  px += dx;
  py += dy;
  if (px == (PANEL_WIDTH * PANEL_NUMBER)-1) dx = -dx;
  if (px == 0)  dx = -dx;
  if (py == PANEL_HEIGHT-1) dy = -dy;
  if (py == 0)  dy = -dy;
  if (pixels[px][py] == 0)
  {
      setPixel(px, py, 1);
  }
  else
  {
      setPixel(px, py, 0);
  }   
 delay(5);
}
#endif


//----------------------------------------------------------
#ifdef TEST7_LETTERS
void test7_letters()
{
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
  
  fillRect(0, 8, (PANEL_WIDTH - 1), (PANEL_HEIGHT - 1), false);
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
  printLetter5x7('a',  0,  0);
  printLetter5x7('b',  6,  0);
  printLetter5x7('c', 12,  0);
  printLetter5x7('d', 18,  0);
  
  printLetter5x7('e',  0,  8);
  printLetter5x7('f',  6,  8);
  printLetter5x7('g', 12,  8);
  printLetter5x7('h', 18,  8);
  
  printLetter5x7('i',  0, 16);
  printLetter5x7('j',  6, 16);
  printLetter5x7('k', 12, 16);
  printLetter5x7('l', 18, 16);
  
  delay(5000);
  
  printLetter5x7('m',  0,  0);
  printLetter5x7('n',  6,  0);
  printLetter5x7('o', 12,  0);
  printLetter5x7('p', 18,  0);
  
  printLetter5x7('q',  0,  8);
  printLetter5x7('r',  6,  8);
  printLetter5x7('s', 12,  8);
  printLetter5x7('t', 18,  8);
  
  printLetter5x7('u',  0, 16);
  printLetter5x7('v',  6, 16);
  printLetter5x7('w', 12, 16);
  printLetter5x7('x', 18, 16);
  
  delay(5000);
}
#endif


//-------------------------------------------------------------
#ifdef TEST8_LAPCOUNTER
void test8_lapcounter()
{
  delay(5000);

  drawAmpel(0, 5, true);
  delay(1000);
  drawAmpel(6, 5, true);
  delay(1000);
  drawAmpel(12, 5, true);
  delay(1000);
  drawAmpel(18, 5, true);
  delay(1000);
  drawAmpel(24, 5, true);
  delay(2300);
  
  clearDisplay();
  printLetter5x7('G',  2, 8);
  printLetter5x7('O',  8, 8);
  printLetter5x7('!', 15, 8);   
  printLetter5x7('!', 18, 8);  

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
}
#endif


//-------------------------------------------------------


