#include <Chassis.h>

void Chassis::driveDistance(float inches){
    float ticksToGo = (inches / (wheelDiameter * 3.14) * 1440);
    float error = (ticksToGo - encoders.getCountsLeft());
    float efforts = (error * kp);
            if(encoders.getCountsLeft() < ticksToGo)
            {
                motors.setEfforts((efforts + basePWR) , (efforts + basePWR));
                done = false;
            }
            else
            {
                motors.setEfforts(0 , 0);
                done = true;
            }
        }


bool Chassis::doneDriving(){
    return done;
}

void Chassis::turnAngle(float degrees){
    float distRot = (((wheelTrack * 3.14) * degrees / 360) / (wheelDiameter *3.14) * 1440);
    float error = (distRot - encoders.getCountsLeft());
    float efforts = (error * kp);
        if(encoders.getCountsLeft() < distRot)
        {
             motors.setEfforts((efforts + basePWR) , (-efforts - basePWR));
             done = false;
        }
        else
        {
            motors.setEfforts(0 , 0);
            done = true;
        }
}
void Chassis::pauseDriving(){
    motors.setEfforts(0 , 0);
}
void Chassis::resumeDriving(){

}
void Chassis::encoderReset(){
    encoders.getCountsAndResetLeft();
    encoders.getCountsAndResetRight();
    Serial.println("Encoder Reset");
}