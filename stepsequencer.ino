int ledPin[4] = {2, 4, 7, 10};
int buttonPin[4] = {36, 35, 34, 33};
boolean lastButtonState[4] = {LOW, LOW, LOW, LOW};
boolean buttonState[4] = {LOW, LOW, LOW, LOW};
boolean stepState[3][4] = {
  {false, false, false, false},
  {false, true, false, true},
  {true, true, false, false}
};
int tempo = 0;
int currentStep = 0;
unsigned long lastStepTime = 0;

int currentChannel = 0;
int backPin = 38;
boolean lastBackState = LOW;
boolean backState = LOW;

int frontPin = 37;
boolean lastFrontState = LOW;
boolean frontState = LOW;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPin[i], INPUT);
  }
  pinMode(backPin, INPUT);
  pinMode(frontPin, INPUT);

}

void loop() {
  checkButtons();
  setLeds();
  sequence();
}

void checkButtons() {

  for (int i = 0; i < 4; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPin[i]);
    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (stepState[currentChannel][i] == false) {
        stepState[currentChannel][i] = true;
      } else if (stepState[currentChannel][i] == true) {
        stepState[currentChannel][i] = false;

      }

    }
  }
  lastBackState = backState;
  backState = digitalRead(backPin);

  lastFrontState = frontState;
  frontState = digitalRead(frontPin);

  if (backState == HIGH && lastBackState == LOW) {
    currentChannel = currentChannel - 1;
  } if (currentChannel < 0) {
    currentChannel = 2;
  }

  if (frontState == HIGH && lastFrontState == LOW) {
    currentChannel = currentChannel + 1;
  } if (currentChannel == 3) {
    currentChannel = 0;
  }
}

/*

  void setLeds() {
  for (int i = 0; i < 4; i++) {
    if (stepState[i] == true) {
     // digitalWrite(ledPin[i], HIGH);
    } else if (stepState[i] == false) {
     // digitalWrite(ledPin[i], LOW);
    }
  }
  }
*/
void setLeds() {
  for (int i = 0; i < 4; i++) {
    if (i == currentStep) {
      analogWrite(ledPin[i], 255);
    } else if (stepState[currentChannel][i] == true) {
      analogWrite(ledPin[i], 25);
    } else  {
      analogWrite(ledPin[i], 0);
    }

  }
}


void sequence() {

  tempo = analogRead(A9);

  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
    // digitalWrite(ledPin[currentStep], LOW);  //turn off the current led

    currentStep = currentStep + 1;         //increment to the next step
    if (currentStep > 3) {
      currentStep = 0;
    }

    sendmidi();

    // digitalWrite(ledPin[currentStep], HIGH); //turn on the new led

    lastStepTime = millis();               //set lastStepTime to the current time
  }
}

void sendmidi() {

  if (stepState[0][currentStep] == true) {
    usbMIDI.sendNoteOff(60, 0, 1);
    usbMIDI.sendNoteOn(60, 100, 1);

  }
  if (stepState[1][currentStep] == true) {
    usbMIDI.sendNoteOff(62, 0, 1);
    usbMIDI.sendNoteOn(62, 100, 1);

  }

  if (stepState[2][currentStep] == true) {
    usbMIDI.sendNoteOff(64, 0, 1);
    usbMIDI.sendNoteOn(64, 100, 1);
  }
}
