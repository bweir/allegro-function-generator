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


void plot_waveform(BITMAP * buffer, int plotx, int ploty, int * w, int * h, int height, int samples,
                    int selectedn, int nlimit, short int * pointy, const char * wavename) {

        int n, pointx1, pointx2, pointy1;

        if (nlimit < samples) n = 0; else {
            n = nlimit - samples;
        }
        if (*w < *h) *w = *h;

        int plotw = *w;
        int ploth = *h;

        rect(buffer, plotx-1, ploty-1, plotx+plotw, ploty+ploth,DARKBLUE);
        hline(buffer, plotx, ploty+ploth/2, plotx+plotw-1, BLUE);
        textprintf_ex(buffer, font, plotx+4, ploty-TEXTHEIGHT, LIGHTBLUE, -1, "%s", wavename);

        textprintf_ex(buffer, font, plotx+4, ploty+ploth+4, LIGHTBLUE, -1, "bin %s int %i",int2bin_16b(pointy[nlimit%SAMPLES]),pointy[nlimit%SAMPLES]);

        while (n < nlimit) {

            pointx1 = plotx+plotw*(samples-nlimit+n)/samples;
            pointx2 = plotx+plotw*(samples-nlimit+n+1)/samples;
            pointy1 = ploty+(1)*ploth/2;
            pointy1 -= ploth*pointy[n%samples]/(height-2);

            if (n == selectedn) rectfill(buffer, pointx1, ploty, pointx2, ploty+ploth-1, DARKRED);

            vline(buffer, pointx1, ploty+ploth/2, pointy1, DARKBLUE);
            hline(buffer, pointx1, pointy1, pointx2, LIGHTBLUE);

            n++;
        }


        textprintf_right_ex(buffer, font, plotx, ploty-TEXTHEIGHT/2, LIGHTBLUE, -1, "%i", height/2-1);
        textprintf_right_ex(buffer, font, plotx, ploty+ploth/2-TEXTHEIGHT/2, LIGHTBLUE, -1, "%i", 0);
        textprintf_right_ex(buffer, font, plotx, ploty+ploth-TEXTHEIGHT/2, LIGHTBLUE, -1, "%i", -height/2+1);
        textprintf_ex(buffer, font, plotx+plotw+10, ploty+ploth/2-TEXTHEIGHT/2, LIGHTBLUE, -1, "%.1fms", ((float)samples)*TSMAX*1000);

}



void plot_FFT(BITMAP * buffer, int plotx, int ploty, int * w, int * h, int height, int samples,
                    int selectedn, int nlimit, unsigned short int * pointy, const char * wavename) {

        int n, pointx1, pointx2, pointy1;

        if (nlimit < samples) n = 0; else {
            n = nlimit - samples;
        }
        if (*w < *h) *w = *h;

        int plotw = *w;
        int ploth = *h;



        rect(buffer, plotx-1, ploty-1, plotx+plotw, ploty+ploth,DARKBLUE);
        hline(buffer, plotx, ploty+ploth, plotx+plotw-1, BLUE);
        textprintf_ex(buffer, font, plotx+4, ploty-TEXTHEIGHT, LIGHTBLUE, -1, "%s", wavename);

        textprintf_ex(buffer, font, plotx+4, ploty+ploth+4, LIGHTBLUE, -1, "bin %s int %i",int2bin_16b(pointy[nlimit%SAMPLES]),pointy[nlimit%SAMPLES]);

        while (n < nlimit) {

            pointx1 = plotx+plotw*(samples-nlimit+n)/samples;
            pointx2 = plotx+plotw*(samples-nlimit+n+1)/samples;
            pointy1 = ploty+ploth;
            pointy1 -= ploth*pointy[n%samples]/(height-2);

            if (n == selectedn) rectfill(buffer, pointx1, ploty, pointx2, ploty+ploth-1, DARKRED);

            vline(buffer, pointx1, ploty+ploth, pointy1, DARKBLUE);
            hline(buffer, pointx1, pointy1, pointx2, LIGHTBLUE);

            n++;
        }


        textprintf_right_ex(buffer, font, plotx, ploty-TEXTHEIGHT/2, LIGHTBLUE, -1, "%i", height/2-1);
        textprintf_right_ex(buffer, font, plotx, ploty+ploth/2-TEXTHEIGHT/2, LIGHTBLUE, -1, "%i", 0);
        textprintf_right_ex(buffer, font, plotx, ploty+ploth-TEXTHEIGHT/2, LIGHTBLUE, -1, "%i", -height/2+1);
        textprintf_ex(buffer, font, plotx+plotw, ploty+ploth/2-TEXTHEIGHT/2, LIGHTBLUE, -1, "pi");

}









