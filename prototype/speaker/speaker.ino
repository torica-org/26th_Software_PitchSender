#include "speaker.h"

void setup() {
  // put your setup code here, to run once:
  speaker_init();
}

float airspeed = 12.0;
float altitude = 10.0;

int count_s = 0;
int count_a = 0;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("airspeed: ");
  Serial.print(airspeed);
  Serial.print("  altitude: ");
  Serial.println(altitude);

  speaker(airspeed, altitude);

  if (count_s > 100) {
    airspeed = airspeed - 0.1;
    if (airspeed < 0) {
      airspeed = 12.0;
    }
    count_s = 0;
  }

  if (count_a > 50){
    float altitude_step = 0.1f;
    if (altitude <= 1.1f) {
      altitude_step = 0.01f;
    }
    altitude = altitude - altitude_step;
    if (altitude < 0) {
      altitude = 12.0;
    }
    count_a = 0;
  }

  count_s++;
  count_a++;

  delay(10);
}
