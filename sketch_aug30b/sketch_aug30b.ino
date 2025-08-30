#include <ESP32Servo.h>


#define BUZZER_PIN 5   // change to your GPIO

Servo myServo;          // create servo object
const int irSensorPin = 4;
int counter=0;
int servoPin = 18;      // GPIO pin connected to servo signal
int entry=0;
TaskHandle_t readIRSensorTaskHandle = NULL;

void readIRSensorTask(void *pvParameters) {
  myServo.write(0);

  while(1) {
    int sensorState = digitalRead(irSensorPin);

    if (sensorState == LOW) {
      if(entry==0){
        entry++;
        myServo.write(180);     // move back to 0 degrees
      }
      else{
        entry=0;
        myServo.write(0);     // move back to 0 degrees
      }
      digitalWrite(BUZZER_PIN, HIGH);  // buzzer ON
      delay(100);                      // wait 0.5s
      digitalWrite(BUZZER_PIN, LOW);  // buzzer OFF
      delay(100);                      
      Serial.println("Object Detected!");
      counter+=1;
      if (counter >= 25) {
        counter=0;
      }
      Serial.print("Counter: ");
      Serial.println(counter);
    } 


  // Sweep back from 180° to 0°
 
    
  }
}

void setup() {
  
  Serial.begin(115200);

  
  pinMode(irSensorPin, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  myServo.attach(servoPin); 
          myServo.write(0);     // move back to 0 degrees
   // attach servo to pin
  Serial.println("IR Sensor Ready. Creating RTOS task...");

  
  xTaskCreatePinnedToCore(
      readIRSensorTask,      
      "Read IR Sensor Task", 
      2048,                  
      NULL,                  
      1,                     
      &readIRSensorTaskHandle, 
      1                     
  );
}

void loop(){

}