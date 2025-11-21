## Project Overview

**datetime.sh** is a sleek, terminal-inspired watchface for Pebble smartwatches that combines retro command-line aesthetics with modern functionality. It displays real-time date, time, and weather information alongside system status (battery level, Bluetooth connectivity, and heart rate) in a customizable command-line interface.

### Key Features

- **Real-Time Information**: Displays current date, time, temperature, and weather conditions
- **System Status Monitoring**: Shows battery percentage, Bluetooth connection status, and heart rate
- **Extensive Customization**: 
  - Choose background and font colors
  - Select temperature unit (°C or °F)
  - Customize command prompt symbol
- **Persistent Settings**: All preferences are saved locally and persist between app launches
- **Intelligent Updates**: Weather data refreshes every 30 minutes; battery, heart rate, and connectivity update in real-time
- **Retro Terminal Design**: Features DOS-inspired typography and command-line command labels

For detailed configuration options, see [SETTINGS.md](docs/SETTINGS.md).

## Supported Platforms

The app targets multiple Pebble watch models:
- aplite (Pebble Classic)
- basalt (Pebble Time)
- diorite (Pebble 2)
- emery (Pebble Time 2)
- flint (Pebble 2 Duo)

## Commands

```bash
# Build the app for all platforms
pebble build

# Clean build artifacts
pebble clean

# Install the app on specific emulator
pebble install --emulator basalt

# Screenshot the running emulator
pebble screenshot --no-open screenshot.png
```

If you need more information on the `pebble` command or a sub-command, append `--help`.

## Project Structure

```
src/c/           - C source files for the watchapp
src/pkjs/        - PebbleKitJS files, for communication with the phone app and Clay configuration file
resources/       - Images, fonts, and other resources
```
## Architecture

The application is a watchface built with the Pebble SDK and follows standard Pebble watchface architecture:

### Core Components

1. **Main Window** (`src/c/main.c`): Central display engine with six text layers organized as command-line interface:
   - Time command label and display (date + time)
   - Info command label and display (battery, Bluetooth, heart rate)
   - Weather command label and display (temperature + conditions)

2. **Settings Management** (`src/c/settings.c`, `src/c/settings.h`): 
   - Persists user preferences (colors, temperature unit, command symbol)
   - Syncs with Clay configuration system

3. **Weather Integration** (`src/c/weather_info.c`, `src/c/weather_info.h`):
   - Struct for weather information

4. **Helper Functions** (`src/c/helpers.c`, `src/c/helpers.h`):
   - Utility functions for UI and data handling

### Event Handling

- **Tick Handler**: Updates time display every minute; requests weather update every 30 minutes
- **Battery Callback**: Monitors battery level changes
- **Bluetooth Callback**: Tracks Bluetooth connectivity status
- **Health Callback**: Monitors heart rate data when available
- **Message Handler**: Processes configuration from Clay and weather data from companion app

### Data Flow

1. Companion app sends weather data and user settings to watchface via AppMessage
2. Settings are saved to persistent storage
3. Tick events trigger display updates and periodic weather requests
4. System callbacks update battery, Bluetooth, and heart rate information in real-time

## Configuration

For information about user-facing configuration options, customizable settings, and Clay integration, see [SETTINGS.md](docs/SETTINGS.md).
