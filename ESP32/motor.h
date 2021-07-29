#ifndef MOTOR_H
#define MOTOR_H

class Motor {
  public:
    Motor(int, int);
    void forward();
    void stop();
    void backward();

  private:
    int in1;
    int in2;
};

#endif
