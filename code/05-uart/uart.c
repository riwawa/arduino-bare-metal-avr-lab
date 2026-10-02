#include <avr/io.h>

void uart_putchar(char c)
{
    while (!(UCSR0A & (1 << UDRE0))){}
    UDR0 = c;
}

void uart_print(const char *texto)
{
    while (*texto != '\0')
    {
        uart_putchar(*texto);
        texto++;
    }
}

int main(void)
{
    UBRR0 = 103;
    UCSR0B = (1 << TXEN0);

    UCSR0C =
        (1 << UCSZ01) |
        (1 << UCSZ00);

    uart_print("OLA");

    while (1)
    {
    }
}