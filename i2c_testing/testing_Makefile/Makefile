SRC=main
LIBS=USART.c I2C.c SH1106.c
HEDS=USART.h I2C.h SH1106.h defs.h 5x7font.h

GCC=avr-gcc
AVRDUDE=sudo avrdude

#-- avrdude command
PROGEAMMER=avrisp2
AVRDFLAGS=-c $(PROGEAMMER) -p m328p -U flash:w:$(SRC).hex


#-- configurations for ATmega328P
MCU=-mmcu=atmega328p #-- CPU
CPU_SPEED=-DF_CPU=8000000UL #-- clock speed of the board (mentioned on the quartz)
CFLAGS=$(MCU) $(CPU_SPEED) -Os -w -Wl,--gc-sections -ffunction-sections -fdata-sections

.PHONY:
build: $(SRC).hex

$(SRC).hex: $(SRC).elf
	echo "first prerec " $<
	avr-objcopy -O ihex $< $@

$(SRC).elf: $(SRC).c $(LIBS) $(HEDS) 
	echo "dependencies"$^
	echo "target" $@
	$(GCC) $(CFLAGS) $(SRC).c $(LIBS) -o $@
	avr-size -C $(SRC).elf

.PHONY:
clean:
	@echo Cleaning ...
	rm -f $(SRC).elf
	rm -f $(SRC).hex
	rm -f *.o
	@echo "done"
	
.PHONY:
burn:
	$(AVRDUDE) $(AVRDFLAGS)
	
	
#--NOTES
#--echo "dependencies"$^
#--echo "target" $@
#--echo "first prerec " $<
	


