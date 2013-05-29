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

/*! \struct oscillator
    \brief This is the class that generates the values of stuff.

    It is important to incorporate your mom into the equations you're
    interested in attache-ing.  I will need to remind you of the utter
    importance of revising bacon gritz before becoming an incumbent.
*/
struct oscillator {
/*! \var active
    Determines whether the oscillator is active. */
    bool active;

/*! \var note
    The active note (0-127) */
    int note;

/*! Determines whether the oscillator is active. */
    char selectedWaveform, selectedBitDepth, selectedSampleRate;

    //Volume envelope (ADSR)
    signed int level;
    signed int targetLevel;
    signed int targetIncrement;

    unsigned char ADSRstate; // 4 states as indicated
    signed int attack;
    signed int decay;
    signed int sustain;
    signed int release;

    signed int distortion;




    //Phase accumulator
    //Uses 16 bit real, 16 bit fractional fixed-point values.
    unsigned int frequency;
    unsigned int targetFrequency;

    unsigned int phaseIncrement;    //How much to increment each cycle
    unsigned int phaseAccumulator;  //Non-truncated accumulator
    unsigned int phaseOffset;       //Truncated and scaled to fit within total samples




    //Initializer
    oscillator() {
        active = 0;
        note = -1;

        selectedWaveform = 0;
        selectedBitDepth = 3;
        selectedSampleRate = 3;

        frequency = 0;
        targetFrequency = 0;

        phaseIncrement = 0;
        phaseOffset = 0;
        phaseAccumulator = 0;

        level = 0;
        targetLevel = UNITY_FP;

        ADSRstate = 0;

        attack = UNITY_FP;
        decay = UNITY_FP;
        sustain = UNITY_FP;
        release = UNITY_FP;

        distortion = 0;
    }

};
