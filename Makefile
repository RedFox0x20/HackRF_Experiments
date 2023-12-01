.PHONY: all

LIBS:=-lm -lhackrf

all: FM_TONE FM_FSK FSK MFSK SWEEP SINESWEEP DEVICES

clean:
	rm -rf ./build/*

FM_TONE: 
	gcc -o build/FM_SingleTone Tests/FM_SingleTone.c $(LIBS)

FM_FSK: 
	gcc -o build/FM_FSK Tests/FM_FSK.c $(LIBS)
	
FSK:
	gcc -o build/FSK Tests/FSK.c $(LIBS)

MFSK:
	gcc -o build/MFSK Tests/MFSK.c $(LIBS)

SWEEP:
	gcc -o build/Sweep Tests/Sweep.c $(LIBS)

SINESWEEP:
	gcc -o build/SineSweep Tests/SineSweep.c $(LIBS)

DEVICES:
	gcc -o build/ListDevices Tests/ListDevices.c $(LIBS)
