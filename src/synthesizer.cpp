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

bool audio_engine_on = 1, play_audio = 1, old_play_audio = play_audio;
string audiostatus = "Audio works GREAT!";
AUDIOSTREAM * stream = NULL;
unsigned short * audiopointer = NULL;
short int audioprintout[BUFFER_SIZE];
unsigned short int audioFFT[BUFFER_SIZE];




//used for LP filter
//int circularsum[2] = {0};
int circularsum = 0;
int oldselectedheight = 0;
int timeconstant = UNITY_7b*1/10;
unsigned int sampleCount = 0;
unsigned int unitcount = 0;



int spaced = 0;
unsigned int * frequencytable = get_frequency_table();

oscillator Osc[OSCILLATORS];
oscillator * get_oscillator_pointer (void) {
    return Osc;
}





AUDIOSTREAM * initialize_soundsystem() {

      //-------------------------initialize sound system---------------------------------
    if (!audio_engine_on) audiostatus = "Sound is currently disabled";

       /* install a digital sound driver */
   if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) != 0) {
      audio_engine_on = 0;
      audiostatus = "No soundcard detected - audio is off";
   }

   /* we want a _real_ sound driver */
   if (digi_card == DIGI_NONE) {
      audio_engine_on = 0;
      audiostatus = "Fake soundcard detected - audio is off - call Windows Genuine Advantage";
   }

   /* create an audio stream */
   stream = play_audio_stream(BUFFER_SIZE, 16, FALSE, FSMAX, 255, 128);
   if (!stream) {
      audio_engine_on = 0;
      audiostatus = "Couldn't create audio stream - audio is off";
   }

   short int * wavetable = get_wavetable();

   return stream;
}












