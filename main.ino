#include "Arduino.h"
#include "Pump.h"
#include "DHT.h"
#include "pulse-sensor-arduino.h"
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"
#include "PiezoSensor.h"
#include "Servo.h"
#include "SevSeg.h"
#include "SolenoidValve.h"
#include "Button.h"


// Pin Definitions
#define BLOWER_PIN_COIL1	2
#define DHT_PIN_DATA	4
#define HEARTPULSE_PIN_SIG	A10
#define NOKIALCD_PIN_CS	7
#define NOKIALCD_PIN_RST	9
#define NOKIALCD_PIN_DC	8
#define PIEZOSENSOR_PIN_POS	A0
#define SERVO360_PIN_SIG	10
#define SERVOMD_PIN_SIG	11
#define S7SEG_PIN_DIG1	17
#define S7SEG_PIN_DIG2	18
#define S7SEG_PIN_DIG3	19
#define S7SEG_PIN_DIG4	22
#define S7SEG_PIN_A	12
#define S7SEG_PIN_B	13
#define S7SEG_PIN_C	14
#define S7SEG_PIN_D	15
#define S7SEG_PIN_E	23
#define S7SEG_PIN_F	24
#define S7SEG_PIN_G	25
#define S7SEG_PIN_DECIMAL	16
#define SOLENOIDVALVE_PIN_COIL1	5
#define TILTSWITCH_PIN_2	27
#define VACCUMPUMP_PIN_COIL1	3
#define PULLPUSHSOLENOIDLARGE_PIN_COIL1	6



