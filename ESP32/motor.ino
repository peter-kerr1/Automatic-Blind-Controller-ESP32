#include "motor.h"

Motor::Motor(int _in1, int _in2) {
  this->in1 = _in1;
  this->in2 = _in2;
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
}

void Motor::forward() {
  digitalWrite(this->in1, HIGH);
  digitalWrite(this->in2, LOW);
}

void Motor::stop() {
  digitalWrite(this->in1, LOW);
  digitalWrite(this->in2, LOW);
}

void Motor::backward() {
  digitalWrite(this->in1, LOW);
  digitalWrite(this->in2, HIGH);
}
