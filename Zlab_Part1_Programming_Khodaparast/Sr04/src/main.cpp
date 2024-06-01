#include <Arduino.h>

//======== function declarations ========

float objectRange(int dimension = 1);
bool checkIfDistance(float isDistance);
float stableOutput();

// ======================================

const int trigPin = 9;     // Trig pin on sensor
const int echoPin = 10;    // Echo pin on Sensor

float duration;            // to hold the time length of the sound wave
float distance;            // to hold the distance of the object

float ambientTemp = 298;   // Ambient temperature [in Kelvin] (25 + 273)
float gamma = 1.4;         // ratio of specific heats
int R = 286;               // Gas Constant [m^2/s^2]
float a = sqrt(gamma*R*ambientTemp);    // speed of sound [m/s]
float soundSpeed = a/10000;             // speed of sound in [cm/microsecnds]



//---------------------------------------

void handleWaves(){

    digitalWrite(trigPin, LOW);         // set trig pin low for 2 microseconds
    delayMicroseconds(2);               // just to make sure the pin is low at first.
    digitalWrite(trigPin, HIGH);        // set trig pin high for 10 microseconds
    delayMicroseconds(10);              // to send 8 cycle sonic burst from trig pin
    digitalWrite(trigPin, LOW);         // which then bounces off the object and hits the echo pin

    duration = pulseIn(echoPin, HIGH);  // returns the wave pass time from trig to echo pin
    distance = (duration * soundSpeed)/2 ;  // speed of sound in cM/uSec
}

void serialDistance(){

    Serial.print("Distance :\t");
    Serial.print(objectRange(1));         // 1 returns distance in centimeters.
    Serial.print(" cm\t");
    Serial.print(objectRange(2));         // 2 returns distance in inches.
    Serial.print(" inch\t");
    Serial.print(objectRange());          // Default dimmension is set to centimeters.
    Serial.print(" cm\t");
}


//---------------------------------------

void setup(){

    pinMode(trigPin, OUTPUT);           // define Trig pin as output (transmitter)
    pinMode(echoPin, INPUT);            // define Echo pin as input (reciever)

    Serial.begin(9600);

}

//---------------------------------------

void loop(){

    handleWaves();
    stableOutput();

// --------- Distance in cm or inch printed in serial monitor ------------
    serialDistance();
// ------------ Check if distance is less than a set value ---------------

    if (checkIfDistance(30) == true){
        Serial.print("true\n");
    } else{
        Serial.print("false\n");
    }
    delay(100);


} 


// ============== Functions Definition ===============
float objectRange(int dimension = 1){
    float range;
    if (dimension == 1){
        range = distance;
    } else if (dimension == 2){
        range = distance/2.54;
    }
    return range;
}

bool checkIfDistance(float isDistance){
    if (distance <= isDistance){
        return true;
    } else{
        return false; 
    }
}

float stableOutput(){
    const int interval = 100;               //interval in milliseconds
    unsigned long nowMillis = millis();
    unsigned long preMillis = 0;
    float newDistance;
    float oldDistance = distance;
    if (nowMillis - preMillis >= interval){   // log data after 100 ms
        newDistance = distance;
        preMillis = nowMillis;
    } else {
        newDistance = oldDistance;
    }
    distance = 0.99*oldDistance + 0.01*newDistance;      // return distance after filtering
    return distance;
}
// ===================================================
