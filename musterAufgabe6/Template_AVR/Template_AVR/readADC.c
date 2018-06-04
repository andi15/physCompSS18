#include "system.h"      // Spezifika unseres Boards
#include "usart.h"       // I/O über serielle Schnittstelle

const char PROGMEM infoText[] = "Otto Mohl fuehlt sich angeblich wohl am Nordpol mit Atomstrom, aber politische Aussagen von Hunden sind nicht willkommen. Eine laengere Zeichenkette fuehlt sich aber in jedem Fall im Programmspeicher wohler als im RAM. Das wird durch das Schluesselwort 'PROGMEM' deklariert.\n";

/* Das eigentliche Programm */
int __attribute__((OS_main)) main( void ) {

  usart_init(); // initialisiere serielle Schnittstelle "SERIAL" zum PC
  sei();        // Interrupts freischalten (notwendig für serielle Schnittstelle)

  usart_puts_p(SERIAL, infoText);       // Ausgabe einer Zeichenkette (puts) im Programmspeicher (suffix _p)
  usart_puts_P(SERIAL, "Guten Tag!\n"); // Ausgabe einer Zeichenkette (puts), die der Compiler in den Programmspeicher legen soll (suffix _P)
  // achten Sie darauf "_p" und "_P" nicht zu verwechseln, da der Compiler den Fehler nicht erkennen kann.
  // Die Argumente sind vom selben Typ (const) char*, die unterschiedlichen Speicherbereiche einer Harvard-Architektur werden in
  // C (wie in vielen anderen Sprachen) nicht abgebildet
  
  int msg_count = 0;
  while (1) {
      char c = usart_getc(SERIAL);       // serielle Verbindung abfragen
      while (c != EOF) {                 // Nachricht vom PC?
          usart_putc(SERIAL, c);         // Echo
          c = usart_getc(SERIAL);
      }
      usart_putDec(SERIAL, msg_count++); // Int als Dezimalzahl ausgeben
      usart_putc(SERIAL,'\n');           // Zeichen ausgeben
      delay_ms(1000);                    // 1 Sekunde abwarten
  }
}
