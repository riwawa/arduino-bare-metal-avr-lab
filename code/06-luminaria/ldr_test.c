#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void uart_init(unsigned int baud){
    unsigned int ubrr = F_CPU / 16 / baud - 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_transmit(char c){
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uart_print(const char *s){
    while (*s) UART_transmit(*s++);
}

int main(void){
    DDRD &= ~(1 << DDD2);
    uart_init(9600);

    while (1){
        if (PIND & (1 << PD2))
            uart_print("PIR = 1\r\n");
        else
            uart_print("PIR = 0\r\n");

        _delay_ms(200);
    }
}