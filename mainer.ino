
// Include Libraries
#include "Arduino.h"
#include "Pump.h"
#include "DHT.h"
#include "pulse-sensor-arduino.h"
#include "LED.h"
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"
#include "PiezoSensor.h"
#include "Wire.h"
#include "RTClib.h"
#include "SolenoidValve.h"
#include "IR.h"
#include "IRremoteInt.h"
#include "IRremote.h"
#include "Button.h"


// Pin Definitions
#define BLOWER_PIN_COIL1	2
#define CAPACITIVETOUCH_PIN_SIG	4
#define DFPLAYER_PIN_TX	19
#define DFPLAYER_PIN_RX	18
#define DHT_PIN_DATA	5
#define FINGERPRINTSCANNER_5V_PIN_RX	10
#define FINGERPRINTSCANNER_5V_PIN_TX	11
#define HEARTPULSE_PIN_SIG	A10
#define HEARTRATE_PIN_SIG	A0
#define LEDB_PIN_VIN	3
#define NOKIALCD_1_PIN_CS	8
#define NOKIALCD_1_PIN_RST	12
#define NOKIALCD_1_PIN_DC	9
#define NOKIALCD_2_PIN_CS	13
#define NOKIALCD_2_PIN_RST	15
#define NOKIALCD_2_PIN_DC	14
#define PIEZOSENSOR_PIN_POS	A3
#define SOLENOIDVALVE_PIN_COIL1	6
#define IR_PIN_OUT	16
#define TILTSWITCH_PIN_2	17
#define VOICERECORDER_PIN_SIG	22
#define PULLPUSHSOLENOIDLARGE_PIN_COIL1	7



