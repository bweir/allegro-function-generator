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
#include "functiongenerator.h"

extern const int bitdepth[];
extern const int height[];
extern const int samplemult[];
extern const int samples[];



short int wavetable[TOTALWAVES*TOTALBITS*SAMPLES];
const char * wavename[] = {"Sine","Square","Ramp","Decay","Triangle"};
const char * heightname[] = {"3","4","8","16"};
int wavedefined = 0;





const char ** get_wavenames () {
    return wavename;
}

const char ** get_heightnames () {
    return heightname;
}


short int * get_wavetable () {
    if (!wavedefined) {
        return generate_lookup();
    } else return wavetable;
}









short int * generate_lookup () {

    printf("\nGenerating lookup tables....\n\n");
    printf("\nBit depth%cHeight\n",9);

    for (int numbits = 0; numbits < 4; numbits++) {
        printf("%i%c%c%i\n", bitdepth[numbits], 9, 9, height[numbits]);

        for (int n = 0; n < SAMPLES; n++) {

            //sine
            wavetable[wavetableindex(0,numbits,n)] = (int) ((float) height[numbits]-2.0)/2.0* sin( WCMIN*TSMAX*((float) n));
            //square
            wavetable[wavetableindex(1,numbits,n)] = (int) (-((float) height[numbits]-2.0)/2.0 + ((float) height[numbits] - 2.0)*((float) (UnitStep(n,0) - UnitStep(n, SAMPLES/2))));
            //ramp
            wavetable[wavetableindex(2,numbits,n)] = (int) (-((float) height[numbits] - 2.0)/2.0 + ((float) height[numbits] - 2.0) * (((float) n)*TSMAX/TCMIN));
            //decay
            wavetable[wavetableindex(3,numbits,n)] = (int) (-((float) height[numbits] - 2.0)/2.0 + ((float) height[numbits] - 2.0)*(exp(-4.0*((float) n)/fSAMPLES)));
            //triangle
            wavetable[wavetableindex(4,numbits,n)] = (int) (-((float) height[numbits] - 2.0)/2.0 + ((float) height[numbits] - 2.0)*(
                                                        (2.0*((float) n)/fSAMPLES)
                                                        + ((float) ( UnitStep(n-1,SAMPLES/2) ))*(2.0 - 4.0*((float) n)/fSAMPLES)
                                                    ));

        }
    }

    printf("\n\nLookup tables generated\n\n");


    wavedefined = 1;
    return wavetable;
}




void print_lookup (int wave, int numbits, int samplemul) {

    printf("\nPRINTING WAVETABLE: ");

    if (wavedefined) {

        printf("%s\n",wavename[wave]);

        for (int n = 0; n < SAMPLES/samplemul; n++) {
            if (n % 8 == 0) printf("\nword%c",9); else printf(", ");
            printf("%i",wavetable[wavetableindex(wave,numbits,n*samplemul)]);
        }
    } else {
        printf("Wavetable not defined!");
    }

    printf("\n\n");

}



