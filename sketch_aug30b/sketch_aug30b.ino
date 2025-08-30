#define BUZZER_PIN 5   // change to your GPIO

const int irSensorPin = 4;
int counter=0;

TaskHandle_t readIRSensorTaskHandle = NULL;

void readIRSensorTask(void *pvParameters) {
  

  while(1) {
    int sensorState = digitalRead(irSensorPin);

    if (sensorState == LOW) {
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
  }
}

void setup() {
  
  Serial.begin(115200);

  
  pinMode(irSensorPin, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

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