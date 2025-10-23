#include <pebble.h>
#include "helpers.h"
#include "weather_info.h"
#include "settings.h"

static Window *s_main_window;
static TextLayer *s_time_cmd_layer;
static TextLayer *s_time_layer;
static TextLayer *s_info_cmd_layer;
static TextLayer *s_info_layer;
static TextLayer *s_weather_cmd_layer;
static TextLayer *s_weather_layer;
static GFont s_time_font;
// Use configured command symbol. Use separate persistent buffers for each layer
static char cmd_time[32];
static char cmd_info[32];
static char cmd_weather[32];
static int s_battery_level;
static bool s_bt_connected;

static void update_weather();

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DOS_15));

  const int FONT_SIZE = 15;
  const int MARGIN = 1;
  int line_pos = MARGIN;

  s_time_cmd_layer = text_layer_init(window_layer, GRect(MARGIN, line_pos, bounds.size.w, FONT_SIZE), s_time_font, GColorClear, GColorWhite, GTextAlignmentLeft);
  line_pos += FONT_SIZE;
  s_time_layer = text_layer_init(window_layer, GRect(MARGIN, line_pos, bounds.size.w, FONT_SIZE * 3), s_time_font, GColorClear, GColorWhite, GTextAlignmentLeft);
  line_pos += FONT_SIZE * 3;
  
  s_info_cmd_layer = text_layer_init(window_layer, GRect(MARGIN, line_pos, bounds.size.w, FONT_SIZE), s_time_font, GColorClear, GColorWhite, GTextAlignmentLeft);
  line_pos += FONT_SIZE;
  s_info_layer = text_layer_init(window_layer, GRect(MARGIN, line_pos, bounds.size.w, FONT_SIZE * 2), s_time_font, GColorClear, GColorWhite, GTextAlignmentLeft);
  line_pos += FONT_SIZE * 2;
  
  s_weather_cmd_layer = text_layer_init(window_layer, GRect(MARGIN, line_pos, bounds.size.w, FONT_SIZE), s_time_font, GColorClear, GColorWhite, GTextAlignmentLeft);
  line_pos += FONT_SIZE;
  s_weather_layer = text_layer_init(window_layer, GRect(MARGIN, line_pos, bounds.size.w, FONT_SIZE), s_time_font, GColorClear, GColorWhite, GTextAlignmentLeft);

  snprintf(cmd_time, sizeof(cmd_time), "%c ./datetime.sh", app_settings.cmd_symbol);
  text_layer_set_text(s_time_cmd_layer, cmd_time);
  snprintf(cmd_info, sizeof(cmd_info), "%c ./info.sh", app_settings.cmd_symbol);
  text_layer_set_text(s_info_cmd_layer, cmd_info);
  snprintf(cmd_weather, sizeof(cmd_weather), "%c ./weather.sh", app_settings.cmd_symbol);
  text_layer_set_text(s_weather_cmd_layer, cmd_weather);
  text_layer_set_text(s_weather_layer, "Loading...");
}

static void update_weather() {
  static char weather_layer_buffer[32];
  if (app_settings.temp_unit == 'F') {
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%dF, %s", 
             weather_info.temperature_f, weather_info.conditions);
  } else {
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%dC, %s", 
             weather_info.temperature_c, weather_info.conditions);
  }
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char s_time[8];
  strftime(s_time, sizeof(s_time), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  
  static char s_date[17];
  strftime(s_date, sizeof(s_date), "%a %b %d, %Y", tick_time);
  
  static char line[] = "Wed Oct 15, 2025\n24:00";
  snprintf(line, sizeof(line), "%s\n%s", s_date, s_time);
    
  text_layer_set_text(s_time_layer, line);
}

static void update_info() {
  static char s_info_line[32];
  snprintf(s_info_line, sizeof(s_info_line), "POWER: %d%% | BT: %s", s_battery_level, s_bt_connected ? "Y" : "N");
  text_layer_set_text(s_info_layer, s_info_line);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();

  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
  }
}

