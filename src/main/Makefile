# Sensitive to these environment variables
TARGET ?= avr-
AVRDUDE_PORT ?= /dev/ttyACM0


PROG_NAME := main
OBJ_FILES := \
	main.o


# The programs needed for compiling and uploading
CC := $(TARGET)gcc
LD := $(TARGET)gcc
OBJCOPY := $(TARGET)objcopy
AVRDUDE := avrdude

# Flags to pass to the programs
CFLAGS := \
	-v -Os -mmcu=atmega328p -Wall -Werror \
	-ffunction-sections -fdata-sections \
	-DF_CPU=16000000
LDFLAGS := \
	-v -mmcu=atmega328p \
	-Wl,--orphan-handling=warn,-gc-sections
OBJCOPYFLAGS := \
	-O ihex
AVRDUDEFLAGS := \
	-p m328p -c arduino -P $(AVRDUDE_PORT)

# Includes and libraries needed
INC :=
LIB :=


.PHONY : all
all : $(PROG_NAME).hex

.PHONY : upload
upload : all
	$(AVRDUDE) $(AVRDUDEFLAGS) -U "flash:w:$(PROG_NAME).hex:i"

.PHONY : clean
clean :
	find . -name "*.o" -delete -print
	find . -name "*.elf" -delete -print
	find . -name "*.hex" -delete -print

%.hex : %.elf
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

%.elf :
	$(LD) $(LDFLAGS) $(LIB) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^


$(PROG_NAME).elf : $(OBJ_FILES)
