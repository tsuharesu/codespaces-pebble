var myAPIKey = 'API_KEY_HERE'; // Replace with your OpenWeatherMap API key

// https://developer.pebble.com/docs/pebblekit-js/Pebble/#on
Pebble.on('message', function (event) {
  // Get the that was passed
  var message = event.data;

  if (message.fetch) {
    navigator.geolocation.getCurrentPosition(function (pos) {
      var url = 'http://api.openweathermap.org/data/2.5/weather' +
        '?lat=' + pos.coords.latitude +
        '&lon=' + pos.coords.longitude +
        '&appid=' + myAPIKey;

      request(url, 'GET', function (respText) {
        var weatherData = JSON.parse(respText);
        Pebble.postMessage({
          'weather': {
            // Convert from Kelvin
            'celcius': Math.round(weatherData.main.temp - 273.15),
            'fahrenheit': Math.round((weatherData.main.temp - 273.15) * 9 / 5 + 32),
            'desc': weatherData.weather[0].main
          }
        });
      });
    }, function (err) {
      console.error('Error requesting location: ' + err);
    }, { timeout: 15000, maximumAge: 60000 });
  }
});

// Simple XMLHttpRequest wrapper
function request(url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
}