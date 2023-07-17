#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

int adcin = A0;
int adcvalue = 0;
float mv_per_lsb = 3300.0F / 1024.0F; // 10-bit ADC with 3.6V input range

Servo myservo; // create servo object to control a servo

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  myservo.attach(A4); // attaches the servo on pin 9 to the servo object

  static constexpr uint32_t baudrate = 115200;
  Serial.begin(baudrate);
  while (!Serial)
    delay(10); // for nrf52840 with native usb

  Serial.println("Serial Echo demo");
  Serial.print("Badurate : ");
  Serial.println(baudrate);
}

void loop()
{
  Serial.println("Test print");
  adcvalue = analogRead(adcin);
  float voltage = static_cast<float>(adcvalue * mv_per_lsb);

  // Display the results
  Serial.print(" [");
  // Serial.print((float)adcvalue * mv_per_lsb);
  Serial.print(voltage);
  Serial.println(" mV]");

  float val =
      map(voltage,
          0,
          1700,
          0,
          180); // scale it to use it with the servo (value between 0 and
  Serial.print("Writing Val: ");
  Serial.println(val);
  // myservo.write(val); // sets the servo position according to the
  // scaled value

  if (voltage < 900)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(100); // wait for a second
}