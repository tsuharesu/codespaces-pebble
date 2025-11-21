#include "weather_info.h"

// Global instance of weather info
WeatherInfo weather_info;

void weather_info_init(void) {
  // Set default values
  weather_info.temperature_c = 0;
  weather_info.temperature_f = 32;
  strncpy(weather_info.conditions, "Loading...", sizeof(weather_info.conditions));
}