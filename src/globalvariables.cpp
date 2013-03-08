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

extern const int bitdepth[] = {3, 4, 8, 16};
extern const int height[] = {pow(2,bitdepth[0]), pow(2,bitdepth[1]), pow(2,bitdepth[2]), pow(2,bitdepth[3])};
extern const int samplemult[] = {32, 16, 4, 1};
extern const int samples[] = {SAMPLES/32, SAMPLES/16, SAMPLES/4, SAMPLES};
