#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
void timer1_init(void)
{
    TCCR1A = 0;
    // prescaler /8
    TCCR1B = (1 << CS11);
}

int main(void){
    DDRB |= (1 << DDB0); // led

    DDRD |= (1<<DDD4); // TRIG
    DDRD &= ~(1<<DDD2);  // ECHO
    
    timer1_init();

    uint16_t ticks;
    float tempo_us;
    float distancia_cm;

    while(1){
        PORTD &= ~ (1<<PD4);  // trig low
        _delay_us(2);

        PORTD |= (1<<PD4); // trig high
        _delay_us(10);
        PORTD &= ~(1<<PD4); 

        while (!(PIND & (1 << PIND2))) {
        }

        TCNT1=0;
        while (PIND & (1 << PIND2)) {
        }

        ticks = TCNT1;

        tempo_us = ticks*0.5;
        distancia_cm = (tempo_us*0.034)/2.0;
        if (distancia_cm < 20) {
            PORTB |= (1 << PB0); // led on 
        } else {
            PORTB &= ~(1 << PB0); // led off
        }
        _delay_ms(60);
    }
}