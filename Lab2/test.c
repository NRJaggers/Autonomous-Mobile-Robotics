#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

int main(void) {
   init();  //initialize board hardware

   test_include();

   return 0;
}
