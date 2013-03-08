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

        //MOUSE INPUT

void get_mouseinput(int * click, int * mousex, int * mousey) {
        if (mouse_b & 1) {
            click[0] = 1;
        } else {
            click[0] = 0;
        }
        if (mouse_b & 2) {
            click[1] = 1;
        } else {
            click[1] = 0;
        }

        *mousex = mouse_x;
        *mousey = mouse_y;
}
