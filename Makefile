# ==========================================
# Generic AVR build rules
# ==========================================

MCU        ?= atmega328p
F_CPU      ?= 16000000UL
PROGRAMMER ?= arduino
PORT       ?= /dev/cu.usbserial-1140
BAUD       ?= 115200

CC      = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS = -Wall -Wextra -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -I.

OBJ = $(SRC:.c=.o)

.PHONY: all flash clean size

all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJ)
	$(CC) -mmcu=$(MCU) $(OBJ) -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	$(AVRDUDE) \
		-c $(PROGRAMMER) \
		-p m328p \
		-P $(PORT) \
		-b $(BAUD) \
		-D \
		-U flash:w:$(TARGET).hex:i

size: $(TARGET).elf
	avr-size -C --mcu=$(MCU) $(TARGET).elf

clean:
	rm -f *.o *.elf *.hex