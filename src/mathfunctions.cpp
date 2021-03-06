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

const unsigned int samplemult[] = {32, 16, 4, 1};



//Unit Step Definitions
float UnitStep(float t, float tau) {
    if (t < tau) return 0.0; else return 1.0;
}
int UnitStep(int t, int tau) {
    if (t < tau) return 0; else return 1;
}




//Returns Binary String
const char * int2bin_8b (Uint8 number) {
    string str = "00000000";        char temp[8];

    for (int count = 0; count < 8; count++) {
        if (number % 2 == 1) temp[count] = '1'; else temp[count] = '0';
        number /= 2;
    }
    for (int count = 0; count < 8; count++) {str[count] = temp[7-count];}

    //printf("%s\n",str.c_str());

    return str.c_str();
}

const char * int2bin_16b (Uint16 number) {
    string str = "0000000000000000";        char temp[16];

    for (int count = 0; count < 16; count++) {
        if (number % 2 == 1) temp[count] = '1'; else temp[count] = '0';
        number /= 2;
    }
    for (int count = 0; count < 16; count++) {str[count] = temp[15-count];}

    return str.c_str();
}

const char * int2bin_32b (Uint32 number) {
    string str = "00000000000000000000000000000000";        char temp[32];

    for (int count = 0; count < 32; count++) {
        if (number % 2 == 1) temp[count] = '1'; else temp[count] = '0';
        number /= 2;
    }
    for (int count = 0; count < 32; count++) {str[count] = temp[31-count];}

    return str.c_str();
}






int twiddleReal[SAMPLES*SAMPLES];
int twiddleImaginary[SAMPLES*SAMPLES];
int twiddleCalculated = 0;

// Twiddle Factors

// Exp(-2*PI*n*k/SAMPLES) for n,k = 0 to SAMPLES-1
void get_twiddle () {
    twiddleCalculated = 1;

    for (int k = 0; k < SAMPLES; k++) {
        for (int n = 0; n < SAMPLES; n++) {
            twiddleReal[twiddleIndex(k,n)] = (int) ((cos(2.0*PI*((float)n)*((float)k)/fBUFFER_SIZE))*((float) (1<<15)));
            twiddleImaginary[twiddleIndex(k,n)] = (int) ((-sin(2.0*PI*((float)n)*((float)k)/fBUFFER_SIZE))*((float) (1<<15)));

        }
    }
}


void calculate_DFT (int samples, short int * inputwave, unsigned short int * outputwave) {

    if (!twiddleCalculated) get_twiddle();

    int tempReal, tempImaginary, tempMagnitude;
    int processing[samples];
    int processing2[samples];

    for (int k = 0; k < samples; k++) {
        tempReal = 0;
        tempImaginary = 0;


        for (int n = 0; n < samples; n++) {

            tempReal += inputwave[n]*twiddleReal[twiddleIndex(k,n)];
            tempImaginary += inputwave[n]*twiddleImaginary[twiddleIndex(k,n)];


        }

        tempReal = tempReal >> 16;
        tempImaginary = tempImaginary >> 16;

        tempMagnitude = sqrt((tempReal*tempReal) + (tempImaginary*tempImaginary));
        tempMagnitude = (int) ( ((float)tempMagnitude)/PI);
        processing[k] = tempMagnitude;



        //SMOOTHING
    }

        int j, number = 4;
        int min, max;


        for (int i = 0; i < samples; i++) {

            processing2[i] = 0;
            j = i - number;
            if (j < 0) j = 0;

            min = j;
            while (j <= i + number && j < samples) {
                processing2[i] += processing[j];
                j++;
            }
            //max = j-1;
            max = j;

            processing2[i] /= (max-min);

            outputwave[i] = (unsigned) processing2[i];

            //if (outputwave[i] < processing[i]) outputwave[i] = (1 << 15)-1;


        }






}




