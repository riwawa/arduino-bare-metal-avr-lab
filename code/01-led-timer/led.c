#include <avr/io.h>

int main(void)
{
    DDRB |= (1 << DDB0);

    TCCR1A = 0;
    TCCR1B = 0;

    OCR1A = 15624;

    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);

    while (1)
    {
        if (TIFR1 & (1 << OCF1A))
        {
            PORTB ^= (1 << PB0);
            TIFR1 |= (1 << OCF1A);
        }
    }
}