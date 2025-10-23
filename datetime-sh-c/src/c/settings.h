#pragma once

#include <pebble.h>

// App settings received from Clay
// temp_unit: 'C' or 'F' (default 'C')
// cmd_symbol: single character shown before commands (default '$')

typedef struct {
  char temp_unit;    // 'C' or 'F'
  char cmd_symbol;   // 1-character symbol
  GColor display_color; // Background color for terminal display (default: 0x000000)
  GColor font_color;    // Font/text color (default: 0xFFFFFF)
} AppSettings;

extern AppSettings app_settings;

// Initialize settings (set defaults and load persisted values)
void settings_init(void);

// Save settings to persistent storage
void settings_save(void);

// Load settings from persistent storage
void settings_load(void);