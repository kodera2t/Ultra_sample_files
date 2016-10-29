/*
  Listfiles

 This example shows how print out the files in a
 directory on a SD card

 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 2 Feb 2014
 by Scott Fitzgerald

Modified for ULTRA sample program

 This example code is in the public domain.*/
 
/*
For ULTRA (Mega1284P board)
#define OLED_RESET 7
#define CS_SD 4
 */

/*
 For ULTRA Zero (SAMD21G18A board)
#define OLED_RESET 7
#define CS_SD 10 
 */

 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>

//For ULTRA (Mega1284P, monochrome OLED board)
#define OLED_RESET 1
#define CS_SD 4
///


// For ULTRA Zero (SAMD21G18A, monochrome OLED board)
//#define OLED_RESET 7
//#define CS_SD 10 
//


Adafruit_SSD1306 display(OLED_RESET);
int screenMem[46]; //the implementation of frame buffer is referenced from Ben Heck's
int cursorX = 0;    //Retro BASIC computer's source
int checkChar = 0;

File root;

// ASCII Characters
#define CR  '\r'
#define NL  '\n'
#define LF      0x0a
#define TAB '\t'
#define BELL  '\b'
#define SPACE   ' '
#define SQUOTE  '\''
#define DQUOTE  '\"'
#define CTRLC 0x1B  // Changed to ESC key (27 - 0x1B)
#define CTRLH 0x08
#define CTRLS 0x13
#define CTRLX 0x18

void setup() {
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
  for (int xg = 0 ; xg < 46 ; xg++) {
    screenMem[xg] = 32;
  }
    doFrame(45);    
  // Open serial communications and wait for port to open:
//  Serial.begin(9600);
 // while (!Serial) {
 //   ; // wait for serial port to connect. Needed for native USB port only
 // }

//  Serial.print("Initializing SD card...");

  if (!SD.begin(CS_SD)) {
  display.setCursor(32,32);
  display.print("CARD NOT");
  display.setCursor(32,40);
  display.print("found!!");
  display.display();  
    return;
  }
  display.setCursor(32,32);
  display.print("CARD found");  
  display.display();
//  Serial.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

 // Serial.println("done!");
}

void loop() {
  // nothing happens after setup finishes.
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      outchar(TAB);
    }
   // Serial.print(entry.name());
    printmsg(entry.name());
    if (entry.isDirectory()) {
      printmsg("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      printmsg("\t\t");
      //Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}


//static void printwpm(int k)
//{
//     display.setCursor(32,56);
//    display.print("wpm=");
//    display.print(k); 
//    if(mode_set==0){
//     display.print("    ");   
//    }else{
//     display.print("   R");
//    }
//    display.display();
//}

static void outchar(unsigned char c)
{
    lcdChar(c);
}

static void lcdChar(unsigned int c) {
if(c <= 128){
  if (c != 13 and c != 10 and c != 8) { 
    screenMem[22 + cursorX] = c;
    cursorX += 1;
    if (cursorX < 11) {
    display.setCursor(32+cursorX*6,48);   
    display.write(c);
//    filesave(c);  
    display.display();
    }
    
  }
  
  if (cursorX == 11 or c == 10) {     
  
    for (int xg = 11 ; xg > 0 ; xg--) {

      screenMem[0 + xg] = screenMem[11 + xg];
      screenMem[11 + xg] = screenMem[22 + xg];    
//      screenMem[22 + xg] = screenMem[33 + xg];
//      screenMem[63 + xg] = screenMem[84 + xg];
//      screenMem[84 + xg] = screenMem[125 + xg]; 
        
//      screenMem[0 + xg] = screenMem[21 + xg];
//      screenMem[21 + xg] = screenMem[42 + xg];    
//      screenMem[42 + xg] = screenMem[63 + xg];
//      screenMem[63 + xg] = screenMem[84 + xg];
//      screenMem[84 + xg] = screenMem[125 + xg];   
//      screenMem[125 + xg] = screenMem[146 + xg];



//      screenMem[146 + xg] = 32;
    
            screenMem[22 + xg] = 32;
    }
  
    cursorX = 0;
    
    doFrame(22);  

  }


}
}

static void doFrame(byte amount) {
        int xposi,yposi,yshift;
        display.clearDisplay();
  for (int xg = 0 ; xg < amount ; xg++) {
                yshift=int(xg/11.0);
                yposi=yshift*8;
                xposi=(xg-yshift*10)*6;
          display.setCursor(xposi+32,yposi+32);
    display.write(screenMem[xg]);
  }
        display.display();
}  

/***************************************************************************/
//void printmsgNoNL(const unsigned char *msg)
void printmsgNoNL(char *msg)
{
  while( pgm_read_byte( msg ) != 0 ) {
    outchar( pgm_read_byte( msg++ ) );
  };
}

/***************************************************************************/
//void printmsg(const unsigned char *msg)
void printmsg(char *msg)
{
  printmsgNoNL(msg);
  line_terminator();
}

static void line_terminator(void)
{
  outchar(NL);
  outchar(CR);
}



