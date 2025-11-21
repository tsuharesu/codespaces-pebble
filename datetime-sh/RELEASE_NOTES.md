# datetime.sh v2.0.0 Release Notes

## Overview
A sleek, terminal-inspired watchface for Pebble smartwatches featuring real-time weather, system information, and extensive customization options.

## New Features

### Display & Customization
- **Dual Color Customization**: Choose both background and font colors to match your personal style
- **Custom Command Symbol**: Personalize the command prompt symbol displayed before each command label
- **Temperature Unit Selection**: Display temperature in either Celsius or Fahrenheit based on your preference

### Real-Time Information Display
- **Time Display**: Shows current date and time (12/24-hour format support)
- **Weather Integration**: Displays current temperature and weather conditions with automatic updates every 30 minutes
- **System Status Bar**: Monitor device health at a glance:
  - Battery level percentage
  - Bluetooth connectivity status
  - Heart rate (when available)

### Terminal Interface
The watchface mimics a retro command-line interface with command labels:
- `$ ./datetime.sh` - Time and date
- `$ ./info.sh` - System information (battery, Bluetooth, heart rate)
- `$ ./weather.sh` - Current weather conditions

## Technical Improvements
- **Persistent Settings**: All customization preferences are saved locally and persist between app launches
- **Intelligent Updates**: Weather data refreshes every 30 minutes, battery, heart rate and connectivity updates in real-time
- **Retro DOS Font**: Features the perfect DOS 15pt font for an authentic terminal aesthetic

## Configuration
Users can customize the watchface through the Pebble companion app settings:
- Background color picker
- Font color picker
- Temperature unit toggle (°C / °F)
- Command symbol character selector

## Supported Platforms
- Pebble Classic (aplite)
- Pebble Time (basalt)
- Pebble 2 (diorite)
- Pebble Time 2 (emery)
- Pebble 2 Duo (flint)

## Requirements
- Pebble smartwatch (any compatible model)
- Pebble mobile companion app for configuration and weather data

