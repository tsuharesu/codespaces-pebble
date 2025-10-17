#include "settings.h"

#define SETTINGS_PKEY 1

// Global settings instance
AppSettings app_settings;

void settings_init(void) {
  // defaults
  app_settings.temp_unit = 'C';
  app_settings.cmd_symbol = '$';

  // load persisted (if any)
  settings_load();
}

void settings_save(void) {
  persist_write_data(SETTINGS_PKEY, &app_settings, sizeof(AppSettings));
}

void settings_load(void) {
  if (persist_exists(SETTINGS_PKEY)) {
    persist_read_data(SETTINGS_PKEY, &app_settings, sizeof(AppSettings));
    // Sanity-check values
    if (app_settings.temp_unit != 'C' && app_settings.temp_unit != 'F') {
      app_settings.temp_unit = 'C';
    }
    if (app_settings.cmd_symbol == '\0') {
      app_settings.cmd_symbol = '$';
    }
  }
}