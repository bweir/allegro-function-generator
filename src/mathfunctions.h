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
unsigned int get_phase(unsigned int n, int selectedrate);

float UnitStep(float t, float tau);
int UnitStep(int t, int tau);

const char * int2bin_8b (Uint8 number);
const char * int2bin_16b (Uint16 number);
const char * int2bin_32b (Uint32 number);

void calculate_DFT (int samples, short int * inputwave, unsigned short int * outputwave);
