#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>

void uart_init(unsigned int baud){
    unsigned int ubrr = F_CPU / 16 / baud - 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_transmit(char data){
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void uart_print(const char *texto){
    while (*texto != '\0'){
        UART_transmit(*texto);
        texto++;
    }
}

void uart_print_uint16(uint16_t number){
    if (number == 0){
        UART_transmit('0');
        return;
    }
    char buffer[5];
    int i = 0;
    while (number > 0){
        buffer[i] = (number % 10) + '0';
        i++;
        number /= 10;
    }
    for (int j = i - 1; j >= 0; j--) UART_transmit(buffer[j]);
}

void adc_init(void){
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(void){
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

int main(void){
    DDRB |= (1 << DDB1) | (1 << DDB2) | (1 << DDB3);
    DDRD &= ~(1 << DDD2);

    uart_init(9600);
    adc_init();

    bool escuro = false;
    bool presenca = false;

    while (1){
        uint16_t valor = adc_read();
        presenca = (PIND & (1 << PD2)) != 0;

        PORTB &= ~((1 << PB1) | (1 << PB2) | (1 << PB3));

        if (valor < 400){
            PORTB |= (1 << PB1);
            escuro = true;
        } else if (valor < 720){
            PORTB |= (1 << PB3) | (1 << PB1);
            escuro = false;
        } else {
            PORTB |= (1 << PB2);
            escuro = false;
        }

        if (escuro && presenca){
            PORTB &= ~((1 << PB1) | (1 << PB2) | (1 << PB3));
            PORTB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
        }

        uart_print("ADC: ");
        uart_print_uint16(valor);
        uart_print(" | ");

        if (escuro && presenca) uart_print("ESCURO + PRESENCA");
        else if (escuro && !presenca) uart_print("ESCURO + SEM PRESENCA");
        else if (!escuro && presenca) uart_print("CLARO + PRESENCA");
        else uart_print("CLARO + SEM PRESENCA");

        uart_print("\r\n");
        _delay_ms(500);
    }

    return 0;
}