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


float pitch (float Fnaught, int n) {
    return Fnaught*pow(2,((float) n)/12.0);
}


float fFrequency[NOTES];
unsigned int iFrequency[NOTES];
int freqdefined = 0;

unsigned int * generate_frequency_table (void) {

    for(int i = 0; i < NOTES; i++) {
        fFrequency[i] = pitch(fFCMIN, i);
        iFrequency[i] = (unsigned int) (pow(2.0,16.0)*fFrequency[i]);


    }
    return iFrequency;
}

unsigned int * get_frequency_table(void) {
    if (!freqdefined) {
        freqdefined = 1;
        return generate_frequency_table();
    } else return iFrequency;
}

