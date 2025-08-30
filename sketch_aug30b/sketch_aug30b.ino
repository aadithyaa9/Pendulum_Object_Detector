#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BUZZER_PIN 5       // buzzer pin
const int irSensorPin = 4; // IR sensor pin
int counter = 0;

TaskHandle_t readIRSensorTaskHandle = NULL;

// Initialize LCD (address 0x27 or 0x3F depending on module, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void readIRSensorTask(void *pvParameters) {
  while (1) {
    int sensorState = digitalRead(irSensorPin);

    if (sensorState == LOW) {
      // Beep buzzer
      digitalWrite(BUZZER_PIN, HIGH);
      vTaskDelay(pdMS_TO_TICKS(100));
      digitalWrite(BUZZER_PIN, LOW);
      vTaskDelay(pdMS_TO_TICKS(100));

      // Update counter
      counter++;
      if (counter >= 25) {
        counter = 0;
      }

      // Show on LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Object Detected");
      lcd.setCursor(0, 1);
      lcd.print("Count: ");
      lcd.print(counter);

    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("All Clear");
    }

    vTaskDelay(pdMS_TO_TICKS(200)); // small delay
  }
}

void setup() {
  pinMode(irSensorPin, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Start LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IR Sensor Ready");

  // Create FreeRTOS task
  xTaskCreatePinnedToCore(
      readIRSensorTask,        // Task function
      "Read IR Sensor Task",   // Name
      2048,                    // Stack size
      NULL,                    // Parameters
      1,                       // Priority
      &readIRSensorTaskHandle, // Task handle
      1                        // Run on core 1
  );
}

void loop() {
  // Nothing here, everything runs in tasks
}