/*
 Name:		MeetingTimer.ino
 Created:	5/6/2017 6:54:03 AM
 Author:	Michael
*/

// Define I/O pins based on hardware configuration
#define RED_PIN 3
#define GREEN_PIN 11
#define BLUE_PIN 10
#define BOARD_LED 13

// #define INVERT_DUTY_CYCLE

#define START_DELAY 500
#define SECOND_DELAY 1000

// These 3 are the PWM pin supported on the Arduino Nano

// the setup function runs once when you press reset or power the board
void setup() {

    // Define our LED pins as outputs
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(BOARD_LED, OUTPUT);

    // Turn everything off to start, then do a G-B-Y-R sweep as POST
    LEDWrite(0, 0, 0);
    delay(START_DELAY);
    LEDWrite(0, 255, 0); // G
    delay(START_DELAY);
    LEDWrite(0, 0, 255); // B
    delay(START_DELAY);
    LEDWrite(64, 255, 0); // Y
    delay(START_DELAY);
    LEDWrite(255, 0, 0); // R
    delay(START_DELAY);
    LEDWrite(0, 0, 0);
    delay(START_DELAY);

}

// the loop function runs over and over again until power down or reset
void loop() {

    // Give a 100ms courtesy time at startup
    delay(100);
    LEDWrite(0, 255, 0);   

    

    // Define the total seconds and the offsets at which the LEDs change from Green --> Blue --> Yellow --> Red
    int totalSeconds = 120;
    int halfWay = (totalSeconds >> 1); // 60 seconds remaining
    int threeQuarterWay = (totalSeconds >> 2) + halfWay; // 30 seconds remaining
    int sevenEigthWay = (totalSeconds >> 3) + threeQuarterWay; // 15 seconds remaining

    for (int secondsCounter = 0; secondsCounter < totalSeconds; secondsCounter++)
    {
        if (secondsCounter > sevenEigthWay) // 7/8 done
        {
            LEDWrite(255, 0, 0); // Red
        }
        else if (secondsCounter > threeQuarterWay) // 3/4 done
        {
            LEDWrite(64, 255, 0); // Yellow
        }
        else if (secondsCounter > halfWay) // 1/2 done
        {
            LEDWrite(0, 0, 255); // Blue
        }
        // 1 second timer, we're at the mercy of the oscillator on the arduino for accuracy
        delay(SECOND_DELAY);
        if (secondsCounter & 1 > 0)
        {
            digitalWrite(BOARD_LED, HIGH);
        }
        else
        {
            digitalWrite(BOARD_LED, LOW);
        }
        
        

    }

    // Once we've given the count down, start flashing FOREVER!
    while (1)
    {
        delay(250);
        LEDWrite(255, 0, 0); // Red
        delay(250);
        LEDWrite(0, 0, 0); // Off
    }

  
}


// Function to apply a scale factor to R, G, B in order to equalize the lumens per duty cycle of the different channels
// R,G,B are expected to be UINT8 format, and the scale factors are U1.6 formatted such that 0x80 is full scale
void LEDWrite(int R, int G, int B)
{


    
    
    
    analogWrite(RED_PIN, R);
    analogWrite(GREEN_PIN, G);
    analogWrite(BLUE_PIN, B);
    

    
    const int RScale = 0x80; // 75% intensity
    const int GScale = 0x80; // 25% * .75 intensity
    const int BScale = 0x80; // 100% intensity
    const int ScaleOffset = 7; // Decimal point is between bits 6 and 7

    // Multiply the 2 8 bit numbers together
    int Red = R * RScale;
    int Green = G * GScale;
    int Blue = B * BScale;


    
    #ifdef INVERT_DUTY_CYCLE
        // Take the amount of RGB, shift it by the scale offset to effectively scale by the numbers above
        // The 255- is due to the PWM dutcy cycle is actually the "off" duty cycle
        // This offset is required due to the common anode LED
        Red = 255 - (Red >> ScaleOffset);
        Green = 255 - (Green >> ScaleOffset);
        Blue = 255 - (Blue >> ScaleOffset);
    #endif // INVERT_DUTY_CYCLE

    #ifndef INVERT_DUTY_CYCLE
        // Take the amount of RGB, shift it by the scale offset to effectively scale by the numbers above
        // The 255- is due to the PWM dutcy cycle is actually the "off" duty cycle
        // This offset is required due to the common anode LED
        Red = (Red >> ScaleOffset);
        Green = (Green >> ScaleOffset);
        Blue = (Blue >> ScaleOffset);
    #endif // !INVERT_DUTY_CYCLE


    analogWrite(RED_PIN, Red);
    analogWrite(GREEN_PIN, Green);
    analogWrite(BLUE_PIN, Blue);

    /*
    RGBDebug((Red >> ScaleOffset), (Green >> ScaleOffset), (Blue >> ScaleOffset));
    */

}
