//Hardware connections:
//
//                    +10µF
//PIN 11 ---[ 1k ]--+---||--->> Audio out
//                  |
//                 === 10nF
//                  |
//                 GND

// DZL 2014
// HTTP://dzlsevilgeniuslair.blogspot.dk
// HTTP://illutron.dk


// arduino nonsense
#include <synth.h>

int potPins[] = {A0, A2, A3, A4};
const int NB = sizeof potPins / sizeof potPins[0];

//ADD BUTTONS
const int buttonPins[] = {2, 3, 4, 5};

/////////////////////////////////////

int vals[NB] = {0};
int voice = 0;
int old_voice = 0;
int currentEnvelope = 0;

int waveform[] = {SINE,TRIANGLE,SQUARE,SAW,RAMP,NOISE};
int envelopes[] = {ENVELOPE1,ENVELOPE2,ENVELOPE3};
int pitch[] = {60,0,0,0};
int leng[] = {60,64,70,20}; // TODO
int mod[] = {60,64,64,64};  // TODO

int synths[2][3];

//Set up the voices:
//setupVoice( voice[0-3] , waveform[SINE,TRIANGLE,SQUARE,SAW,RAMP,NOISE] , pitch[0-127], envelope[ENVELOPE0-ENVELOPE3], length[0-127], mod[0-127, 64=no mod])

synth edgar;    //-Make a synth

void setup() {

    Serial.begin(115200);

    for (int i=0; i<NB; i++)
        pinMode(buttonPins[i], INPUT_PULLUP);

    // SETUP SYNTH
    edgar.begin();
    edgar.setupVoice(0, waveform[0], pitch[0], envelopes[0], 60, 64);
    edgar.setupVoice(1, waveform[4], pitch[1], envelopes[2], 64, 64);
    edgar.setupVoice(2, waveform[1], pitch[2], envelopes[1], 70, 64);
    edgar.setupVoice(3, waveform[0], pitch[3], envelopes[3], 20, 64);

    // SETUP SEQUENCER

}

void loop()
{
    //-Plays some notes on the voices:
    for (int i = 0; i < NB-1; i++)
        vals[i] = map(analogRead(potPins[i]), 0, 1024, 0, 127);
    vals[5] = map(analogRead(potPins[5]), 0, 1024, 0, 4);

    // print out the value you read:
    Serial.print("Synth: ");
    Serial.print(voice);
    Serial.print(" Freq: ");
    Serial.print(val1);
    Serial.print(" Mod: ");
    Serial.print(val2);
    Serial.print(" Pitch: ");
    Serial.print(val3);
    Serial.print(" Length: ");
    Serial.print(val4);
    Serial.print(" Envelope: ");
    Serial.println(val5);

    checkButtons();
    getNewSynth();

    for (int i = 0; i < NB; i++)
    {
        edgar.setFrequency(voice,vals[1]);

        // MOD
        edgar.setMod(voice,vals[2]);

        // PITH
        edgar.setPitch(voice,vals[3]);

        // LENGTH
        edgar.setLength(voice, NB);

        // ENVELOPE
        edgar.setEnvelope(voice,currentEnvelope);

        // PLAY NOTE
        edgar.mTrigger(voice, vals[5]);
    }
}


void checkButtons(){
    static int buttons[NB] = {0};

    for (int i=0; i<NB; i++) {
        if (buttons[i] != digitalRead(buttonPins[i])) {
            buttons[i] = digitalRead(buttonPins[i]);
            voice = i;

            Serial.print("button changed => voice = ");
            Serial.println(voice);
        }
    }
}

void saveNewValues(int voice){

    edgar.setWave(voice,synths[voice][0]);
    edgar.setFrequency(voice,synths[voice][1]);
    edgar.setPitch(voice,synths[voice][2]);
    edgar.setEnvelope(voice,synths[voice][3]);
    edgar.setLength(voice,synths[voice][4]);
    edgar.setMod(voice,synths[voice][5]);
    currentEnvelope = synths[voice][3];

}


void getNewSynth(){

    if (old_voice != voice)
    {
        //Serial.println("new voice" + voice);
        // SAVE OLD VALUES
        saveNewValues(old_voice);
        // LOAD NEW VALUES
        saveNewValues(voice);
        old_voice = voice;
    }
}

