#include "BlueMotor.h"

long oldValue;
long newValue;
long errorCount;
long count = 0;

const char X = 5;
char encoderArray[4][4] = {
    {0, -1, 1, X},
    {1, 0, X, -1},
    {-1, X, 0, 1},
    {X, 1, -1, 0}};


void BlueMotor::setEffort(int effort, bool clockwise){
    if(clockwise){
        setEffort(effort);
    }
    else{
        setEffort(-1 * effort);
    }
}

void BlueMotor::setEffort(float effort){
    if(effort == 0){
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, LOW);
        OCR1C = effort;
    }
    else if(effort > 0){
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);

        int newEffort = map(effort, 0, 400, dbUp, 400);
        OCR1C = newEffort;
    }
    else{
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);

        int newEffort = map(abs(effort), 0, 400, dbDown, 400);
        Serial.println(newEffort);
        OCR1C = newEffort;
    }
}

void BlueMotor::moveTo(long position){

}

long BlueMotor::getPosition(){
    return count;
}

void BlueMotor::reset(){
    count = 0;
}

static void BlueMotor::EncoderISR(){
    newValue = (digitalRead(3) << 1) | digitalRead(2);
    char value = encoderArray[oldValue][newValue];
    if(value == X){
        errorCount++;
    }
    else{
        count += value;
    }
    oldValue = newValue;
}

void BlueMotor::setup(){
    //PWM Setup
    TCCR1A = 0xA8;
    TCCR1B = 0x11;
    ICR1 = 400;
    OCR1C = 0;

    //Encoder Setup
    const int encPin1 = 2;
    const int encPin2 = 3;
    pinMode(encPin1, INPUT);//Encoder in 1
    pinMode(encPin2, INPUT);//Encoder in 2
    pinMode(STOP, INPUT); //Stop Switch
    count = 0;

    pinMode(4, OUTPUT);
    pinMode(11, OUTPUT);
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(encPin1), EncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(encPin2), EncoderISR, CHANGE);
}

float BlueMotor::getRadians(){
    float position = getPosition();
    return (position / EncoderRatio * 2 * 3.14159);
}

float BlueMotor::getVelocity(){
    float currentPosition = count;
    long currentTime = millis();
    float positionDifference = (currentPosition - previousPosition) / EncoderRatio;
    double timeDifference = (currentTime - previousTime);

    previousPosition = currentPosition;     //Reset position
    previousTime = currentTime;             //Reset time
    // return timeDifference;
    return(positionDifference / timeDifference) * 1000 * 60;
}

bool BlueMotor::home(){
    if(digitalRead(STOP) == HIGH){
        setEffort(0);
        return true;
    }
    else if (digitalRead(STOP) == LOW){
        setEffort(-400);
        return false;
    }
}

void BlueMotor::goToPosition(int degree){
    long currentPosition = -1 * getPosition();

    long desiredPosition = degree * 45;                 //TODO MATH

    long error = desiredPosition - currentPosition;

    totalError = totalError + error;

    int effort = (kp*error - ki*(totalError) + kd*(error-previousError));

    setEffort(effort);

    previousError = error;
}