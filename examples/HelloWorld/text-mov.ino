#include <DMDESP.h>
#include <fonts/Mono5x7.h>
#include <fonts/Arial14.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/Arial_Black_16_ISO_8859_1.h>
#include <fonts/Arial_bold_14.h>
#include <fonts/Droid_Sans_16.h>
#include <fonts/Verdana_digits_24.h>
#include <fonts/fixednums7x15.h>
#include <fonts/fixednums8x16.h>

#define PANEL_WIDTH 2
#define PANEL_HEIGHT 1
DMDESP display(PANEL_WIDTH, PANEL_HEIGHT);

void setup()
{
    display.start();
    display.setBrightness(10);
    //display.setFont(Droid_Sans_16);
    //display.drawString(0, 0, "123,45.67890");
}

void loop()
{
    display.loop();
    TeksJalan(0, 50); // Tampilkan teks berjalan TeksJalan(posisi y, kecepatan);
}
static char *teks[] = {"12,23.456789"};

void TeksJalan(int y, uint8_t kecepatan) {

  static uint32_t pM;
  static uint32_t x;
  int width = display.getWidth();
  display.setFont(Droid_Sans_16);
  int fullScroll = display.getTextWidth(teks[0]) + width;
  if((millis() - pM) > kecepatan) { 
    pM = millis();
    if (x < fullScroll) {
      ++x;
    } else {
      x = 0;
      return;
    }
    display.drawString(width - x, y, teks[0]);
  }  

}
