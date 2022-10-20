#if defined(ADAFRUIT_FEATHER_M4_EXPRESS)
#include <Adafruit_GC9A01A.h>

//#define TWO_DISPLAYS

 #if defined(GLOBAL_VAR)
  SPIClass SPI1(&sercom3, 6, 13, 12, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_2);
 #else
  extern SPIClass SPI1;
 #endif

#define ARCADA_TFT_SPI SPI
#define ARCADA_TFT_CS 11  // Display CS Arduino pin number
#define ARCADA_TFT_DC 10  // Display D/C Arduino pin number
#define ARCADA_TFT_RST 14 // Display reset Arduino pin number

#define ARCADA_LEFTTFT_SPI SPI1
#define ARCADA_LEFTTFT_CS  5
#define ARCADA_LEFTTFT_DC  6
#define ARCADA_LEFTTFT_RST 14

//#define ARCADA_TFT_LITE 9

#define ARCADA_TFT_ROTATION 2
#define ARCADA_TFT_DEFAULTFILL 0xFFFF
#define ARCADA_TFT_WIDTH 240
#define ARCADA_TFT_HEIGHT 240


#define ARCADA_AUDIO_OUT A0
#define ARCADA_NEOPIXEL_PIN 8
#define ARCADA_NEOPIXEL_NUM 1

#define ARCADA_CALLBACKTIMER 4
#define ARCADA_CALLBACKTIMER_HANDLER TC4_Handler

#define ARCADA_USE_JSON

#include "arcadatype.h"

class Adafruit_Arcada : public Adafruit_Arcada_SPITFT {
public:
  Adafruit_Arcada(void) { _has_accel = false; };

  Adafruit_GC9A01A *_display, *display2;

  bool variantBegin(void) { return true; }
  
  uint32_t variantReadButtons(void) { return 0; }

  void displayBegin(void) {
	//pinMode(ARCADA_TFT_RST, OUTPUT);
    //digitalWrite(ARCADA_TFT_RST, LOW);
    //delay(10);
    //digitalWrite(ARCADA_TFT_RST, HIGH);
    //delay(10);
    
	_display = new Adafruit_GC9A01A(
      &ARCADA_TFT_SPI, ARCADA_TFT_DC, ARCADA_TFT_CS, -1);
    _display->begin();
    _display->setRotation(ARCADA_TFT_ROTATION);
    _display->fillScreen(ARCADA_TFT_DEFAULTFILL);
    uint8_t rtna =
        0x01; // Screen refresh rate control (datasheet 9.2.18, FRCTRL2)
    _display->sendCommand(0xC6, &rtna, 1);
    _display->setSPISpeed(50000000); // yes fast

//#if defined(ARCADA_LEFTTFT_SPI)
    pinPeripheral(6, PIO_SERCOM_ALT); // Enable SPI1 on RX, TX
    pinPeripheral(13, PIO_SERCOM);
	pinPeripheral(12, PIO_SERCOM);
	
    //display2 = new Adafruit_GC9A01A(&ARCADA_LEFTTFT_SPI, ARCADA_LEFTTFT_DC,
	display2 = new Adafruit_GC9A01A(&SPI1, ARCADA_LEFTTFT_DC,
                                    ARCADA_LEFTTFT_CS, -1);
    display2->begin();
    display2->sendCommand(0xC6, &rtna, 1); // Helps contrast a little
    display2->setSPISpeed(50000000);
    display2->setRotation(ARCADA_TFT_ROTATION);
    display2->fillScreen(0xabc1);
//#endif

	display = _display;
  }
};

#endif
