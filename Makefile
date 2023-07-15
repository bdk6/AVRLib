#AVR Libc Makefile Template
# Modified from AVR-libc simple sample project
# WRC 20220701

# Set project name and output file here
PRG            = avrlib
OBJ            = avrlib.o

# Uncomment appropriate processor target
#MCU_TARGET     = at90s2313
#MCU_TARGET     = at90s2333
#MCU_TARGET     = at90s4414
#MCU_TARGET     = at90s4433
#MCU_TARGET     = at90s4434
#MCU_TARGET     = at90s8515
#MCU_TARGET     = at90s8535
#MCU_TARGET     = atmega128
#MCU_TARGET     = atmega1280
#MCU_TARGET     = atmega1281
#MCU_TARGET     = atmega1284p
#MCU_TARGET     = atmega16
#MCU_TARGET     = atmega163
#MCU_TARGET     = atmega164p
#MCU_TARGET     = atmega165
#MCU_TARGET     = atmega165p
#MCU_TARGET     = atmega168
#MCU_TARGET     = atmega169
#MCU_TARGET     = atmega169p
#MCU_TARGET     = atmega2560
#MCU_TARGET     = atmega2561
#MCU_TARGET     = atmega32
#MCU_TARGET     = atmega324p
#MCU_TARGET     = atmega325
#MCU_TARGET     = atmega3250
#MCU_TARGET     = atmega329
#MCU_TARGET     = atmega3290
#MCU_TARGET     = atmega32u4
#MCU_TARGET     = atmega48
#MCU_TARGET     = atmega64
#MCU_TARGET     = atmega640
#MCU_TARGET     = atmega644
#MCU_TARGET     = atmega644p
#MCU_TARGET     = atmega645
#MCU_TARGET     = atmega6450
#MCU_TARGET     = atmega649
#MCU_TARGET     = atmega6490
MCU_TARGET     = atmega8
#MCU_TARGET     = atmega8515
#MCU_TARGET     = atmega8535
#MCU_TARGET     = atmega88
#MCU_TARGET     = attiny2313
#MCU_TARGET     = attiny24
#MCU_TARGET     = attiny25
#MCU_TARGET     = attiny26
#MCU_TARGET     = attiny261
#MCU_TARGET     = attiny44
#MCU_TARGET     = attiny45
#MCU_TARGET     = attiny461
#MCU_TARGET     = attiny84
#MCU_TARGET     = attiny85
#MCU_TARGET     = attiny861

# Select optimization level here
#OPTIMIZE       = -O2
#OPTIMIZE      = -O1
#OPTIMIZE      = -O3
OPTIMIZE      = -Os
#OPTIMIZE      = -Os -mcall-prologues

# Set any Defines and Libraries used here.
DEFS           =
LIBS           =

# You should not have to change anything below here.

CC             = avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map -ffunction-sections -flto

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

avrlib.a: systick.o gpio.o lcd_44780.o softspi.o
	avr-ar r avrlib.a lcd_44780.o softspi.o systick.o gpio.o

devicelib.a:	button.o keypad.o lcd_44780.o encoder.o
	avr-ar r devicelib.a button.o keypad.o lcd_44780.o encoder.o


avrlib.elf:  avrlib_test.o systick.o gpio.o lcd_44780.o softspi.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o avrlib.elf  avrlib_test.o systick.o gpio.o lcd_44780.o softspi.o $(LDFLAGS) $(LIBS)

avrlib_test.o:	avrlib_test.c
	$(CC) $(CFLAGS) -c avrlib_test.c

gpio.o:	gpio.c gpio.h
	$(CC) $(CFLAGS) -c gpio.c

systick.o:	systick.c systick.h
	$(CC) $(CFLAGS) -c systick.c

softspi.o:	softspi.c softspi.h
	$(CC) $(CFLAGS) -c softspi.c

button.o:	button.c button.h device_config.h
	$(CC) $(CFLAGS) -c button.c

encoder.o:	encoder.c encoder.h device_config.h
	$(CC) $(CFLAGS) -c encoder.c

keypad.o:	keypad.c keypad.h device_config.h
	$(CC) $(CFLAGS) -c keypad.c

lcd_44780.o:	lcd_44780.c lcd_44780.h device_config.h
	$(CC) $(CFLAGS) -c lcd_44780.c



datefile.txt:
	date -u +%Y%m%d%H%M%S >datefile.txt




avrlib.hex: avrlib.elf
	avr-objcopy -j .text -j .data -O ihex avrlib.elf  avrlib.hex

avrlib.flash:	avrlib.hex
	avrdude -cavrisp -v -pm8 -P/dev/ttyUSB0 -b19200 -Uflash:w:avrlib.hex:i

fuse:
	avrdude -cavrisp  -pm8 -P/dev/ttyUSB0 -b19200 -U lfuse:w:0xef:m -U hfuse:w:0xd9:m 

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)
################################################################################
# this will create an ELF file!
#lcbdk:  lcbdk.o lcd_44780.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o lcbdk lcbdk.o lcd_44780.o $(LDFLAGS) $(LIBS)

#lcbdk.o:        lcbdk.c 
#	$(CC) $(CFLAGS) -c lcbdk.c


#all: $(PRG).elf lst text eeprom

#$(PRG).elf: $(OBJ)
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

# dependency:
#demo.o: demo.c iocompat.h

#template_clean:
#	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak 
#	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

#lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images
#myhex: $(PRG)
#	$(OBJCOPY) -j .text -j .data -O ihex $(PRG) $(PRG).hex

text: hex bin srec

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec

ehex:  $(PRG)_eeprom.hex
ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@ \
	|| { echo empty $@ not generated; exit 0; }

%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@ \
	|| { echo empty $@ not generated; exit 0; }

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@ \
	|| { echo empty $@ not generated; exit 0; }

# Every thing below here is used by avr-libc's build system and can be ignored
# by the casual user.

FIG2DEV                 = fig2dev
EXTRA_CLEAN_FILES       = *.hex *.bin *.srec

dox: eps png pdf

eps: $(PRG).eps
png: $(PRG).png
pdf: $(PRG).pdf

%.eps: %.fig
	$(FIG2DEV) -L eps $< $@

%.pdf: %.fig
	$(FIG2DEV) -L pdf $< $@

%.png: %.fig
	$(FIG2DEV) -L png $< $@


flash_write:    $(PRG).hex
#                avrdude -cstk500v1 -v -patmega8 -P/dev/ttyUSB0 -b19200 -Uflash:w:$(PRG).hex:i

#flash:		$(PRG).hex
#		avrdude -cavrisp -v -pm8 -P/dev/ttyUSB0 -b19200 -Uflash:w:$(PRG).hex:i


lchex:
	avr-objcopy -j .text -j .data -O ihex lcbdk lcbdk.hex

lcflash:
	avrdude -cavrisp -v -pm8 -P/dev/ttyUSB0 -b19200 -Uflash:w:lcbdk.hex:i

lcfuse:
	avrdude -cavrisp  -pm8 -P/dev/ttyUSB0 -b19200 -U lfuse:w:0xef:m -U hfuse:w:0xd9:m 

