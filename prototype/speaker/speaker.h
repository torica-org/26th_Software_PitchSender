#pragma once
#include <Arduino.h>

extern int O_SPK;

void speaker_init();

void speaker(float airspeed, float altitude);