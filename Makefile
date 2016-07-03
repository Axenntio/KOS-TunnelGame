include .knightos/variables.make

HEADERS:=$(wildcard *.h)
IMAGES:=$(wildcard img/*.png)
OBJECTS=$(OUT)crt0.o $(patsubst %.c,$(OUT)%.o,$(wildcard *.c))
OBJECTS+=$(patsubst img/%.png,$(OUT)img/%.o, $(IMAGES))
INCLUDE:=$(INCLUDE) -I include/

ALL_TARGETS:=$(BIN)tunnel $(APPS)tunnel.app $(SHARE)icons/tunnel.img

$(OUT)img/%.bin: img/%.png
	mkdir -p $(OUT)img
	kimg -m -b $< $@

$(OUT)img/%.o: $(OUT)img/%.bin
	scwrap $< $@ $$(basename -s .bin $<)_sprite

$(OUT)tunnel.o: $(OBJECTS)
	mkdir -p $(OUT)
	scas $(ASFLAGS) -c $(OBJECTS) $(LIBRARIES) -o $(OUT)tunnel.o

$(BIN)tunnel: $(OUT)tunnel.o
	mkdir -p $(BIN)
	scas $(ASFLAGS) $(OUT)tunnel.o -o $(BIN)tunnel

$(APPS)tunnel.app: config/tunnel.app
	mkdir -p $(APPS)
	cp config/tunnel.app $(APPS)

$(SHARE)icons/tunnel.img: config/tunnel.png
	mkdir -p $(SHARE)icons
	kimg -c config/tunnel.png $(SHARE)icons/tunnel.img

include .knightos/sdk.make