// Global variables and defines
//define Nokia LCD contrast and dimentions(in pixels)
#define LCD_CONTRAST 60
#define LCD_SIZE_COL 84
#define LCD_SIZE_ROW 48
int nokiaLcdCounter = 0;
const int servoMDRestPosition   = 20;  //Starting position
const int servoMDTargetPosition = 150; //Position when event is detected
int s7segCounter = 0; 
unsigned long s7segTimer = millis();
byte s7segDigitPins[] = { S7SEG_PIN_DIG1, S7SEG_PIN_DIG2, S7SEG_PIN_DIG3, S7SEG_PIN_DIG4 };
byte s7segSegmentPins[] = { S7SEG_PIN_A, S7SEG_PIN_B, S7SEG_PIN_C, S7SEG_PIN_D, S7SEG_PIN_E, S7SEG_PIN_F ,S7SEG_PIN_G, S7SEG_PIN_DECIMAL };
// object initialization
Pump blower(BLOWER_PIN_COIL1);
DHT dht(DHT_PIN_DATA);
PulseSensor heartpulse;
Adafruit_PCD8544 nokiaLcd(NOKIALCD_PIN_DC,NOKIALCD_PIN_CS,NOKIALCD_PIN_RST);
PiezoSensor piezoSensor(PIEZOSENSOR_PIN_POS);
Servo servo360;
Servo servoMD;
SevSeg s7seg(s7segDigitPins, s7segSegmentPins);
SolenoidValve solenoidValve(SOLENOIDVALVE_PIN_COIL1);
Button TiltSwitch(TILTSWITCH_PIN_2);
Pump vaccumpump(VACCUMPUMP_PIN_COIL1);


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    dht.begin();
    heartpulse.begin(HEARTPULSE_PIN_SIG);
    //Initialize Nokia instance
    nokiaLcd.begin(LCD_SIZE_COL, LCD_SIZE_ROW);
    nokiaLcd.setContrast(LCD_CONTRAST); //Adjust display contrast
    servoMD.attach(SERVOMD_PIN_SIG);
    servoMD.write(servoMDRestPosition);
    delay(100);
    servoMD.detach();
    s7seg.setBrightness(90);      //Seven-Segment LED brightness 0 - 100
    TiltSwitch.init();
    pinMode(PULLPUSHSOLENOIDLARGE_PIN_COIL1, OUTPUT);
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1')
    {
    // Disclaimer: The BME680 Temperature, Humidity, Pressure and Gas sensor is in testing and/or doesn't have code, therefore it may be buggy. Please be kind and report any bugs you may find.
    }
    else if(menuOption == '2') {
    // Blower - Squirrel Cage (12V) - Test Code
    // The water pump will turn on and off for 2000ms (4 sec)
    blower.on(); // 1. turns on
    delay(2000);       // 2. waits 500 milliseconds (0.5 sec).
    blower.off();// 3. turns off
    delay(2000);       // 4. waits 500 milliseconds (0.5 sec).

    }
    else if(menuOption == '3') {
    // DHT22/11 Humidity and Temperature Sensor - Test Code
    // Reading humidity in %
    float dhtHumidity = dht.readHumidity();
    // Read temperature in Celsius, for Fahrenheit use .readTempF()
    float dhtTempC = dht.readTempC();
    Serial.print(F("Humidity: ")); Serial.print(dhtHumidity); Serial.print(F(" [%]\t"));
    Serial.print(F("Temp: ")); Serial.print(dhtTempC); Serial.println(F(" [C]"));

    }
    else if(menuOption == '4') {
    // Heart Rate Pulse Sensor - Test Code
    //Measure Heart Rate
    int heartpulseBPM = heartpulse.BPM;
    Serial.println(heartpulseBPM);
    if (heartpulse.QS == true) {
    Serial.println("PULSE");
    heartpulse.QS = false;
    }
    }
    else if(menuOption == '5') {
    // Graphic LCD 84x48 - Nokia 5110 - Test Code
    nokiaLcd.clearDisplay();             //Erase & clear display buffer
    nokiaLcd.setTextColor(BLACK);        //Set text color to black, background is white by default
    nokiaLcd.setTextSize(1);             //set text size
    nokiaLcd.println("circuito.io");     //print to screen
    nokiaLcd.println();                  
    nokiaLcd.setTextColor(WHITE, BLACK); //set text color to white and backgroung to black
    nokiaLcd.setTextSize(2);             //set text size
    nokiaLcd.println(nokiaLcdCounter++);          //print to screen
    nokiaLcd.display();                  //display on screen
    }
    else if(menuOption == '6') {
    // Piezo Element - Test Code
    int piezoSensorVal = piezoSensor.read();
    Serial.print(F("Val: ")); Serial.println(piezoSensorVal);

    }
    else if(menuOption == '7') {
    // Servo - Generic Continuous Rotation (Micro Size) - Test Code
    // The servo will rotate CW in full speed, CCW in full speed, and will stop  with an interval of 2000 milliseconds (2 seconds) 
    servo360.attach(SERVO360_PIN_SIG);         // 1. attach the servo to correct pin to control it.
    servo360.write(180);  // 2. turns servo CW in full speed. change the value in the brackets (180) to change the speed. As these numbers move closer to 90, the servo will move slower in that direction.
    delay(2000);                              // 3. waits 2000 milliseconds (2 sec). change the value in the brackets (2000) for a longer or shorter delay in milliseconds.
    servo360.write(0);    // 4. turns servo CCW in full speed. change the value in the brackets (0) to change the speed. As these numbers move closer to 90, the servo will move slower in that direction.
    delay(2000);                              // 5. waits 2000 milliseconds (2 sec). change the value in the brackets (2000) for a longer or shorter delay in milliseconds.
    servo360.write(90);    // 6. sending 90 stops the servo 
    delay(2000);                              // 7. waits 2000 milliseconds (2 sec). change the value in the brackets (2000) for a longer or shorter delay in milliseconds.
    servo360.detach();                    // 8. release the servo to conserve power. When detached the servo will NOT hold it's position under stress.
    }
    else if(menuOption == '8') {
    // Standard Size - High Torque - Metal Gear Servo - MG995 - Test Code
    // The servo will rotate to target position and back to resting position with an interval of 500 milliseconds (0.5 seconds) 
    servoMD.attach(SERVOMD_PIN_SIG);         // 1. attach the servo to correct pin to control it.
    servoMD.write(servoMDTargetPosition);  // 2. turns servo to target position. Modify target position by modifying the 'ServoTargetPosition' definition above.
    delay(500);                              // 3. waits 500 milliseconds (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    servoMD.write(servoMDRestPosition);    // 4. turns servo back to rest position. Modify initial position by modifying the 'ServoRestPosition' definition above.
    delay(500);                              // 5. waits 500 milliseconds (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    servoMD.detach();                    // 6. release the servo to conserve power. When detached the servo will NOT hold it's position under stress.
    }
    else if(menuOption == '9') {
    // 7 - Segment Display - 20mm - Test Code
    if (millis() - s7segTimer >= 50) 
    {
    s7segTimer = millis();
    s7segCounter++;                           //increment counter
    s7seg.setNumber(s7segCounter, 1);   //set display value
    }
    s7seg.refreshDisplay(); // Must run repeatedly
    }
    else if(menuOption == '10') {
    // 12V Solenoid Valve - 3/4'' - Test Code
    // The solenoid valve will turn on and off for 500ms (0.5 sec)
    solenoidValve.on(); // 1. turns on
    delay(500);       // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    solenoidValve.off();// 3. turns off
    delay(500);       // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.

    }
    else if(menuOption == '11') {
    // Tilt Sensor - AT407 - Test Code
    //Read Tilt Switch state. 
    //if Switch is tilted function will return HIGH (1). if not function will return LOW (0). 
    //for debounce funtionality try also TiltSwitch.onPress(), .onRelease() and .onChange().
    //if debounce is not working properly try changing 'debounceDelay' variable in Button.h
    bool TiltSwitchVal = TiltSwitch.read();
    Serial.print(F("Val: ")); Serial.println(TiltSwitchVal);

    }
    else if(menuOption == '12') {
    // Vacuum Pump - 12V - Test Code
    // The water pump will turn on and off for 2000ms (4 sec)
    vaccumpump.on(); // 1. turns on
    delay(2000);       // 2. waits 500 milliseconds (0.5 sec).
    vaccumpump.off();// 3. turns off
    delay(2000);       // 4. waits 500 milliseconds (0.5 sec).

    }
    else if(menuOption == '13') {
    // Large Push-Pull Solenoid - Test Code
    digitalWrite(PULLPUSHSOLENOIDLARGE_PIN_COIL1, HIGH); //Turn Solenoid on. 
    delay(1000);  //waits 1000 milliseconds (1 sec). change the value in the brackets (1000) for a longer or shorter delay in milliseconds.
    digitalWrite(PULLPUSHSOLENOIDLARGE_PIN_COIL1, LOW); //Turn Solenoid off. 
    delay(1000);  //waits 1000 milliseconds (1 sec). change the value in the brackets (1000) for a longer or shorter delay in milliseconds.
    }
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) BME680 Temperature, Humidity, Pressure and Gas sensor"));
    Serial.println(F("(2) Blower - Squirrel Cage (12V)"));
    Serial.println(F("(3) DHT22/11 Humidity and Temperature Sensor"));
    Serial.println(F("(4) Heart Rate Pulse Sensor"));
    Serial.println(F("(5) Graphic LCD 84x48 - Nokia 5110"));
    Serial.println(F("(6) Piezo Element"));
    Serial.println(F("(7) Servo - Generic Continuous Rotation (Micro Size)"));
    Serial.println(F("(8) Standard Size - High Torque - Metal Gear Servo - MG995"));
    Serial.println(F("(9) 7 - Segment Display - 20mm"));
    Serial.println(F("(10) 12V Solenoid Valve - 3/4''"));
    Serial.println(F("(11) Tilt Sensor - AT407"));
    Serial.println(F("(12) Vacuum Pump - 12V"));
    Serial.println(F("(13) Large Push-Pull Solenoid"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
    			Serial.println(F("Now Testing BME680 Temperature, Humidity, Pressure and Gas sensor - note that this component doesn't have a test code"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing Blower - Squirrel Cage (12V)"));
    		else if(c == '3') 
    			Serial.println(F("Now Testing DHT22/11 Humidity and Temperature Sensor"));
    		else if(c == '4') 
    			Serial.println(F("Now Testing Heart Rate Pulse Sensor"));
    		else if(c == '5') 
    			Serial.println(F("Now Testing Graphic LCD 84x48 - Nokia 5110"));
    		else if(c == '6') 
    			Serial.println(F("Now Testing Piezo Element"));
    		else if(c == '7') 
    			Serial.println(F("Now Testing Servo - Generic Continuous Rotation (Micro Size)"));
    		else if(c == '8') 
    			Serial.println(F("Now Testing Standard Size - High Torque - Metal Gear Servo - MG995"));
    		else if(c == '9') 
    			Serial.println(F("Now Testing 7 - Segment Display - 20mm"));
    		else if(c == '10') 
    			Serial.println(F("Now Testing 12V Solenoid Valve - 3/4''"));
    		else if(c == '11') 
    			Serial.println(F("Now Testing Tilt Sensor - AT407"));
    		else if(c == '12') 
    			Serial.println(F("Now Testing Vacuum Pump - 12V"));
    		else if(c == '13') 
    			Serial.println(F("Now Testing Large Push-Pull Solenoid"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}
