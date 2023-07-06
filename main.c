#include <pdk/device.h>
#include "auto_sysclock.h"
#include "delay.h"




// IGNTION is placed on the PA5 pin (Port A, Bit 5) with an active high configuration
#define IGN_BIT               5
// IGNTION is active high, so define helper for better readability below
#define isIgnActive()         (PA & (1 << IGN_BIT))

// LOCK is placed on the PA6 pin (Port A, Bit 6) with an active high configuration
#define LOCK_BIT              6
// LOCK is active high, so define helper for better readability below
#define isLockActive()        (PA & (1 << LOCK_BIT))

// UNLOCK is placed on the PA7 pin (Port A, Bit 7) with an active high configuration
#define UNLOCK_BIT            7
// UNLOCK is active high, so define helper for better readability below
#define isUnlockActive()      (PA & (1 << UNLOCK_BIT))



// DS34C87N_EN is placed on the PA0 (Port A, Bit 0) with a current sink configuration
#define DIFF_EN_BIT            0
// ESP is active high (current sink), so define helpers for better readability below
#define enableDIFF()           PA |= (1 << DIFF_EN_BIT) 
#define disableDIFF()          PA &= ~(1 << DIFF_EN_BIT)

// ESP is placed on the PA3 pin (Port A, Bit 3) with a current sink configuration
#define ESP_EN_BIT            3
// ESP is active high (current sink), so define helpers for better readability below
#define enableESP()           PA |= (1 << ESP_EN_BIT) 
#define disableESP()          PA &= ~(1 << ESP_EN_BIT)

// ESP is placed on the PA4 pin (Port A, Bit 4) with a current sink configuration
#define ESP_BIT               4
// ESP is active high (current sink), so define helpers for better readability below
#define espSigStart()           PA |= (1 << ESP_BIT) 
#define espSigStop()          PA &= ~(1 << ESP_BIT)

void inline double_press();
void inline long_press();
void inline short_press();


// Main program
void main() {

  // Initialize hardware
  PADIER |= (1 << IGN_BIT);         // Enable IGN as digital input
  PADIER |= (1 << LOCK_BIT);        // Enable Lock as digital input
  PADIER |= (1 << UNLOCK_BIT);      // Enable Unlock as digital input
  
  PAC |= (1 << ESP_BIT);            // Set LED as output (all pins are input by default)
  PAC |= (1 << ESP_EN_BIT);         // Set LED as output (all pins are input by default)
  PAC |= (1 << DIFF_EN_BIT);         // Set LED as output (all pins are input by default)
  

  disableESP();
  disableDIFF();

  _delay_ms(1000);
  _delay_ms(1000);
  _delay_ms(1000);

  enableESP();
  _delay_ms(500);
  enableDIFF();
  
  uint8_t ignSigSent = 0;
  
  while (1) {

    if (isUnlockActive()) {

     short_press();

    } else if (!ignSigSent) {
      if (isIgnActive()) {
        double_press();
        ignSigSent = 1;
      }
    } else if (ignSigSent) {
      if (!isIgnActive()) {
        short_press();
        ignSigSent = 0;
      }
    } else if (isLockActive()) {
      if ( !isIgnActive()){
        long_press();
      }
    }
    _delay_ms(50);
  }
}


inline void double_press(void) {

  espSigStart();
  _delay_ms(200); 
  espSigStop();
  _delay_ms(200); 
  espSigStart();
  _delay_ms(200); 
  espSigStop();
  _delay_ms(1000);
}

inline void long_press(void) {
  
  espSigStart();
  _delay_ms(800);               // 50 < x < 600 < y
  espSigStop();
  _delay_ms(1000);
}

inline void short_press(void){

  espSigStart();
  _delay_ms(200); 
  espSigStop();
  _delay_ms(1000);
}



// Startup code - Setup/calibrate system clock
unsigned char _sdcc_external_startup(void) {

  // Initialize the system clock (CLKMD register) with the IHRC, ILRC, or EOSC clock source and correct divider.
  // The AUTO_INIT_SYSCLOCK() macro uses F_CPU (defined in the Makefile) to choose the IHRC or ILRC clock source and divider.
  // Alternatively, replace this with the more specific PDK_SET_SYSCLOCK(...) macro from pdk/sysclock.h
  AUTO_INIT_SYSCLOCK();

  // Insert placeholder code to tell EasyPdkProg to calibrate the IHRC or ILRC internal oscillator.
  // The AUTO_CALIBRATE_SYSCLOCK(...) macro uses F_CPU (defined in the Makefile) to choose the IHRC or ILRC oscillator.
  // Alternatively, replace this with the more specific EASY_PDK_CALIBRATE_IHRC(...) or EASY_PDK_CALIBRATE_ILRC(...) macro from easy-pdk/calibrate.h
  AUTO_CALIBRATE_SYSCLOCK(TARGET_VDD_MV);

  return 0;   // Return 0 to inform SDCC to continue with normal initialization.
}
