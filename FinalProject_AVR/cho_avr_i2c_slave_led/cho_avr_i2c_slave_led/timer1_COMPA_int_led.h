#include <avr/interrupt.h>

#define SHIFT_LEFT 1
#define SHIFT_RIGHT 2
#define BLINKING 3
#define SET_LED 0
#define GET_LED_STATUS 0

extern char ledOperationMode;

void timer1_init(void);
