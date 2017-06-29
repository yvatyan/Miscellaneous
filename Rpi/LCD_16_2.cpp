/*
 * lcd.c:
 *     Simple program to send a string to the LCD 
 */

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define LCD_E 19
#define LCD_RS 26
#define LCD_D4 21
#define LCD_D5 20
#define LCD_D6 16
#define LCD_D7 12

void pulseEnable ()
{
   digitalWrite (LCD_E, HIGH) ; 
   delay(0.5); //  1/2 microsecond pause - enable pulse must be > 450ns
   digitalWrite (LCD_E, LOW) ; 
}

/*
  send a byte to the lcd in two nibbles
  before calling use SetChrMode or SetCmdMode to determine whether to send character or command
*/
void lcd_byte(char bits)
{
  digitalWrite (LCD_D4,(bits & 0x10)) ;  
  digitalWrite (LCD_D5,(bits & 0x20)) ;  
  digitalWrite (LCD_D6,(bits & 0x40)) ;  
  digitalWrite (LCD_D7,(bits & 0x80)) ;  
  pulseEnable();

  digitalWrite (LCD_D4,(bits & 0x1)) ;  
  digitalWrite (LCD_D5,(bits & 0x2)) ;  
  digitalWrite (LCD_D6,(bits & 0x4)) ;  
  digitalWrite (LCD_D7,(bits & 0x8)) ;  
  pulseEnable();         
}

void SetCmdMode() 
{
  digitalWrite (LCD_RS, 0); // set for commands
}

void SetChrMode() 
{
  digitalWrite (LCD_RS, 1); // set for characters
}

void lcd_text(const char *s)
{
  while(*s) {
	  printf("%c\n", *s);
   if(*s == '\n' || *s == '\r') {
	   SetCmdMode();
	   lcd_byte(0xC0);
	   SetChrMode();
   }
   else {
	   lcd_byte(*s);
   }  
   s++;
  }
 }

void lcd_init()
{
   wiringPiSetupGpio () ; // use BCIM numbering
   // set up pi pins for output
   pinMode (LCD_E,  OUTPUT);
   pinMode (LCD_RS, OUTPUT);
   pinMode (LCD_D4, OUTPUT);
   pinMode (LCD_D5, OUTPUT);
   pinMode (LCD_D6, OUTPUT);
   pinMode (LCD_D7, OUTPUT);
   
   // initialise LCD
   SetCmdMode(); // set for commands
   lcd_byte(0x33); // full init 
   lcd_byte(0x32); // 4 bit mode
   lcd_byte(0x28); // 2 line mode
   lcd_byte(0x0C); // display on, cursor off, blink off
   lcd_byte(0x01);  // clear screen
   delay(3);        // clear screen is slow!
}

int main (int argc, char *argv []) 
{
  lcd_init();
  
  SetChrMode(); 
  if (argc > 1) 
     lcd_text(argv[1]);
  else 
     lcd_text("hello\nworld!");
  return 0;
}
