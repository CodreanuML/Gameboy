

#define TFT_CS        10 // Hallowing display control pins: chip select
#define TFT_RST       8 // Display reset
#define TFT_DC        9 // Display data/command select
#define TFT_BACKLIGHT  12
#define TFT_MOSI 11   //Data out
#define TFT_SCLK 13  // Clock out


/***************************************************
 * nano RP2040 Connect exercise
 * + ST7745/SD
 * 
 * On Arduino nanp RP2040 Connect:
 * (CIPO/MISO)  - D12
 * (COPI/MOSI)  - D11
 * (SCK)        - D13
 * (CS/SS) - Any GPIO (except for A6/A7
 * 
 * This example drive ST7735 SPI and SD using hardware SPI
 * 
 * TFT_ST7735 nano RP2040 Connect
 * ------------------------------
 * VCC        3V3
 * GND        GND
 * CS         10
 * RESET      9
 * A0(DC)     8
 * SDA        11
 * SCK        13
 * LED        3V3
 * 
 ****************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SD.h>
#include <SPI.h>



Adafruit_ST7735 tft_ST7735 = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
int x= 0;
int y=0;
void setup(void) {
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  Serial.println("=====================");
  Serial.println("- setup() -");
  tft_ST7735.initR(128*160); //INITR_BLACKTAB
  Serial.println("tft: " 
                  + String(tft_ST7735.width()) + " : " 
                  + String(tft_ST7735.height()));

  tft_ST7735.fillScreen(ST7735_BLACK);
  tft_ST7735.setTextWrap(true);
  tft_ST7735.setTextColor(ST77XX_WHITE);
   tft_ST7735.setRotation(2);
  tft_ST7735.setCursor(0, 80);
  tft_ST7735.print("O");
  tft_ST7735.println("\n");

  //----------------------------------------
  delay(2000);

 
  tft_ST7735.setCursor(0, 30);
  tft_ST7735.print("rotation: " + String(tft_ST7735.getRotation()));
  tft_ST7735.setCursor(0, 40);
  tft_ST7735.print(String(tft_ST7735.width()) + " x " + String(tft_ST7735.height()));
  
  delay(2000);

  tft_ST7735.fillScreen(ST77XX_RED);
  tft_ST7735.setCursor(50, 50);
  tft_ST7735.print("RED");
  delay(1000);
  tft_ST7735.fillScreen(ST77XX_GREEN);
  tft_ST7735.setCursor(50, 50);
  tft_ST7735.print("GREEN");
  delay(1000);
  tft_ST7735.fillScreen(ST77XX_BLUE);
  tft_ST7735.setCursor(50, 50);
  tft_ST7735.print("BLUE");
  delay(1000);

  delay(1000);
  
  //----------------------------------------

  Serial.println("\n- End of setup() -\n");
  tft_ST7735.fillScreen(ST7735_BLACK);
}


void loop() {

  
  tft_ST7735.drawPixel(x, y,ST77XX_GREEN);
  if (x<128){
    x++;
  }
  if (y<160){
    y++;
  }

  delay(200);

}