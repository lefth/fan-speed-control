/*
MIT License

Copyright (c) 2018 Dan Zwell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

const int dialPin = A0;     // The middle pin of a potentiometer voltage divider
const int buttonPin = 12;   // Button for turning the fan on and off
const int powerPin = 11;    // For controlling whether the fan gets power.
const int outputPin = 10;   // PWM output, to determine how fast the fan spins.
							// The fan speed wire is pulled up. Pull it down to
							// ground to set the speed to the lowest level. And
							// use PWM (pulling it down to ground) to change the
							// speed. In this circuit, setting the output HIGH
							// means the fan's wire is pulled down to ground by a
							// NPN transistor. An external resistor is not needed.
							// MUST use a transistor, since the pulled-up
							// voltage can be up to 12V.
const int ledPin = 13;      // No need to hook up anything if you're happy to look
							// directly at the light on the board.

void setup() {
  pinMode(dialPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(outputPin, OUTPUT);
  pinMode(powerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

// Treat a momentary button as a toggle, detecting each separate press
// and toggling the state. And ignore noise.
bool getToggleState()
{
  static bool isOn = true;
  static bool wasPressed = false;
  const int arrayLength = 20;
  static bool historyBuffer[arrayLength];
  static unsigned int index = 0;

  bool _buttonPressed = !digitalRead(buttonPin);
  historyBuffer[index = (index + 1) % arrayLength] = _buttonPressed;

  // Only consider the button pressed if it has been down for 90% of the
  // remembered consecutive frames, and only consider it NOT pressed if it's
  // been it's been up for 90% of the remembered consecutive frames. Otherwise,
  // assume the previous value is correct. The strict difference between
  // "pressed" and "not pressed" will reduce flicker.
  int pressedInMemory = 0;
  for (int i=0; i<arrayLength; ++i)
    if (historyBuffer[i])
      ++pressedInMemory;

  bool buttonPressed;
  
  // NOTE: adjust the 10% and 90% ratios if you have an unreliable button
  if (pressedInMemory / (double)arrayLength >= 0.9)
    buttonPressed = true;
  else if (pressedInMemory / (double)arrayLength <= 0.1)
    buttonPressed = false;
  else
	buttonPressed = wasPressed; // no change

  if (buttonPressed != wasPressed) {
    if (buttonPressed) {
	  // only toggle on press start, not press end
	  // TODO: write isOn to EEPROM so the state doesn't change if power is interrupted.
      isOn = !isOn;
	}

    wasPressed = buttonPressed;
  }
  
  return isOn;
}

double getChosenSpeed() {
  int _voltage = analogRead(dialPin);
  //double voltage = _voltage / 1023. * 5;
  //Serial.println(voltage);
  double speed = _voltage / 1023.0;
  return speed;
}

void setSpeed(double speed) {
  // output more ON means the transistor will be more on,
  // and that will ground the control and reduce the fan speed.
  int output = 255 * (1-speed);
  analogWrite(outputPin, output);
}

void loop() {
  bool on = getToggleState();
  if (on) {
    digitalWrite(powerPin, HIGH);
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(powerPin, LOW);
    digitalWrite(ledPin, LOW);
    return;
  }
  
  double speed = getChosenSpeed();
  setSpeed(speed);
}

// vim: sw=2:
