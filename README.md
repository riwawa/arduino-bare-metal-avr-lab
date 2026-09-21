# Bare Metal AVR in C

A hands-on repository for learning embedded systems by programming AVR microcontrollers directly in C, without relying on high-level Arduino APIs.

The goal is to understand what happens below abstractions such as `pinMode()`, `digitalWrite()`, `analogRead()` and `delay()` by working directly with registers, timers, GPIO, interrupts, ADC, PWM, UART and other peripherals.

The main target is the **ATmega328P**, using an Arduino-compatible UNO board as the development board.

## Goals

- read microcontroller datasheets
- understand registers and bit manipulation
- configure GPIO directly
- work with timers and prescalers
- use polling and interrupts
- learn ADC, PWM and UART
- understand clock systems
- debug hardware and firmware
- translate schematics into breadboard circuits
- compile and flash firmware without the Arduino IDE

## Hardware

- Arduino-compatible UNO R3
- ATmega328P
- 16 MHz clock
- 5 V logic

## Toolchain

```text
avr-gcc
avr-objcopy
avrdude
make
```

Environment:

```text
macOS
VS Code
Homebrew AVR GCC
```

Build flow:

```text
source.c
   ↓
avr-gcc
   ↓
object file
   ↓
ELF
   ↓
avr-objcopy
   ↓
Intel HEX
   ↓
avrdude
   ↓
ATmega328P Flash
```

## Repository Structure

```text
code/
    source code for each experiment

visual/
    breadboard photos, schematics and diagrams

docs/
    notes about datasheets, registers and peripherals
```

Suggested project layout:

```text
project/
├── source.c
├── Makefile
└── README.md
```

Visuals:

```text
visual/project/
├── breadboard.jpg
├── schematic.png
└── notes.md
```

## References

- ATmega328P datasheet
- AVR instruction set documentation
- board schematics

## Status

Work in progress.

This repository will grow as I explore AVR peripherals and embedded systems through small practical projects.
