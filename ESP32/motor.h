#ifndef MOTOR_H
#define MOTOR_H

class Motor {
  public:
    Motor(int, int, int, int, int);
    void clockwise();
    void stop();
    void antiClockwise();
    void encoderCallback();
    int encoderVal;
    int encoderMin;
    int encoderMax;
    int halt = false;

  private:
    int pwm;
    int in1;
    int in2;
    int sens1;
    int sens2;
    bool outOfBounds = false;
};

#endif
