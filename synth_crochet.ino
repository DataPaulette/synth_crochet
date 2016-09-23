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

int pot1 = A0;
int pot2 = A1;
int pot3 = A2;
int pot4 = A3;
int pot5 = A4;

//ADD BUTTONS
const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int buttonPin3 = 4;
const int buttonPin4 = 5;

int button1 = 1;
int button2 = 0;
int button3 = 0;
int button4 = 0;

//byte buttons[4] = {B1000};


/////////////////////////////////////

int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
int voice = 0;
int old_voice = 0;
int currentEnvelope = 0;

int waveform[] = {SINE,TRIANGLE,SQUARE,SAW,RAMP,NOISE};
int envelopes[] = {ENVELOPE1,ENVELOPE2,ENVELOPE3};
int pitch[] = {60,0,0,0};
int leng[] = {60,64,70,20};
int mod[] = {60,64,64,64};

int synths[2][3];

//Set up the voices:
//setupVoice( voice[0-3] , waveform[SINE,TRIANGLE,SQUARE,SAW,RAMP,NOISE] , pitch[0-127], envelope[ENVELOPE0-ENVELOPE3], length[0-127], mod[0-127, 64=no mod])

synth edgar;    //-Make a synth

void setup() {

    Serial.begin(115200);

    pinMode(buttonPin1, INPUT);
    pinMode(buttonPin2, INPUT);
    pinMode(buttonPin3, INPUT);
    pinMode(buttonPin4, INPUT);

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

    int val1 = map(analogRead(pot1), 0, 1024, 0, 127);
    int val2 = map(analogRead(pot2), 0, 1024, 0, 127);
    int val3 = map(analogRead(pot3), 0, 1024, 0, 127);
    int val4 = map(analogRead(pot4), 0, 1024, 0, 127);
    int val5 = map(analogRead(pot5), 0, 1024, 0, 4);
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

    for (int i = 0; i < 4; i++)
    {
        //edgar.setWave(voice,val1);
        edgar.setFrequency(voice,val1);

        // MOD
        edgar.setMod(voice,val2);

        // PITH
        edgar.setPitch(voice,val3);

        // LENGTH
        edgar.setLength(voice,4);

        // ENVELOPE
        edgar.setEnvelope(voice,currentEnvelope);

        // PLAY NOTE
        edgar.mTrigger(voice, val5);

        // delay(300);
    }
}


void checkButtons(){

    button1 = digitalRead(buttonPin1);
    button2 = digitalRead(buttonPin2);
    button3 = digitalRead(buttonPin3);
    button4 = digitalRead(buttonPin4);

    if (button1 == HIGH) {
        voice = 0;
    } else if (button2 == HIGH) {
        voice = 1;
    }else if (button3 == HIGH) {
        voice = 2;
    } else {
        voice = 3;
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
