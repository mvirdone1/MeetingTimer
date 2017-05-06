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

// Uses arduino analog write which is 0-255 for intesity
// All dimming done in software, no inline resistor for this
const int RGBYellow[] = { 20, 60, 0 };
const int RGBOff[] = { 0, 0, 0 };
const int RGBBlue[] = { 0, 0, 80};
const int RGBGreen[] = { 0, 60, 0 };
const int RGBRed[] = { 80, 0, 0 };

// the setup function runs once when you press reset or power the board
void setup() {

    // Define our LED pins as outputs
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(BOARD_LED, OUTPUT);

    // Turn everything off to start, then do a G-B-Y-R sweep as POST
    LEDWrite(RGBOff);
    delay(START_DELAY);
    LEDWrite(RGBGreen); // G
    delay(START_DELAY);
    LEDWrite(RGBBlue); // B
    delay(START_DELAY);
    LEDWrite(RGBYellow); // Y
    delay(START_DELAY);
    LEDWrite(RGBRed); // R
    delay(START_DELAY);
    LEDWrite(RGBOff);
    delay(START_DELAY);

}

// the loop function runs over and over again until power down or reset
void loop() {

    // Give a 100ms courtesy time at startup
    delay(100);
    LEDWrite(RGBGreen);   

    

    // Define the total seconds and the offsets at which the LEDs change from Green --> Blue --> Yellow --> Red
    int totalSeconds = 120;
    int halfWay = (totalSeconds >> 1); // 60 seconds remaining
    int threeQuarterWay = (totalSeconds >> 2) + halfWay; // 30 seconds remaining
    int sevenEigthWay = (totalSeconds >> 3) + threeQuarterWay; // 15 seconds remaining

    for (int secondsCounter = 0; secondsCounter < totalSeconds; secondsCounter++)
    {
        if (secondsCounter > sevenEigthWay) // 7/8 done
        {
            LEDWrite(RGBRed); // Red
        }
        else if (secondsCounter > threeQuarterWay) // 3/4 done
        {
            LEDWrite(RGBYellow); // Yellow
        }
        else if (secondsCounter > halfWay) // 1/2 done
        {
            LEDWrite(RGBBlue); // Blue
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
        LEDWrite(RGBRed); // Red
        delay(250);
        LEDWrite(RGBOff); // Off
    }

  
}

void LEDWrite(int colors[3])
{
    #ifdef INVERT_DUTY_CYCLE
        analogWrite(RED_PIN, 255 - colors[0]);
        analogWrite(GREEN_PIN, 255 - colors[1]);
        analogWrite(BLUE_PIN, 255 - colors[2]);
    #endif // INVERT_DUTY_CYCLE

    // Don't invert
    #ifndef INVERT_DUTY_CYCLE
        analogWrite(RED_PIN, colors[0]);
        analogWrite(GREEN_PIN, colors[1]);
        analogWrite(BLUE_PIN, colors[2]);
    #endif // !INVERT_DUTY_CYCLE

}

// Function to apply a scale factor to R, G, B in order to equalize the lumens per duty cycle of the different channels
// R,G,B are expected to be UINT8 format, and the scale factors are U1.6 formatted such that 0x80 is full scale
void LEDWriteScaled(int R, int G, int B)
{
       
    
    const int RScale = 0x80; // 75% intensity
    const int GScale = 0x80; // 25% * .75 intensity
    const int BScale = 0x80; // 100% intensity
    const int ScaleOffset = 7; // Decimal point is between bits 6 and 7

    // Multiply the 2 8 bit numbers together
    int Red = R * RScale;
    int Green = G * GScale;
    int Blue = B * BScale;


    

    // Take the amount of RGB, shift it by the scale offset to effectively scale by the numbers above
    Red = (Red >> ScaleOffset);
    Green = (Green >> ScaleOffset);
    Blue = (Blue >> ScaleOffset);
        
    

    int RGBValues[3];
    RGBValues[0] = Red;
    RGBValues[1] = Green;
    RGBValues[2] = Blue;

    LEDWrite(RGBValues);

    /*
    RGBDebug((Red >> ScaleOffset), (Green >> ScaleOffset), (Blue >> ScaleOffset));
    */

}
