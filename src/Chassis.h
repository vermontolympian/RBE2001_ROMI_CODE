#include <Arduino.h>
#include <Romi32U4.h>


class Chassis{
    private:
        Romi32U4Motors motors;
        Romi32U4Encoders encoders;
        int leftEffort , rightEffort;
        const unsigned CPR = 1440;
        const float wheelDiameter = 2.75;
        const float wheelTrack = 5.75;
        long targetCount;
        float kp = 0.05;
        float basePWR = 25;
        

    public:
        void driveDistance(float inches);
            
        bool doneDriving();

        void turnAngle(float degrees);
            
        void pauseDriving();

        void resumeDriving();

        void encoderReset();
        
        bool done = false;
};