const myAPIKey = "API_HERE"; // Replace with your OpenWeatherMap API key

function locationSuccess(pos) {
  // Construct URL
  var url =
    "http://api.openweathermap.org/data/2.5/weather?lat=" +
    pos.coords.latitude +
    "&lon=" +
    pos.coords.longitude +
    "&appid=" +
    myAPIKey;

  // Send request to OpenWeatherMap
  xhrRequest(url, "GET", function (responseText) {
    // responseText contains a JSON object with weather info
    var weatherData = JSON.parse(responseText);

    // Assemble dictionary using our keys from package.json
    const weather = {
      // Convert from Kelvin
      TEMPERATURE_C: Math.round(weatherData.main.temp - 273.15),
      TEMPERATURE_F: Math.round(
        ((weatherData.main.temp - 273.15) * 9) / 5 + 32
      ),
      CONDITIONS: weatherData.weather[0].main,
    };

    console.log("Weather is " + JSON.stringify(weather));

    // Send to Pebble
    Pebble.sendAppMessage(
      weather,
      function (e) {
        console.log("Weather info sent to Pebble successfully!");
      },
      function (e) {
        console.log("Error sending weather info to Pebble!");
      }
    );
  });
}

function locationError(err) {
  console.log("Error requesting location!", err);
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(locationSuccess, locationError, {
    timeout: 15000,
    maximumAge: 60000,
  });
}

// Listen for when the watchface is opened
Pebble.addEventListener("ready", function (e) {
  console.log("PebbleKit JS ready!");

  // Get the initial weather
  getWeather();
});

// Listen for when an AppMessage is received
Pebble.addEventListener("appmessage", function (e) {
  console.log("AppMessage received!");
  getWeather();
});

// Simple XMLHttpRequest wrapper
function xhrRequest(url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
}
