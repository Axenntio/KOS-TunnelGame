include .knightos/variables.make

HEADERS:=$(wildcard *.h)
IMAGES:=$(wildcard img/*.png)
OBJECTS=$(OUT)crt0.o $(patsubst %.c,$(OUT)%.o,$(wildcard *.c))
OBJECTS+=$(patsubst img/%.png,$(OUT)img/%.o, $(IMAGES))
INCLUDE:=$(INCLUDE) -I include/

ALL_TARGETS:=$(BIN)test

$(OUT)img/%.bin: img/%.png
	mkdir -p $(OUT)img
	kimg -m -b $< $@

$(OUT)img/%.o: $(OUT)img/%.bin
	scwrap $< $@ $$(basename -s .bin $<)_sprite

$(OUT)test.o: $(OBJECTS)
	mkdir -p $(OUT)
	scas $(ASFLAGS) -c $(OBJECTS) $(LIBRARIES) -o $(OUT)test.o

$(BIN)test: $(OUT)test.o
	mkdir -p $(BIN)
	scas $(ASFLAGS) $(OUT)test.o -o $(BIN)test

$(APPS)test.app: config/test.app
	mkdir -p $(APPS)
	cp config/test.app $(APPS)

$(SHARE)icons/test.img: config/test.png
	mkdir -p $(SHARE)icons
	kimg -c config/test.png $(SHARE)icons/test.img

include .knightos/sdk.make
