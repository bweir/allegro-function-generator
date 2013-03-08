#!/bin/bash

SOURCES=globalvariables.cpp mathfunctions.cpp wavetable.cpp frequencytiming.cpp input.cpp notecontroller.cpp menu.cpp plotter.cpp oscillator.cpp synthesizer.cpp main.cpp


all:
	@echo "Make the Allegro Function Generator\n"
	@echo "     make linux"
	@echo "     make windows\n"

linux:
	mkdir -p bin/
	cd src; g++ -I../include $(SOURCES) `allegro-config --libs` -o ../bin/allegfuncgen

windows:
	
	mkdir -p bin/
	cd src; i586-mingw32msvc-g++ -I../include $(SOURCES) -L/usr/i586-mingw32msvc/lib -Wl,-Bsymbolic-functions -lalleg -o ../bin/allegfuncgen.exe -mwindows

clean:
	rm -rf bin/*	
