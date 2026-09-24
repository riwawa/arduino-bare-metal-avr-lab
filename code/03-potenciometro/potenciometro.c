#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

void adc_init(void)
{
    ADMUX = (1 << REFS0);
    ADCSRA =
        (1 << ADEN) |
        (1 << ADPS2) |
        (1 << ADPS1) |
        (1 << ADPS0);
}

uint16_t adc_read(void)
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
    {
    }

    return ADC;
}

void pwm_init(void)
{
    DDRB |= (1 << DDB1);

    TCCR1A =
        (1 << COM1A1) |
        (1 << WGM10);

    TCCR1B =
        (1 << WGM12) |
        (1 << CS11);
}

int main(void)
{
    DDRC &= ~(1 << DDC0);

    adc_init();
    pwm_init();

    while (1)
    {
        uint16_t valor_potenciometro = adc_read();
        OCR1A = valor_potenciometro / 4;

        _delay_ms(10);
    }
}