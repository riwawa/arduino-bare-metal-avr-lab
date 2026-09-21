# ATmega328P — Datasheet Notes

Notas pessoais de leitura do datasheet do ATmega328P.

O objetivo não é resumir o documento inteiro, mas registrar as partes que já usei em projetos e como elas se conectam ao código.

> Personal rule: **Do not document registers I have not used yet.**

## 1. GPIO / I/O Ports

Os pinos digitais do ATmega328P são organizados em ports:

- Port B → PB0 ... PB7
- Port C → PC0 ...
- Port D → PD0 ... PD7

Cada pino possui três controles principais:

- `DDRx` → direção
- `PORTx` → valor de saída / pull-up
- `PINx` → leitura do pino

No nome genérico do datasheet:

```text
x = port
n = número do bit

DDxn
PORTxn
PINxn
```

Exemplo para PB0:

```text
DDRx   → DDRB
DDxn   → DDB0
PORTx  → PORTB
PORTxn → PB0
PINx   → PINB
```

## 2. DDRx — Data Direction Register

```text
DDxn = 0 → INPUT
DDxn = 1 → OUTPUT
```

Exemplo:

```c
DDRB |= (1 << DDB0);
```

Interpretação:

```text
1 << DDB0
↓
cria máscara para o bit 0

DDRB |= máscara
↓
coloca DDB0 em 1

resultado
↓
PB0 vira OUTPUT
```

Isso ainda não define HIGH ou LOW.

## 3. PORTx

Quando o pino está configurado como output:

```text
PORTxn = 0 → OUTPUT LOW
PORTxn = 1 → OUTPUT HIGH
```

Exemplo:

```c
PORTB |= (1 << PB0);
```

No circuito:

```text
PB0/D8 → resistor → LED → GND
```

HIGH acende o LED.

Para colocar em LOW:

```c
PORTB &= ~(1 << PB0);
```

## 4. PINx

`PINx` é usado para observar o estado lógico presente no pino.

No AVR, escrever `1` em um bit de `PINx` faz toggle do bit correspondente de `PORTx`.

## 5. Bit masks

```c
1 << n
```

cria uma máscara com apenas o bit `n` em 1.

```text
1 << 0 → 00000001
1 << 1 → 00000010
1 << 3 → 00001000
```

Setar:

```c
REG |= (1 << BIT);
```

Limpar:

```c
REG &= ~(1 << BIT);
```

Alternar:

```c
REG ^= (1 << BIT);
```

Testar:

```c
if (REG & (1 << BIT))
```

# Timer1

## 6. Timer1 é um contador de 16 bits

```text
TCNT1
```

Faixa:

```text
0 ... 65535
```

## 7. Clock e prescaler

```text
F_CPU = 16 MHz
```

Timer0 e Timer1 podem usar:

```text
clock direto
clock / 8
clock / 64
clock / 256
clock / 1024
```

Para `/1024`:

```text
16.000.000 / 1024 = 15.625 Hz
```

Logo:

```text
15.625 ticks/s
```

Cada tick dura:

```text
1 / 15625 = 64 µs
```

## 8. Clock Select — CS12:0

Para prescaler `/1024`:

```text
CS12 CS11 CS10
  1    0    1
```

Código:

```c
TCCR1B |= (1 << CS12) | (1 << CS10);
```

## 9. CTC — Clear Timer on Compare Match

No modo CTC:

```text
TCNT1 aumenta
↓
TCNT1 == TOP
↓
compare match
↓
TCNT1 volta para 0
```

No projeto:

```text
TOP = OCR1A
```

O modo CTC com `OCR1A` como TOP usa:

```text
WGM13 WGM12 WGM11 WGM10
  0     1     0     0
```

## 10. WGM — Waveform Generation Mode

Para CTC com `OCR1A`:

```c
TCCR1B |= (1 << WGM12);
```

## 11. OCR1A

Para aproximadamente 1 segundo:

```text
F_CPU = 16.000.000 Hz
prescaler = 1024

timer frequency =
16.000.000 / 1024
=
15.625 Hz
```

Precisamos de 15.625 contagens.

Como a contagem inclui zero:

```text
0 ... 15624
```

então:

```c
OCR1A = 15624;
```

Fórmula:

```text
OCR1A =
(F_CPU / prescaler) × tempo - 1
```

## 12. Compare Match Flag — OCF1A

Quando:

```text
TCNT1 == OCR1A
```

o hardware seta:

```text
OCF1A = 1
```

`OCF1A` fica em:

```text
TIFR1
```

Teste:

```c
if (TIFR1 & (1 << OCF1A))
```

## 13. Write-one-to-clear

`OCF1A` é limpa escrevendo `1` no próprio bit:

```c
TIFR1 |= (1 << OCF1A);
```

## 14. Projeto: LED com Timer1 CTC

Fluxo:

```text
16 MHz system clock
        ↓
prescaler /1024
        ↓
15.625 timer ticks/s
        ↓
TCNT1 conta
        ↓
0 ... 15624
        ↓
TCNT1 == OCR1A
        ↓
OCF1A = 1
        ↓
software detecta a flag
        ↓
PORTB ^= (1 << PB0)
        ↓
LED troca de estado
```

Código:

```c
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
```

## Datasheet sections already used

- Section 1 — Pin Configurations
- Section 13 — I/O Ports
- Section 13.2 — Ports as General Digital I/O
- Section 15 — 16-bit Timer/Counter1
- Section 15.9.2 — Clear Timer on Compare Match
- Section 16 — Timer/Counter0 and Timer/Counter1 Prescalers
