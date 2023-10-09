PREFIX=arm-none-eabi-

ARCHFLAGS=-mthumb -mcpu=cortex-m0plus
COMMONFLAGS=-g3 -Og -Wall -Werror $(ARCHFLAGS)

CFLAGS=-I./includes -I./drivers $(COMMONFLAGS)
LDFLAGS=$(COMMONFLAGS) --specs=nano.specs -Wl,--gc-sections,-Map,$(TARGET).map,-Tlink.ld
LDLIBS=

CC=$(PREFIX)gcc
LD=$(PREFIX)gcc
RM=rm -f

TARGETLED=led_blinky
TARGETHELLO=hello_world

SRC=$(wildcard *.c)

SRCLED=$(filter-out hello_world.c, $(filter-out pin_muxHello.c, $(SRC)))
OBJLED=$(patsubst %.c, %.o, $(SRCLED))

SRCHELLO=$(filter-out led_blinky.c, $(filter-out pin_mux.c, $(SRC)))
OBJHELLO=$(patsubst %.c, %.o, $(SRCHELLO))


clean:
	$(RM) $(TARGETLED).srec $(TARGETLED).elf $(TARGETLED).bin $(TARGETLED).map $(OBJLED) $(TARGETHELLO).srec $(TARGETHELLO).elf $(TARGETHELLO).bin $(TARGETHELLO).map $(OBJHELLO)
	
$(TARGETLED).elf: $(OBJLED)
	$(LD) $(LDFLAGS) $(OBJLED) $(LDLIBS) -o $@
	
$(TARGETHELLO).elf: $(OBJHELLO)
	$(LD) $(LDFLAGS) $(OBJHELLO) $(LDLIBS) -o $@
	
flash_led: $(TARGETLED).elf
	openocd -f openocd.cfg -c "program $(TARGETLED).elf verify reset exit"
	
flash_hello: $(TARGETHELLO).elf
	openocd -f openocd.cfg -c "program $(TARGETHELLO).elf verify reset exit"
	
