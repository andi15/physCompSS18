#include "system.h"      // Spezifika unseres Boards
#include "usart.h"       // I/O über serielle Schnittstelle
#include <avr/sleep.h>

const char PROGMEM lineBreak[] = "\n";
// const char PROGMEM binPrefix[] = "0b";

/* Das eigentliche Programm */
int __attribute__((OS_main)) main( void ) {

  // NTC KONFIGURATION ________________________________________

  // setzt PIN PB3 (NTC_EN) auf OUTPUT
  PORTB_DIR=0b00001000;

  // setzt NTC PIN PB3 (NTC_EN) auf LOW -> aktiviert NTC
  PORTB_OUT=0b0;

  // ADC KONFIGURATION ________________________________________

  // ADC auf Messung mit Vorzeichen und 12 Bit Auflösung setzen
  ADCB_CTRLB = 0b00010000;

  //switch on ADC
  ADCB_CTRLA = 0b00000001;

  // Referenzspannung auf  Betriebsspannung / 1.6 setzen
  ADCB_REFCTRL = 0b00010000;

  // Prescaler auf DIV8 seten (sinnvoller Wert im Verhältnis zu Systemtakt)
  ADCB_PRESCALER = 0b1;

  // lese PIN0 aus PORTB:  PB0(NTC_TEMP)
  ADCB_CH0_MUXCTRL = 0b0;

  // ADCB_CH0_CTRL setzt Gain =1; Mode = Single-Ended; (!STARTS CONVERSION! laut Datenblatt; wird nicht gestetzt)
  ADCB_CH0_CTRL = 0b10000001;


  usart_init(); // initialisiere serielle Schnittstelle "SERIAL" zum PC
  sei();        // Interrupts freischalten (notwendig für serielle Schnittstelle)

  while (1) {
      delay_ms(1000);
      if(ADCB_CH0_INTFLAGS){
        uint16_t result = ADCB_CH0RES;
        // reinvoke conversion
        ADCB_CH0_CTRL = 0b10000001;
        usart_putBin(SERIAL,result);
        usart_puts_p(SERIAL, lineBreak);
      }
  }
}
