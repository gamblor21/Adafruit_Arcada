#if defined(ARDUINO_ARCH_RP2040)
#include <hardware/spi.h>
#include <Adafruit_GC9A01A.h>
//#include <Adafruit_ST7789.h>

#define ARCADA_TFT_SPI SPI
#define ARCADA_TFT_RP2040_SPI spi0
#define ARCADA_TFT_CS 17  // Display CS Arduino pin number
#define ARCADA_TFT_DC 15  // Display D/C Arduino pin number
#define ARCADA_TFT_RST 14 // Display reset Arduino pin number

#define ARCADA_LEFTTFT_SPI SPI1
#define ARCADA_LEFTTFT_RP2040_SPI spi1
#define ARCADA_LEFTTFT_CS  13
#define ARCADA_LEFTTFT_DC  8
#define ARCADA_LEFTTFT_RST 14

#define ARCADA_TFT_ROTATION 2
#define ARCADA_TFT_DEFAULTFILL 0xFFFF
#define ARCADA_TFT_WIDTH 240
#define ARCADA_TFT_HEIGHT 240

#define ARCADA_AUDIO_OUT 13

#define ARCADA_CALLBACKTIMER 4
#define ARCADA_CALLBACKTIMER_HANDLER TC4_Handler

#define ARCADA_USE_JSON

#include "arcadatype.h"

class Adafruit_Arcada : public Adafruit_Arcada_SPITFT {
public:
  Adafruit_Arcada(void) { _has_accel = false; };

  Adafruit_GC9A01A *_display, *display2;
  //Adafruit_ST7789 *_display;

  bool variantBegin(void) { return true; }
  
  uint32_t variantReadButtons(void) { return 0; }

  void displayBegin(void) {
	SPI.setRX(16);
	SPI.setCS(ARCADA_TFT_CS);
    SPI.setSCK(18);
    SPI.setTX(19);
	
	pinMode(ARCADA_TFT_RST, OUTPUT);
    digitalWrite(ARCADA_TFT_RST, LOW);
    delay(10);
    digitalWrite(ARCADA_TFT_RST, HIGH);
    delay(10);

    uint8_t rtna = 0x01; // Screen refresh rate control (datasheet 9.2.18, FRCTRL2)
	
	_display = new Adafruit_GC9A01A(&ARCADA_TFT_SPI, ARCADA_TFT_DC, ARCADA_TFT_CS, -1);
    _display->begin();
	
	//_display = new Adafruit_ST7789(&ARCADA_TFT_SPI, ARCADA_TFT_CS, ARCADA_TFT_DC, -1);
	//_display->init(240, 240);
	
    _display->sendCommand(0xC6, &rtna, 1);
    _display->setSPISpeed(50000000); // yes fast
    _display->setRotation(ARCADA_TFT_ROTATION);
    _display->fillScreen(ARCADA_TFT_DEFAULTFILL);

    display = _display;

#if defined(ARCADA_LEFTTFT_SPI)
	SPI1.setCS(ARCADA_LEFTTFT_CS);
    SPI1.setSCK(10);
    SPI1.setTX(11);
	SPI1.setRX(12);
	
	display2 = new Adafruit_GC9A01A(&SPI1, ARCADA_LEFTTFT_DC, ARCADA_LEFTTFT_CS, -1);
    display2->begin();
    display2->sendCommand(0xC6, &rtna, 1); // Helps contrast a little
    display2->setSPISpeed(50000000);
    display2->setRotation(ARCADA_TFT_ROTATION);
    display2->fillScreen(ARCADA_TFT_DEFAULTFILL);
#endif
  }
};

#endif
