#include <Arduino.h>
#pragma once
class BlueMotor
{
    public:
        void setEffort(float effort);
        void moveTo(long position); //for next lab
        long getPosition();
        void reset();
        void setup();
        float getRadians();
        float getVelocity();

    private:
        void setEffort(int effort, bool clockwise);
        static void EncoderISR();
        const int tolerance = 3;
        const int PWMOutPin = 11;
        const int AIN2 = 4;
        const int AIN1 = 13;
        const int ENCA = 0;
        const int ENCB = 1;
        const int EncoderRatio = 540;

        long previousTime = 0;
        float previousPosition = 0;       
};