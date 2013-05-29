/*
    This file is part of the Allegro Function Generator.
    Copyright (C) 2010 by Brett Weir 
  
    The Allegro Function Generator is free software: you can redistribute 
    it and/or modify it under the terms of the GNU General Public License 
    as published by the Free Software Foundation, either version 3 of the 
    License, or (at your option) any later version.

    The Allegro Function Generator is distributed in the hope that it will 
    be useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Allegro Function Generator.  If not, see 
    <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string>

#include <allegro.h>

#ifdef _WIN32 || _WIN64
#include <winalleg.h>
#endif

using namespace std;


#ifndef DATATYPES
#define DATATYPES

typedef unsigned char Uint8;
typedef unsigned short int Uint16;
typedef unsigned int Uint32;

typedef signed char Sint8;
typedef signed short int Sint16;
typedef signed int Sint32;

typedef struct packed_int {
		 unsigned char b0;
		 unsigned char b1;
		 unsigned char b2;
		 unsigned char b3;
} packed_int;

typedef union {
	unsigned int i;
	packed_int b;
} packed;

#endif







#define PI                  (3.14159265)
#define UNITY_FP            ((1 << 15)-1)
#define UNITY_7b            ((1<<7)-1)

#define BLACK               makecol(0,0,0)
#define REALLYGREY          makecol(40,40,40)
#define DARKGREY            makecol(80,80,80)
#define GREY                makecol(150,150,150)
#define WHITE               makecol(255,255,255)
#define GREEN               makecol(0,255,0)
#define RED                 makecol(255,0,0)
#define DARKRED             makecol(150,0,0)
#define BLUE                makecol(0,0,255)
#define DARKBLUE            makecol(0,0,125)
#define LIGHTBLUE           makecol(200,200,255)

#define TEXTHEIGHT          12
#define TEXTWIDTH           18


#define TOTALWAVES          5
#define TOTALBITS           4
#define TOTALRATES          4

///Frequency definitions
#define FSMAX             (14080*2)
//#define FSMAX               (44100)
//#define FSMAX             pow(14080,100)



#define FCMIN               55
#define FCMAX               (FSMAX/2)
#define FCMAX_SHIFTED       (FCMAX << 16)

#define fFSMAX              ((float) FSMAX)
#define fFCMIN              ((float) FCMIN)

#define WSMAX               (2.0*PI*((float) FSMAX))
#define WCMIN               (2.0*PI*((float) FCMIN))

#define TSMAX               (1.0/((float) FSMAX))
#define TCMIN               (1.0/((float) FCMIN))



#define SAMPLES             (FSMAX/FCMIN)
#define SHIFTEDSAMPLES      ((FSMAX<<16)/FCMIN)
#define fSAMPLES            (fFSMAX/fFCMIN)

#define PLOTSAMPLES         128
#define FFTSAMPLES          (SAMPLES/2)

#define wavetableindex(wave,bits,sample)   ((TOTALBITS*SAMPLES*(wave%TOTALWAVES))+(SAMPLES*(bits%TOTALBITS))+(sample%SAMPLES))
#define twiddleIndex(k,n)       (((k)%SAMPLES)*SAMPLES+(n)%SAMPLES)



/// Menu stuff
#define MENU_VERTICAL       1
#define MENU_HORIZONTAL     2
#define MENU_SLIDER         3

#define MENU_PADDING        2
#define MENU_W              100
#define MENU_H              TEXTHEIGHT

#define PLOT_SPACING_X      50
#define PLOT_SPACING_Y      50



/// AUDIO STUFF



///frequencytiming
#define NOTES               128

#define ADSR_STATES         4

#define ATTACK              0
#define DECAY               1
#define SUSTAIN             2
#define RELEASE             3
#define OFF                 4






#include "mathfunctions.h"
#include "wavetable.h"
#include "frequencytiming.h"
#include "input.h"
#include "notecontroller.h"
#include "menu.h"
#include "plotter.h"
#include "oscillator.h"
#include "synthesizer.h"







