byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;
int play_flag = 0;
int count = 0;
byte data;

const int WHOLE_PIN = 6;
const int HALF_PIN = 7;
const int QUARTER_PIN = 8;
const int EIGHTH_PIN = 9;
const int SIXTEENTH_PIN = 10;

int pins[] = {WHOLE_PIN, HALF_PIN, QUARTER_PIN, EIGHTH_PIN, SIXTEENTH_PIN};

int shouldPulseWhole = 0;
int shouldPulseHalf = 0;
int shouldPulseQuarter = 0;
int shouldPulseEighth = 0;
int shouldPulseSixteenth = 0;

void setup() {
  Serial.begin(31250);

  int i;
  for (i = 0; i < 5; i = i + 1) {
    pinMode(pins[i], OUTPUT);
  }
}

void loop() {
  if (Serial.available() > 0) {
    data = Serial.read();

    if ((data == midi_start) || (data == midi_continue)) {
      play_flag = 1;
    } else if (data == midi_stop) {
      play_flag = 0;
      count = 0;
      resetOuts();
    } else if ((data == midi_clock) && (play_flag == 1)) {
      checkSteps();
      doPulses();
      incrementCount();
    }
  }
}

void resetOuts() {
  int i;
  for (i = 0; i < 5; i = i + 1) {
    setLow(pins[i]);
  }
}

void doPulses() {
  shouldPulseWhole
  ? setHigh(WHOLE_PIN)
  : setLow(WHOLE_PIN);

  shouldPulseHalf
  ? setHigh(HALF_PIN)
  : setLow(HALF_PIN);

  shouldPulseQuarter
  ? setHigh(QUARTER_PIN)
  : setLow(QUARTER_PIN);

  shouldPulseEighth
  ? setHigh(EIGHTH_PIN)
  : setLow(EIGHTH_PIN);

  shouldPulseSixteenth
  ? setHigh(SIXTEENTH_PIN)
  : setLow(SIXTEENTH_PIN);

  shouldPulseWhole = 0;
  shouldPulseHalf = 0;
  shouldPulseQuarter = 0;
  shouldPulseEighth = 0;
  shouldPulseSixteenth = 0;
}

void incrementCount() {
  count = count + 1;
  if (count == 96) count = 0;
}

void checkSteps() {
  if (count == 0) {
    shouldPulseWhole = 1;
  }

  if (count % 48 == 0) {
    shouldPulseHalf = 1;
  }

  if (count % 24 == 0) {
    shouldPulseQuarter = 1;
  }

  if (count % 12 == 0) {
    shouldPulseEighth = 1;
  }

  if (count % 6 == 0) {
    shouldPulseSixteenth = 1;
  }
}

void setHigh(int pin) {
  digitalWrite(pin, HIGH);
}

void setLow(int pin) {
  digitalWrite(pin, LOW);
}
