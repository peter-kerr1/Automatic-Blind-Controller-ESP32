#include "motor.h"

Motor::Motor(int _in1, int _in2, int _sens1, int _sens2) {
  in1 = _in1;
  in2 = _in2;
  sens1 = _sens1;
  sens2 = _sens2;
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(sens1, INPUT);
  pinMode(sens2, INPUT);
}

void Motor::clockwise() {
  if (encoderVal < encoderMax) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    halt = true; // Sets the command on Firebase back to 'stop'
  }
}

void Motor::stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void Motor::antiClockwise() {
  if (encoderVal > encoderMin) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else {
    halt = true;
  }
}

void Motor::encoderCallback() {
  if (digitalRead(sens2) == HIGH) {
    encoderVal++;
  } else {
    encoderVal--;
  }

  // Stop the motor as soon as we go out of the pre-set bounds.
  // Allow motor to move back into bounds before triggering a stop again.
  if (outOfBounds == false && (encoderVal <= encoderMin || encoderVal >= encoderMax)) {
    stop();
    halt = true; // Can't update Firebase during an interrupt - causes CPU timeout
    outOfBounds = true;
  } else if (encoderVal > encoderMin && encoderVal < encoderMax) {
    outOfBounds = false;
  }
}