// Global variables and defines
//define Nokia LCD contrast and dimentions(in pixels)
#define LCD_CONTRAST 60
#define LCD_SIZE_COL 84
#define LCD_SIZE_ROW 48
int nokiaLcd_1Counter = 0;
//define Nokia LCD contrast and dimentions(in pixels)
#define LCD_CONTRAST 60
#define LCD_SIZE_COL 84
#define LCD_SIZE_ROW 48
int nokiaLcd_2Counter = 0;
// object initialization
Pump blower(BLOWER_PIN_COIL1);
DHT dht(DHT_PIN_DATA);
PulseSensor heartpulse;
LED ledB(LEDB_PIN_VIN);
Adafruit_PCD8544 nokiaLcd_1(NOKIALCD_1_PIN_DC,NOKIALCD_1_PIN_CS,NOKIALCD_1_PIN_RST);
Adafruit_PCD8544 nokiaLcd_2(NOKIALCD_2_PIN_DC,NOKIALCD_2_PIN_CS,NOKIALCD_2_PIN_RST);
PiezoSensor piezoSensor(PIEZOSENSOR_PIN_POS);
RTC_PCF8523 rtcPCF;
SolenoidValve solenoidValve(SOLENOIDVALVE_PIN_COIL1);
IR ir(IR_PIN_OUT);
Button TiltSwitch(TILTSWITCH_PIN_2);


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
    
    pinMode(CAPACITIVETOUCH_PIN_SIG, INPUT);
    dht.begin();
    heartpulse.begin(HEARTPULSE_PIN_SIG);
    //Initialize Nokia instance
    nokiaLcd_1.begin(LCD_SIZE_COL, LCD_SIZE_ROW);
    nokiaLcd_1.setContrast(LCD_CONTRAST); //Adjust display contrast
    //Initialize Nokia instance
    nokiaLcd_2.begin(LCD_SIZE_COL, LCD_SIZE_ROW);
    nokiaLcd_2.setContrast(LCD_CONTRAST); //Adjust display contrast
    if (! rtcPCF.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
    }
    if (! rtcPCF.initialized()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtcPCF.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtcPCF.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    // initialize IR device
    ir.init();
    TiltSwitch.init();
    pinMode(VOICERECORDER_PIN_SIG, OUTPUT);
    pinMode(PULLPUSHSOLENOIDLARGE_PIN_COIL1, OUTPUT);
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {
    // Blower - Squirrel Cage (12V) - Test Code
    // The water pump will turn on and off for 2000ms (4 sec)
    blower.on(); // 1. turns on
    delay(2000);       // 2. waits 500 milliseconds (0.5 sec).
    blower.off();// 3. turns off
    delay(2000);       // 4. waits 500 milliseconds (0.5 sec).

    }
    else if(menuOption == '2') {
    // Capacitive Touch Sensor Module - Test Code
    //Read Capacitive Touch Sensor. CapacitiveTouchVar will be '1' if the sensor will be touched
    bool CapacitiveTouchVar = digitalRead(CAPACITIVETOUCH_PIN_SIG);
    Serial.print(F("Capacitive Touch Sensor: ")); Serial.println(CapacitiveTouchVar);

    }
    else if(menuOption == '3')
    {
    // Disclaimer: The DFPlayer - Mini MP3 Player is in testing and/or doesn't have code, therefore it may be buggy. Please be kind and report any bugs you may find.
    }
    else if(menuOption == '4') {
    // DHT22/11 Humidity and Temperature Sensor - Test Code
    // Reading humidity in %
    float dhtHumidity = dht.readHumidity();
    // Read temperature in Celsius, for Fahrenheit use .readTempF()
    float dhtTempC = dht.readTempC();
    Serial.print(F("Humidity: ")); Serial.print(dhtHumidity); Serial.print(F(" [%]\t"));
    Serial.print(F("Temp: ")); Serial.print(dhtTempC); Serial.println(F(" [C]"));

    }
    else if(menuOption == '5')
    {
    // Disclaimer: The Fingerprint Scanner - TTL (GT-511C3) is in testing and/or doesn't have code, therefore it may be buggy. Please be kind and report any bugs you may find.
    }
    else if(menuOption == '6') {
    // Heart Rate Pulse Sensor - Test Code
    //Measure Heart Rate
    int heartpulseBPM = heartpulse.BPM;
    Serial.println(heartpulseBPM);
    if (heartpulse.QS == true) {
    Serial.println("PULSE");
    heartpulse.QS = false;
    }
    }
    else if(menuOption == '7')
    {
    // Disclaimer: The Gravity: Analog Heart Rate Monitor Sensor (ECG) For Arduino is in testing and/or doesn't have code, therefore it may be buggy. Please be kind and report any bugs you may find.
    }
    else if(menuOption == '8') {
    // LED - Basic Blue 5mm - Test Code
    // The LED will turn on and fade till it is off
    for(int i=255 ; i> 0 ; i -= 5)
    {
        ledB.dim(i);                      // 1. Dim Led 
        delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    }                                          
    ledB.off();                        // 3. turns off
    }
    else if(menuOption == '9') {
    // Graphic LCD 84x48 - Nokia 5110 #1 - Test Code
    nokiaLcd_1.clearDisplay();             //Erase & clear display buffer
    nokiaLcd_1.setTextColor(BLACK);        //Set text color to black, background is white by default
    nokiaLcd_1.setTextSize(1);             //set text size
    nokiaLcd_1.println("circuito.io");     //print to screen
    nokiaLcd_1.println();                  
    nokiaLcd_1.setTextColor(WHITE, BLACK); //set text color to white and backgroung to black
    nokiaLcd_1.setTextSize(2);             //set text size
    nokiaLcd_1.println(nokiaLcd_1Counter++);          //print to screen
    nokiaLcd_1.display();                  //display on screen
    }
    else if(menuOption == '10') {
    // Graphic LCD 84x48 - Nokia 5110 #2 - Test Code
    nokiaLcd_2.clearDisplay();             //Erase & clear display buffer
    nokiaLcd_2.setTextColor(BLACK);        //Set text color to black, background is white by default
    nokiaLcd_2.setTextSize(1);             //set text size
    nokiaLcd_2.println("circuito.io");     //print to screen
    nokiaLcd_2.println();                  
    nokiaLcd_2.setTextColor(WHITE, BLACK); //set text color to white and backgroung to black
    nokiaLcd_2.setTextSize(2);             //set text size
    nokiaLcd_2.println(nokiaLcd_2Counter++);          //print to screen
    nokiaLcd_2.display();                  //display on screen
    }
    else if(menuOption == '11') {
    // Piezo Element - Test Code
    int piezoSensorVal = piezoSensor.read();
    Serial.print(F("Val: ")); Serial.println(piezoSensorVal);

    }
    else if(menuOption == '12') {
    // Adafruit PCF8523 Real Time Clock Assembled Breakout Board - Test Code
    //This will display the time and date of the RTC. see RTC.h for more functions such as rtcPCF.hour(), rtcPCF.month() etc.
    DateTime now = rtcPCF.now();
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print("  ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    delay(1000);
    }
    else if(menuOption == '13') {
    // 12V Solenoid Valve - 3/4'' - Test Code
    // The solenoid valve will turn on and off for 500ms (0.5 sec)
    solenoidValve.on(); // 1. turns on
    delay(500);       // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    solenoidValve.off();// 3. turns off
    delay(500);       // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.

    }
    else if(menuOption == '14') {
    // IR Receiver Diode - TSOP38238 - Test Code
    //Receive IR code into irCode
    long irCode =  ir.detect();
    // if a code was received print it to serial monitor, note that the code is of type long but printed in HEX
    // Consider changing the if statment to something like: if(irCode == "YOUR IR CODE")
    if(irCode)
    {
    Serial.print("0x");
    Serial.println(irCode, HEX);
    }
    }
    else if(menuOption == '15') {
    // Tilt Sensor - AT407 - Test Code
    //Read Tilt Switch state. 
    //if Switch is tilted function will return HIGH (1). if not function will return LOW (0). 
    //for debounce funtionality try also TiltSwitch.onPress(), .onRelease() and .onChange().
    //if debounce is not working properly try changing 'debounceDelay' variable in Button.h
    bool TiltSwitchVal = TiltSwitch.read();
    Serial.print(F("Val: ")); Serial.println(TiltSwitchVal);

    }
    else if(menuOption == '16') {
    // Voice Recorder - Test Code
    digitalWrite(VOICERECORDER_PIN_SIG, HIGH); //play button is on. 
    delay(10);  //short signal is enough.
    digitalWrite(VOICERECORDER_PIN_SIG, LOW); //play button is off. 
    delay(10);  //waits only 10 milliseconds (0.01 sec) to push the button again. change the value in the brackets (10) for a longer or shorter delay in milliseconds.
    }
    else if(menuOption == '17') {
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
    Serial.println(F("(1) Blower - Squirrel Cage (12V)"));
    Serial.println(F("(2) Capacitive Touch Sensor Module"));
    Serial.println(F("(3) DFPlayer - Mini MP3 Player"));
    Serial.println(F("(4) DHT22/11 Humidity and Temperature Sensor"));
    Serial.println(F("(5) Fingerprint Scanner - TTL (GT-511C3)"));
    Serial.println(F("(6) Heart Rate Pulse Sensor"));
    Serial.println(F("(7) Gravity: Analog Heart Rate Monitor Sensor (ECG) For Arduino"));
    Serial.println(F("(8) LED - Basic Blue 5mm"));
    Serial.println(F("(9) Graphic LCD 84x48 - Nokia 5110 #1"));
    Serial.println(F("(10) Graphic LCD 84x48 - Nokia 5110 #2"));
    Serial.println(F("(11) Piezo Element"));
    Serial.println(F("(12) Adafruit PCF8523 Real Time Clock Assembled Breakout Board"));
    Serial.println(F("(13) 12V Solenoid Valve - 3/4''"));
    Serial.println(F("(14) IR Receiver Diode - TSOP38238"));
    Serial.println(F("(15) Tilt Sensor - AT407"));
    Serial.println(F("(16) Voice Recorder"));
    Serial.println(F("(17) Large Push-Pull Solenoid"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
    			Serial.println(F("Now Testing Blower - Squirrel Cage (12V)"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing Capacitive Touch Sensor Module"));
    		else if(c == '3') 
    			Serial.println(F("Now Testing DFPlayer - Mini MP3 Player - note that this component doesn't have a test code"));
    		else if(c == '4') 
    			Serial.println(F("Now Testing DHT22/11 Humidity and Temperature Sensor"));
    		else if(c == '5') 
    			Serial.println(F("Now Testing Fingerprint Scanner - TTL (GT-511C3) - note that this component doesn't have a test code"));
    		else if(c == '6') 
    			Serial.println(F("Now Testing Heart Rate Pulse Sensor"));
    		else if(c == '7') 
    			Serial.println(F("Now Testing Gravity: Analog Heart Rate Monitor Sensor (ECG) For Arduino - note that this component doesn't have a test code"));
    		else if(c == '8') 
    			Serial.println(F("Now Testing LED - Basic Blue 5mm"));
    		else if(c == '9') 
    			Serial.println(F("Now Testing Graphic LCD 84x48 - Nokia 5110 #1"));
    		else if(c == '10') 
    			Serial.println(F("Now Testing Graphic LCD 84x48 - Nokia 5110 #2"));
    		else if(c == '11') 
    			Serial.println(F("Now Testing Piezo Element"));
    		else if(c == '12') 
    			Serial.println(F("Now Testing Adafruit PCF8523 Real Time Clock Assembled Breakout Board"));
    		else if(c == '13') 
    			Serial.println(F("Now Testing 12V Solenoid Valve - 3/4''"));
    		else if(c == '14') 
    			Serial.println(F("Now Testing IR Receiver Diode - TSOP38238"));
    		else if(c == '15') 
    			Serial.println(F("Now Testing Tilt Sensor - AT407"));
    		else if(c == '16') 
    			Serial.println(F("Now Testing Voice Recorder"));
    		else if(c == '17') 
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
