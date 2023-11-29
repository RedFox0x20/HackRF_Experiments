.PHONY: all

LIBS:=-lm -lhackrf

all: FM_TONE FM_FSK FSK

clean:
	rm -rf ./build/*

FM_TONE: 
	gcc -o build/FM_SingleTone Tests/FM_SingleTone.c $(LIBS)

FM_FSK: 
	gcc -o build/FM_FSK Tests/FM_FSK.c $(LIBS)
	
FSK:
	gcc -o build/FSK Tests/FSK.c $(LIBS)