static void battery_callback(BatteryChargeState state) {
  s_battery_level = state.charge_percent;
  update_info();
}

static void bluetooth_callback(bool connected) {
  s_bt_connected = connected;
  update_info();
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read tuples for data
  Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);
  // Clay settings tuples
  Tuple *conf_temp_tuple = dict_find(iterator, MESSAGE_KEY_ConfTemp);
  Tuple *conf_symbol_tuple = dict_find(iterator, MESSAGE_KEY_ConfSymbol);

  // If Clay settings present, update settings first
  if(conf_temp_tuple || conf_symbol_tuple) {
    bool settings_changed = false;
    if(conf_temp_tuple) {
      // ConfTemp should be sent as a string 'C'/'F'
      if(conf_temp_tuple->type == TUPLE_CSTRING && conf_temp_tuple->value->cstring[0]) {
        char c = conf_temp_tuple->value->cstring[0];
        if(c == 'C' || c == 'c') {
          app_settings.temp_unit = 'C';
          settings_changed = true;
        } else if(c == 'F' || c == 'f') {
          app_settings.temp_unit = 'F';
          settings_changed = true;
        }
      }
    }
    if(conf_symbol_tuple && conf_symbol_tuple->type == TUPLE_CSTRING) {
      if(conf_symbol_tuple->value->cstring[0] != '\0') {
        app_settings.cmd_symbol = conf_symbol_tuple->value->cstring[0];
        settings_changed = true;
      }
    }
    if(settings_changed) {
      settings_save();
      // Refresh command labels to use new symbol (update persistent buffers)
      snprintf(cmd_time, sizeof(cmd_time), "%c ./datetime.sh", app_settings.cmd_symbol);
      text_layer_set_text(s_time_cmd_layer, cmd_time);
      snprintf(cmd_info, sizeof(cmd_info), "%c ./info.sh", app_settings.cmd_symbol);
      text_layer_set_text(s_info_cmd_layer, cmd_info);
      snprintf(cmd_weather, sizeof(cmd_weather), "%c ./weather.sh", app_settings.cmd_symbol);
      text_layer_set_text(s_weather_cmd_layer, cmd_weather);
    }
  }

  // If weather data is present, update it
  if(temp_tuple && conditions_tuple) {
    // Get temperature in Kelvin as a float for more accurate conversion
    float temp_k = (float)temp_tuple->value->int32;
    
    // Convert Kelvin to Celsius and Fahrenheit with proper rounding
    weather_info.temperature_c = (int)(temp_k - 273.15 + 0.5f);
    weather_info.temperature_f = (int)((temp_k - 273.15) * 9.0f / 5.0f + 32.0f + 0.5f);
    
    strncpy(weather_info.conditions, conditions_tuple->value->cstring, sizeof(weather_info.conditions));
  }
  update_weather();
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void main_window_unload(Window *window) {
  text_layer_destroy_safe(s_time_cmd_layer);
  text_layer_destroy_safe(s_time_layer);
  text_layer_destroy_safe(s_info_cmd_layer);
  text_layer_destroy_safe(s_info_layer);
  text_layer_destroy_safe(s_weather_cmd_layer);
  text_layer_destroy_safe(s_weather_layer);
  fonts_unload_custom_font_safe(s_time_font);
}

static void init() {
  // Initialize settings and weather info
  settings_init();
  weather_info_init();
  
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_set_background_color(s_main_window, GColorBlack);
  
  window_stack_push(s_main_window, true);
  
  update_time();
  battery_callback(battery_state_service_peek());
  bluetooth_callback(connection_service_peek_pebble_app_connection());

  // Register time callbacks
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // Register for Battery connection updates
  battery_state_service_subscribe(battery_callback);
  // Register for Bluetooth connection updates
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });
  // Register message callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}