void run_synthesizer(BITMAP * buffer, int volume, int rc, int selectedrate, int selectedheight) {

    // char ** wavename = get_wavenames();
    //  char ** heightname = get_heightnames();

    short int * wavetable = get_wavetable();

    //Audio status
    int statusy = SCREEN_H-TEXTHEIGHT-5;
    //rectfill(buffer, 0, statusy, SCREEN_W-1, SCREEN_H-1, BLACK);
    hline(buffer, 0, statusy, SCREEN_W-1, LIGHTBLUE);


    timeconstant = rc;

    if (oldselectedheight != selectedheight) circularsum = 0;
    oldselectedheight = selectedheight;



    //should audio play?
    if (key[KEY_SPACE]) {
        if (!spaced) {
            spaced = 1;
            play_audio = !play_audio;
        }
    } else spaced = 0;

    if (audio_engine_on) {
        if (play_audio) {
            if (old_play_audio != play_audio) voice_start(stream->voice);

            textprintf_ex(buffer, font, TEXTHEIGHT, statusy+4, LIGHTBLUE, -1, "Audio engine active - Now playing");
            audiopointer = (unsigned short *) get_audio_stream_buffer(stream);

            if (audiopointer) {
                for (int i=0; i<BUFFER_SIZE; i++) {



                    //CORE AUDIO PROCESSING--------------------
                    int osctemp = 0;
                    int processingstage = 0;
            //        int oldprocessingstage = 0;
                    int sign = 0, oldsign = 0; //0 for positive, 1 for negative.



                    for (int i = 0; i < OSCILLATORS; i++) {

                        //Test if frequency can be accurately reproduced
                        //if (Osc[i].active) {

                            //remember that osc freq is left-shifted, FCMIN is not
                            Osc[i].phaseIncrement = Osc[i].frequency/FCMIN;
                            Osc[i].phaseAccumulator = (Osc[i].phaseAccumulator + Osc[i].phaseIncrement);
                            Osc[i].phaseOffset = ((Osc[i].phaseAccumulator * samplemult[selectedrate]) >> 16) % SAMPLES;

                            //Get sample
                            osctemp = (wavetable[wavetableindex(Osc[i].selectedWaveform,selectedheight,Osc[i].phaseOffset)]/(4*PERVOICE));



                            //ADSR


                            if (Osc[i].ADSRstate == ATTACK) {
                                Osc[i].targetLevel = UNITY_FP;
                                Osc[i].targetIncrement = UNITY_7b/(Osc[i].attack+1)+1;

                                if (Osc[i].level < Osc[i].targetLevel) {
                                    Osc[i].level += Osc[i].targetIncrement;


                                }
                                if (Osc[i].level > Osc[i].targetLevel) {
                                    Osc[i].level = Osc[i].targetLevel;
                                    Osc[i].ADSRstate = DECAY;
                                }



                            } else if (Osc[i].ADSRstate == DECAY) {
                                Osc[i].targetLevel = Osc[i].sustain;
                                Osc[i].targetIncrement = UNITY_7b/(Osc[i].decay+1)+1;

                                if (Osc[i].level > Osc[i].targetLevel) {
                                    Osc[i].level -= Osc[i].targetIncrement;

                                } else {
                                    Osc[i].level = Osc[i].targetLevel;
                                }


                            } else if (Osc[i].ADSRstate == RELEASE) {
                                Osc[i].targetLevel = 0;
                                Osc[i].targetIncrement = UNITY_7b/(Osc[i].release+1)+1;

                                if (Osc[i].level > Osc[i].targetLevel) {
                                    Osc[i].level -= Osc[i].targetIncrement;
                                    if (Osc[i].level < Osc[i].targetLevel) {
                                        Osc[i].level = Osc[i].targetLevel;
                                        Osc[i].ADSRstate = OFF;
                                    }
                                }

                            }






                            if (Osc[i].level > UNITY_FP) Osc[i].level = UNITY_FP;
                            if (Osc[i].level < 0) Osc[i].level = 0;

                            osctemp = osctemp*(Osc[i].level)/UNITY_FP;


                            //Add distortion
                            osctemp = osctemp*(UNITY_FP + 2*Osc[i].distortion)/UNITY_FP;

                            processingstage += osctemp;
                            //processingstage += (wavetable[wavetableindex(Osc[i].selectedWaveform,selectedheight,Osc[i].phaseOffset)]/(4*PERVOICE))*(UNITY_7b+Osc[i].distortion)/UNITY_7b;

                        //}
                    }




                    // LP Filter
//                    circularsum = (processingstage*timeconstant + ((1<<16)-timeconstant)*circularsum)>>16;
                    circularsum = (processingstage*timeconstant + (UNITY_7b-timeconstant)*circularsum)>>7;
                    processingstage = circularsum;



                    //adjust master volume
                    processingstage = processingstage*volume/UNITY_7b;

                    audioprintout[i] = (processingstage) << (16-bitdepth[selectedheight]);

                    // Check for clipping
                    if (processingstage < 0) oldsign = 1; else oldsign = 0;
                    if (audioprintout[i] < 0) sign = 1; else sign = 0;
                    if (sign != oldsign)
                        if (oldsign == 1) audioprintout[i] = -UNITY_FP; else audioprintout[i] = UNITY_FP;


                    processingstage += height[selectedheight]/2;
                    audiopointer[i] = ((unsigned) (processingstage)) << (16-bitdepth[selectedheight]);






                    //---------------------------------------
                    sampleCount++;
                }

                free_audio_stream_buffer(stream);
            }

        } else {
            textprintf_ex(buffer, font, TEXTHEIGHT, statusy+4, LIGHTBLUE, -1, "Audio engine active - Play disabled");
            if (old_play_audio != play_audio) voice_stop(stream->voice);
        }
    } else {
        textprintf_ex(buffer, font, TEXTHEIGHT, statusy+4, LIGHTBLUE, -1, "%s", audiostatus.c_str());
        if (old_play_audio != play_audio) voice_stop(stream->voice);
    }
    old_play_audio = play_audio;

    int protx = 330;
    int srcplotw = 600, srcploth = 200;

    //dealies
    string inputstr = "Audio Output:";
    plot_waveform(buffer, protx, 20, &srcplotw, &srcploth, height[3], BUFFER_SIZE,
                BUFFER_SIZE-1, BUFFER_SIZE, audioprintout, inputstr.c_str());





    //Calculate DFT
    if (unitcount%4 == 0) calculate_DFT(BUFFER_SIZE, audioprintout, audioFFT);

    //Plot DFT
    plot_FFT(buffer, protx, 260, &srcplotw, &srcploth, height[3], SAMPLES,
                SAMPLES-1, SAMPLES, audioFFT, "Discrete Fourier Transform");



    unitcount++;
    return;
}










