#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// MQ-3 sensor 
int sensorPin = A0;
int sensorValue = 0;
int threshold = 800; 

// Motor control pins
int motorIn1 = 10;
int motorIn2 = 11;
int motorPWM = 9; // control the speed

// Initializing the LCD object with the I2C address and size
LiquidCrystal_I2C lcd(0x27, 16, 2);

// SIM800L and GPS module connections
SoftwareSerial sim800l(2, 3);  
SoftwareSerial gpsSerial(4, 5); 

// Create TinyGPS++ object for GPS parsing
TinyGPSPlus gps;

// Global variable to track if a message was sent
bool alcoholDetected = false;

void setup() {
    Serial.begin(9600);    
    sim800l.begin(9600);  // Initializing gsm
    gpsSerial.begin(9600);  // Initializing gps

    lcd.init(); // Initializing lcd
    lcd.backlight();

    // Initialize motor pins
    pinMode(motorIn1, OUTPUT);
    pinMode(motorIn2, OUTPUT);
    pinMode(motorPWM, OUTPUT);

    // Display initial message on the LCD
    displayAlternatingMessage();

    // Setup GSM
    initializeSIM800L();
}

void loop() {
    // Process GPS data continuously
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }

    // Read the sensor value
    sensorValue = analogRead(sensorPin);
    
    // Print sensor value to the serial monitor
    Serial.print("MQ-3 Sensor Value: ");
    Serial.println(sensorValue);

    // If alcohol is detected, process GPS location and stop the motor
    if (sensorValue > threshold) {
        Serial.println("Alcohol detected!");
        digitalWrite(LED_BUILTIN, HIGH); 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alcohol Detected!");

        // Stop the motor
        stopMotor();

        // Send SMS based on GPS fix
        bool gpsFix = processGPSData();
        sendAlcoholDetectedMessage(gpsFix);

        delay(30000); // Delay between readings after detection

    } else {
        // If no alcohol is detected, run the motor
        Serial.println("No alcohol detected.");
        digitalWrite(LED_BUILTIN, LOW); 
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("No Alcohol");

        // Run the motor
        runMotor();
        
        // Reset flag to allow future messages
        alcoholDetected = false;
    }

    delay(1000); // 1-second delay between sensor readings
}

void initializeSIM800L() {
    sim800l.println("AT");
    delay(1000);
    
    sim800l.println("AT+CMGF=1"); 
    delay(1000);
    
    sim800l.println("AT+CSCS=\"GSM\"");
    delay(1000);
    
    sim800l.println("AT+CNMI=1,2,0,0,0");
    delay(1000);
}

void sendAlcoholDetectedMessage(bool gpsFix) {
    String message;

    if (gpsFix) {
        double latitude = gps.location.lat();
        double longitude = gps.location.lng();

        // Constructing Google Maps link
        String googleMapsLink = "https://maps.google.com/?q=" + String(latitude, 6) + "," + String(longitude, 6);
        message = "Alcohol detected! Location: " + googleMapsLink;
    } else {
        // Send message without GPS location
        message = "Alcohol detected!";
    }

    // Send SMS
    sim800l.println("AT+CMGS=\"+919901678347\"");  
    delay(1000);
    sim800l.print(message);
    delay(1000);
    sim800l.write(26);  
    delay(1000);

    Serial.println("SMS Sent: " + message);

    // Display "Location Sent" or "Alcohol detected" on the LCD
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(gpsFix ? "Location Sent!" : "Alcohol Detected!");
}

bool processGPSData() {
   bool gpsFix = false; 
   unsigned long startMillis = millis(); 
   unsigned long timeout = 20000; 

   while (millis() - startMillis < timeout) { 
       while (gpsSerial.available() > 0) {
           gps.encode(gpsSerial.read());

           if (gps.location.isUpdated()) {
               Serial.print("Latitude: ");
               Serial.println(gps.location.lat(), 6);
               Serial.print("Longitude: ");
               Serial.println(gps.location.lng(), 6);
               gpsFix = true;
               break;  
           }
       }
       if (gpsFix) break; 

       Serial.println("Waiting for GPS fix...");
       delay(100); 
   }

   if (!gpsFix) {
       Serial.println("GPS fix not obtained after 20 seconds.");
   }

   return gpsFix;  
}

// Function to stop the motor
void stopMotor() {
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, LOW);
    analogWrite(motorPWM, 0); // Set PWM to 0 to stop the motor
    Serial.println("Motor stopped.");
}

// Function to run the motor
void runMotor() {
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);
    analogWrite(motorPWM, 255); // Full speed
    Serial.println("Motor running.");
}

void displayAlternatingMessage() {
   lcd.clear();
   lcd.setCursor(5, 0);
   lcd.print("Hello!");
   delay(3000);

   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("AlcoholDetection");
   lcd.setCursor(0, 1);
   lcd.print("In Motor Vehicle");
   delay(3000);
}
