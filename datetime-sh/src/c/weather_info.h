#pragma once

#include <pebble.h>

// Structure to hold weather information
typedef struct {
  int temperature_c;      // Temperature in Celsius
  int temperature_f;      // Temperature in Fahrenheit
  char conditions[32];    // Weather conditions string
} WeatherInfo;

// Global instance of weather info
extern WeatherInfo weather_info;

// Function to initialize weather info with defaults
void weather_info_init(void);