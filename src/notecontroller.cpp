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

int noteon[NOTES] = {0}, oldnoteon[NOTES] = {0};
int notestart = 50;
int note[OSCILLATORS] = {0};
int notecount = 0;
int noteinit = 0;
int activeosc = 0;

int keyspacecnt = 0;

int redx1=0, redx2=0;
int keyx1, keyx2, keyy1, keyy2;



int * note_controller (BITMAP * buffer) {

        //Control display frequency
        if (key[KEY_PGUP]) notestart += 1;
        if (key[KEY_PGDN]) notestart -= 1;
        if (notestart < 0) notestart = 0;
        if (notestart >= NOTES-14) notestart = NOTES-15;


        for (int i = 0; i < NOTES; i++)
            noteon[i] = 0;

        if (key[KEY_A]) noteon[(notestart)%NOTES] = 1;
        if (key[KEY_W]) noteon[(notestart+1)%NOTES] = 1;
        if (key[KEY_S]) noteon[(notestart+2)%NOTES] = 1;
        if (key[KEY_E]) noteon[(notestart+3)%NOTES] = 1;
        if (key[KEY_D]) noteon[(notestart+4)%NOTES] = 1;

        if (key[KEY_F]) noteon[(notestart+5)%NOTES] = 1;
        if (key[KEY_T]) noteon[(notestart+6)%NOTES] = 1;
        if (key[KEY_G]) noteon[(notestart+7)%NOTES] = 1;
        if (key[KEY_Y]) noteon[(notestart+8)%NOTES] = 1;
        if (key[KEY_H]) noteon[(notestart+9)%NOTES] = 1;
        if (key[KEY_U]) noteon[(notestart+10)%NOTES] = 1;

        if (key[KEY_J]) noteon[(notestart+11)%NOTES] = 1;
        if (key[KEY_K]) noteon[(notestart+12)%NOTES] = 1;
        if (key[KEY_L]) noteon[(notestart+14)%NOTES] = 1;


        if (!noteinit) {
            noteinit = 1;
            for (int i = 0; i < OSCILLATORS; i++)
                note[i] = -1;
        }




        //DRAWS KEYBOARD (mainly)

        int keyboardx = 40, keyboardy = SCREEN_H-100;
        int keyw = 12, keyh = 50, bkeyh = 30;
        int dotlocx = keyboardx+10, dotlocy = keyboardy+TEXTHEIGHT;
        int voicex = dotlocx, voicey = keyboardy-10-keyw;
        //int spacer = keyw+1;
        int spacer = keyw/2;
        int oscspacer = 25;




        //Draws the oscillator indicators
        activeosc = 0;
        for (int i = 0; i < OSCILLATORS; i++) {
            if (note[i] > -1) {
                rectfill(buffer, voicex+i*oscspacer, voicey, voicex+(i+1)*oscspacer-2, voicey+keyw-1, BLUE);
                activeosc++;
            } else
                rectfill(buffer, voicex+i*oscspacer, voicey, voicex+(i+1)*oscspacer-2, voicey+keyw-1, GREY);


            if (!noteon[note[i]] && oldnoteon[note[i]])
                note[i] = -1;
        }


        //draw notestart
        //indicates current available keys

        //WHITE NOTES
        keyspacecnt = 0;
        keyy1 = dotlocy;
        keyy2 = dotlocy+keyh-1;
        for (int i = 0; i < NOTES; i++) {

            int ack = i % 12;
            if (!(ack == 1 || ack == 4 || ack == 6 || ack == 9 || ack == 11)) {

                keyx1 = dotlocx+keyspacecnt*spacer+1;
                keyx2 = dotlocx+keyspacecnt*spacer+keyw-2;

                if (noteon[i]) {
                    rectfill(buffer, keyx1, keyy1, keyx2, keyy2, GREY);
                    hline(buffer, keyx1, keyy2, keyx2, DARKGREY);
                    vline(buffer, keyx1, keyy1, keyy2, DARKGREY);

                } else {
                    rectfill(buffer, keyx1, keyy1, keyx2, keyy2, WHITE);
                    hline(buffer, keyx1, keyy2, keyx2, GREY);
                    vline(buffer, keyx1, keyy1, keyy2, GREY);
                }

                if (i == notestart) redx1 = keyx1;
                if (i == notestart+14) redx2 = keyx2;

                //if (noteon[i]) rectfill(buffer, keyx1+1, keyy2-keyw+1, keyx2-1, keyy2-1, BLUE);
                keyspacecnt+=2;
            }
        }

        //BLACK NOTES
        keyspacecnt = 1;
        keyy1 = dotlocy;
        keyy2 = dotlocy+bkeyh-1;
        for (int i = 0; i < NOTES; i++) {
            int ack = i % 12;
            if (ack == 1 || ack == 4 || ack == 6 || ack == 9 || ack == 11) {

                keyx1 = dotlocx+keyspacecnt*spacer+1;
                keyx2 = dotlocx+keyspacecnt*spacer+keyw-2;


                if (noteon[i]) {
                    rectfill(buffer, keyx1, keyy1, keyx2, keyy2, REALLYGREY);
                    hline(buffer, keyx1, keyy1, keyx2, GREY);
                    vline(buffer, keyx2, keyy1, keyy2, GREY);
                    hline(buffer, keyx1, keyy2, keyx2, BLACK);
                    vline(buffer, keyx1, keyy1, keyy2, BLACK);

                } else {
                    rectfill(buffer, keyx1, keyy1, keyx2, keyy2, DARKGREY);
                    hline(buffer, keyx1, keyy1, keyx2, WHITE);
                    vline(buffer, keyx2, keyy1, keyy2, WHITE);
                    hline(buffer, keyx1, keyy2, keyx2, BLACK);
                    vline(buffer, keyx1, keyy1, keyy2, BLACK);
                }




                if (i == notestart) redx1 = keyx1;
                if (i == notestart+14) redx2 = keyx2;

//                if (noteon[i]) rectfill(buffer, keyx1+1, keyy2-keyw+1, keyx2-1, keyy2-1, BLUE);
                if (ack == 1 || ack == 6)
                    keyspacecnt+=4;
                else
                    keyspacecnt+=2;
            }
        }

        //RED BOX
        rectfill(buffer, redx1, dotlocy+keyh+1, redx2, dotlocy+keyh+5, RED);


        for (int i = 0; i < NOTES; i++) {
            //indicates what notes are being pressed
            if (noteon[i]) {

                if (!oldnoteon[i]) {
                    for (int j = 0; j < PERVOICE; j++) {
                        int osccount = 0;

                        while (note[notecount%OSCILLATORS] > 0 && osccount < OSCILLATORS) {
                            osccount++;
                            notecount++;
                        }
                        note[notecount%OSCILLATORS] = i;
                        notecount++;
                    }
                }

            }
            oldnoteon[i] = noteon[i];
        }

        textprintf_ex(buffer, font, keyboardx+2, keyboardy+2, LIGHTBLUE, -1, "Triggered Oscillators (%i/16):", activeosc);
        textprintf_ex(buffer, font, keyboardx+2, keyboardy+2-10-keyw*3, LIGHTBLUE, -1, "Keyboard:");



    return note;
}
