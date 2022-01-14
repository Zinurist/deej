const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A6};
const int digitalInputs[NUM_SLIDERS] = {5, 6, 7, 8, 9};

int analogSliderValues[NUM_SLIDERS];
bool mutedSliders[NUM_SLIDERS];
int mutedTimeout[NUM_SLIDERS];

void setup() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
    pinMode(digitalInputs[i], INPUT_PULLUP);
  }

  Serial.begin(9600);
}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    analogSliderValues[i] = 1023 - analogRead(analogInputs[i]);
    if (digitalRead(digitalInputs[i]) == HIGH) {
      if (mutedTimeout[i] < 20) {
        mutedTimeout[i]++;
      }
    } else if (mutedTimeout[i] > 10) {
      mutedSliders[i] = !mutedSliders[i];
      mutedTimeout[i] = 0;
    }
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    int val = 0;
    if (!mutedSliders[i]) {
      val = (int)analogSliderValues[i];
    }
    builtString += String(val);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }

  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
