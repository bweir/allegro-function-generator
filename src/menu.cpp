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

int pointx = 5;
int pointy = 60;

void set_menu_cursor (int menulocation_x, int menulocation_y) {
    pointx = menulocation_x;
    pointy = menulocation_y;
}


void set_menu_cursor_x (int menulocation_x) {
    pointx = menulocation_x;
}

void set_menu_cursor_y (int menulocation_y) {
    pointy = menulocation_y;
}


void display_menu (BITMAP * buffer, int power, int type, int * click, int totalchoices, int * selectedchoice, char ** choicename, char * title) {

    int x = pointx, y = pointy;
    pointy += TEXTHEIGHT;

    int menux = x+TEXTHEIGHT, menuy = y+TEXTHEIGHT;
    int dotr = (TEXTHEIGHT-4)/2;

    if (click[0]) {
        if (mouse_x >= x && mouse_x < x+2*dotr && mouse_y >= y && mouse_y < menuy+2*dotr) {
            if (*selectedchoice != totalchoices) {
                *selectedchoice = totalchoices;
            } else {
                *selectedchoice = 0;
            }
        }
    }

    //Power button
    if (power) {
        if (*selectedchoice != totalchoices)
            circlefill(buffer, x+dotr, y+dotr, dotr, RED);
        else
            circlefill(buffer, x+dotr, y+dotr, dotr, DARKRED);
    }


    textprintf_ex(buffer, font, x+TEXTHEIGHT+MENU_PADDING, y+MENU_PADDING, LIGHTBLUE, -1, "%s", title);




    if (type == MENU_VERTICAL) {


        for (int index = 0; index < totalchoices; index++) {

            pointy += TEXTHEIGHT;


            if (click[0]) {
                if (mouse_x >= menux && mouse_x < menux+MENU_W && mouse_y >= menuy+index*TEXTHEIGHT && mouse_y < menuy+MENU_H+index*TEXTHEIGHT) {
                    *selectedchoice = index;
                }
            }

            if (index == *selectedchoice) {
                rectfill(buffer, menux, menuy+index*TEXTHEIGHT, menux+MENU_W-1, menuy+index*TEXTHEIGHT+MENU_H-1, LIGHTBLUE);
                textprintf_ex(buffer, font, menux+MENU_PADDING, menuy+index*TEXTHEIGHT+MENU_PADDING, BLACK, -1, "%i. %s", index+1, choicename[index]);
            } else {
                rect(buffer, menux, menuy+index*TEXTHEIGHT, menux+MENU_W-1, menuy+index*TEXTHEIGHT+MENU_H-1, LIGHTBLUE);
                textprintf_ex(buffer, font, menux+MENU_PADDING, menuy+index*TEXTHEIGHT+MENU_PADDING, LIGHTBLUE, -1, "%i. %s", index+1, choicename[index]);
            }

        }

    }

    if (type == MENU_HORIZONTAL) {

        pointy += TEXTHEIGHT;

        for (int index = 0; index < totalchoices; index++) {


            if (click[0]) {
                if (mouse_x >= menux+index*TEXTWIDTH && mouse_x < menux+(index+1)*TEXTWIDTH && mouse_y >= menuy && mouse_y < menuy+MENU_H) {
                    *selectedchoice = index;
                }
            }

            if (index == *selectedchoice) {
                rectfill(buffer, menux+index*TEXTWIDTH, menuy, menux+(index+1)*TEXTWIDTH-1, menuy+MENU_H-1, LIGHTBLUE);
                textprintf_ex(buffer, font, menux+index*TEXTWIDTH+MENU_PADDING, menuy+MENU_PADDING, BLACK, -1, "%s", choicename[index]);
            } else {
                rect(buffer, menux+index*TEXTWIDTH, menuy, menux+(index+1)*TEXTWIDTH-1, menuy+MENU_H-1, LIGHTBLUE);
                textprintf_ex(buffer, font, menux+index*TEXTWIDTH+MENU_PADDING, menuy+MENU_PADDING, LIGHTBLUE, -1, "%s", choicename[index]);
            }

        }
    }


    if (type == MENU_SLIDER) {

        pointy += TEXTHEIGHT;
        int value_x = MENU_W*(*selectedchoice)/totalchoices;

        rectfill(buffer, menux, menuy+TEXTHEIGHT/2-1, menux+MENU_W-1, menuy+TEXTHEIGHT/2+1, DARKBLUE);
        rectfill(buffer, menux+value_x-1, menuy+1, menux+value_x+1, menuy+TEXTHEIGHT-1, LIGHTBLUE);


        if (click[0]) {
            if (mouse_x >= menux && mouse_x < menux+MENU_W && mouse_y >= menuy && mouse_y < menuy+MENU_H) {
                *selectedchoice = ((mouse_x-menux)*totalchoices)/MENU_W;
            }
        }



     }

    pointy += TEXTHEIGHT;


}
