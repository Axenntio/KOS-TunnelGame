include .knightos/variables.make

HEADERS:=$(wildcard *.h)
IMAGES:=$(wildcard img/*.png)
OBJECTS=$(OUT)crt0.o $(patsubst %.c,$(OUT)%.o,$(wildcard *.c))
OBJECTS+=$(patsubst img/%.png,$(OUT)img/%.o, $(IMAGES))
INCLUDE:=$(INCLUDE) -I include/

ALL_TARGETS:=$(BIN)Tunnel

$(OUT)img/%.bin: img/%.png
	mkdir -p $(OUT)img
	kimg -m -b $< $@

$(OUT)img/%.o: $(OUT)img/%.bin
	scwrap $< $@ $$(basename -s .bin $<)_sprite

$(OUT)Tunnel.o: $(OBJECTS)
	mkdir -p $(OUT)
	scas $(ASFLAGS) -c $(OBJECTS) $(LIBRARIES) -o $(OUT)Tunnel.o

$(BIN)Tunnel: $(OUT)Tunnel.o
	mkdir -p $(BIN)
	scas $(ASFLAGS) $(OUT)Tunnel.o -o $(BIN)Tunnel

$(APPS)Tunnel.app: config/Tunnel.app
	mkdir -p $(APPS)
	cp config/Tunnel.app $(APPS)

$(SHARE)icons/Tunnel.img: config/Tunnel.png
	mkdir -p $(SHARE)icons
	kimg -c config/Tunnel.png $(SHARE)icons/Tunnel.img

include .knightos/sdk.make
