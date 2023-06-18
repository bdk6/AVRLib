
#define F_CPU 16000000

#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "gpio.h"
#include "lcd_44780.h"
#include "systick.h"


volatile char c = 0;

systick_timer_t timers[4];


void systick_callback(void)
{
  c++;
}

int main()
{
  GPIO_init(0,0,0,0,0,0);

  SYSTICK_init(CLK_DIV_64, timers, 4);
  
  LCD_44780_init(16,2,GPIO_PIN_B1, GPIO_PIN_B0,
		       GPIO_PIN_D7, GPIO_PIN_D6, GPIO_PIN_D5, GPIO_PIN_D4);
  LCD_44780_clear();
  _delay_ms(1000);
  LCD_44780_write_string("Hello, World!");
  _delay_ms(2000);
  //LCD_44780_goto(4,1);
  //LCD_44780_write_string("More");
  //SYSTICK_set_callback(0, systick_callback);
  SYSTICK_set_timer_ms(0, 500, 1, systick_callback);
  
  sei();
  
  _delay_ms(2000);
  
  //SYSTICK_set_callback(0, systick_callback);
  //while(c == 0);
  LCD_44780_goto(1,1);
  //LCD_44780_write_string("More:");
  _delay_ms(1000);
  //if(SYSTICK_get_ticks()  == 0) LCD_44780_write_string("zero");
  //else LCD_44780_write_string("nonzero");
  char str[32];

  LCD_44780_clear();
  LCD_44780_goto(0,0);
  sprintf(str, ":%ld:", (uint32_t) SYSTICK_get_irq_frequency() );
  LCD_44780_write_string(str);
  sprintf(str, "%ld:", SYSTICK_get_ticks() );
  LCD_44780_write_string(str);
  sprintf(str, "%d:", TCCR0);
  LCD_44780_write_string(str);
  sprintf(str, "%d:", TCNT0);
  LCD_44780_write_string(str);
  LCD_44780_goto(0,1);

  sprintf(str, "msk:%d:", TIMSK);
  LCD_44780_write_string(str);

  while(1)
    {
      char c2 = c;
      while( c2 == c);
      // changed
      LCD_44780_clear();
      LCD_44780_write_data(c);
    }
  
  

}
