PRG            = calibrate
OBJ            = calibrate.o
MCU_TARGET     = attiny4313
MCU		= attiny4313
PRG_TARGET 	= attiny4313

#optimize for size
OPTIMIZE       = -Os

FLASHCMD	= avrdude -c usbasp -v -p $(PRG_TARGET) -U flash:w:$(PRG).hex
FLASHFUSESCMD   = avrdude -c usbasp -v -p $(PRG_TARGET) -B4 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

SERIAL = /dev/ttyUSB0

DEFS           = -DF_CPU=4000000

# You should not have to change anything below here.

CC             = avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS        =  -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst text

$(PRG).elf: $(OBJ)
	$(CC) -DEXT_OSCCAL=$(EXT_OSCCAL)  $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak *.bin *.elf *.srec *.hex
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

flash:
	$(FLASHFUSESCMD)
	$(FLASHCMD)

lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images

text: hex bin

hex:  $(PRG).hex
bin:  $(PRG).bin

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

