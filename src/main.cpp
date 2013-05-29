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

volatile int ticks;
void ticker(void)
{
   ++ticks;
} END_OF_FUNCTION(ticker);

LOCK_FUNCTION(ticker);
LOCK_VARIABLE(ticks);





int main(int argc, char *argv[]) {


    int selectedheight = 2;
    int selectedrate = TOTALRATES-1;
    int selectedwave[PERVOICE] = {0};
    int adsr[PERVOICE*ADSR_STATES] = {1};
    char * adsrname[] = {"Attack","Decay","Sustain","Release"};



    char * filtername[] = {"Low-Pass","ADSR"};
    int totalfilters = 2, selectedfilter = 0;

    //Retrieve information about synthesizer

    char ** wavename = get_wavenames();
    char ** heightname = get_heightnames();
    unsigned int * freqtable = get_frequency_table();

    print_lookup (0, 3, samplemult[3]);


    string inputstr = "";
    int sizex = 1000, sizey = 610;

    int running = 1;

    //Generate Wavetables


    //initialize graphical system
    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();
    srand(time(NULL));

    printf("\n\n%i %i %i\n\n",KEY_A, KEY_S, KEY_W);

    printf("Initializing screen...");

    set_color_depth(32);
    int ret = set_gfx_mode( GFX_AUTODETECT_WINDOWED, sizex, sizey, sizex, sizey );
    if (ret != 0) {
        allegro_message( allegro_error );
        return NULL;
    }
    set_window_title("Function Generator Simulation");
    BITMAP * buffer = create_bitmap(SCREEN_W, SCREEN_H);
    PALLETE pal;
    get_pallete(pal);
    show_mouse(screen);

    printf("DONE\n");




    //-------------------------initialize mouse input---------------------------------
    int click[2], mousex, mousey, arrowed = 0;
    AUDIOSTREAM * stream = initialize_soundsystem();



    int vol = UNITY_7b;
    int rc = 30;
    int dist = 0;
    int f2 = 0, f2counter = 0, f2active = 0;

    oscillator * osc = get_oscillator_pointer();

    int * note;

    install_int_ex(ticker, BPS_TO_TIMER(35));





    while (running) {

        rectfill(buffer, 0, 0, SCREEN_W-1, SCREEN_H-1, BLACK);

        // ------------ USER INPUT ------------
        poll_keyboard();
        poll_mouse();


        //should program continue running?
        if (key[KEY_ESC]) running = 0;




        if (key[KEY_DOWN]) {        //bit depth control
            if (!arrowed) {
                arrowed = 1;
                selectedheight--;
                if (selectedheight < 0) selectedheight = 0;
            }
        } else if (key[KEY_UP]) {
            if (!arrowed) {
                arrowed = 1;
                selectedheight++;
                if (selectedheight >= TOTALBITS) selectedheight = TOTALBITS-1;
            }
        } else if (key[KEY_LEFT]) {  //samplerate control
            if (!arrowed) {
                arrowed = 1;
                selectedrate--;
                if (selectedrate < 0) selectedrate = 0;
            }
        } else if (key[KEY_RIGHT]) {
            if (!arrowed) {
                arrowed = 1;
                selectedrate++;
                if (selectedrate >= TOTALRATES) selectedrate = TOTALRATES-1;
            }
        } else {
            arrowed = 0;
        }


        note = note_controller(buffer);
        get_mouseinput(click, &mousex, &mousey);

        //display parameter menus
        int oldactive = 0;
        for (int i = 0; i < OSCILLATORS; i++) {

            oldactive = osc[i].active;
            osc[i].attack = adsr[0+i%2*ADSR_STATES];
            osc[i].decay = adsr[1+i%2*ADSR_STATES];
            osc[i].sustain = adsr[2+i%2*ADSR_STATES];
            osc[i].release = adsr[3+i%2*ADSR_STATES];

            if (note[i] >= 0 && note[i] < NOTES && selectedwave[i%PERVOICE] < TOTALWAVES) {
                osc[i].active = 1;
                osc[i].frequency = freqtable[note[i]];
            } else {
                osc[i].active = 0;
            }

            osc[i].selectedWaveform = selectedwave[i%PERVOICE];
            osc[i].distortion = dist;


            if (osc[i].active) {
                //if new oscillator, reset
                if (oldactive == 0) {
                    osc[i].level = 0;
                    osc[i].ADSRstate = 0;
                }

            } else {
                osc[i].targetLevel = 0;
                osc[i].ADSRstate = RELEASE;
            }


        }



        //Run synthesizer
        run_synthesizer(buffer, vol, rc, selectedrate, selectedheight);




        int menuy = 10;
        int menux = 10;


        for (int i = 0; i < PERVOICE; i++) {
            set_menu_cursor(menux+i*(MENU_W+40), menuy);

            display_menu (buffer, 1, MENU_VERTICAL, click, TOTALWAVES, &selectedwave[i], wavename, "Oscillator");

            display_menu (buffer, 1, MENU_SLIDER, click, UNITY_7b, &adsr[0+(i%2)*ADSR_STATES], wavename, adsrname[0]);
            display_menu (buffer, 1, MENU_SLIDER, click, UNITY_7b, &adsr[1+(i%2)*ADSR_STATES], wavename, adsrname[1]);
            display_menu (buffer, 1, MENU_SLIDER, click, UNITY_FP, &adsr[2+(i%2)*ADSR_STATES], wavename, adsrname[2]);
            display_menu (buffer, 1, MENU_SLIDER, click, UNITY_7b, &adsr[3+(i%2)*ADSR_STATES], wavename, adsrname[3]);

        }


        set_menu_cursor_x(menux);

        display_menu (buffer, 0, MENU_HORIZONTAL, click, TOTALBITS, &selectedheight, heightname, "Bit Depth:");

        inputstr = "Frequency: ";

        display_menu (buffer, 1, MENU_SLIDER, click, UNITY_7b*2, &vol, NULL, "Level:");
        display_menu (buffer, 1, MENU_SLIDER, click, UNITY_7b, &rc, NULL, "RC:");
        display_menu (buffer, 1, MENU_SLIDER, click, UNITY_FP, &dist, NULL, "Distortion:");




        if (key[KEY_F2]) {
            if (f2 == 0)
                save_bmp("screeny.bmp", buffer, pal);
            f2 = 1;
            f2active = 1;
        } else {
            f2 = 0;
        }

        if (!f2active) {
            textprintf_right_ex(buffer, font, SCREEN_W-TEXTHEIGHT, SCREEN_H-TEXTHEIGHT-1, LIGHTBLUE, -1, "Press F2 to save a screenshot!");
            f2counter = 0;
        } else {
            textprintf_right_ex(buffer, font, SCREEN_W-TEXTHEIGHT, SCREEN_H-TEXTHEIGHT-1, LIGHTBLUE, -1, "Screenshot saved!");
            f2counter++;
            if (f2counter > 50)
                f2active = 0;
        }

        textprintf_centre_ex(buffer, font, SCREEN_W/2, SCREEN_H-TEXTHEIGHT-1, LIGHTBLUE, -1, "Copyright 2010 by Brett Weir");



        blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);

        while (ticks<1) {
            yield_timeslice();
            rest(20);
        }
        ticks=0;


    }

    readkey();

    stop_audio_stream(stream);
    destroy_bitmap(buffer);

    set_gfx_mode( GFX_TEXT, 0, 0, 0, 0);
    printf("Press any key to quit.\n");

    allegro_exit();
    return 0;
}
END_OF_MAIN()
