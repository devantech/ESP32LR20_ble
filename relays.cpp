
#include <Arduino.h>
#include "relays.h"


void init_relays() {

  pinMode( RELAY_1, OUTPUT );      
  pinMode( RELAY_2, OUTPUT );

  digitalWrite( RELAY_1, LOW );
  digitalWrite( RELAY_2, LOW );
    
}


void get_relay_states( char * states ) {
    

  states[0] = ( digitalRead(RELAY_2) == LOW ) ? '0' : '1';
  states[1] = ( digitalRead(RELAY_1) == LOW ) ? '0' : '1';
  states[2] = '\0';

}


void set_relay_states( const char *st ) {

  digitalWrite( RELAY_2, ( st[0] == '1' ) ? HIGH : LOW );
  digitalWrite( RELAY_1, ( st[1] == '1' ) ? HIGH : LOW );
  
}
