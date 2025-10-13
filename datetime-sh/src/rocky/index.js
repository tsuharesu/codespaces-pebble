var rocky = require('rocky');

// An object to cache our date & time values,
// to minimize computations in the draw handler.
var clockData = {
  time: '',
  date: ''
};

// Global object to store weather data
var weather;

// Every hour
rocky.on('hourchange', function (event) {
  // Send a message to fetch the weather information (on startup and every hour)
  rocky.postMessage({ 'fetch': true });
});

// Every minute
// https://developer.pebble.com/docs/rockyjs/rocky/#on
rocky.on('minutechange', function (event) {
  // Current date/time
  // https://developer.pebble.com/docs/rockyjs/Date/
  var d = event.date;

  // Get current time, based on 12h or 24h format (01:00 or 1:00 AM)
  clockData.time = d.toLocaleTimeString().replace(/:\d+($| )/, '$1');

  // Day of month
  var day = d.toLocaleDateString(undefined, ({ day: 'numeric' }));

  // Month name
  var month = d.toLocaleDateString(undefined, ({ month: 'long' }));

  // Date
  clockData.date = (day + ' ' + month);

  // Force screen redraw
  rocky.requestDraw();
});

rocky.on('message', function (event) {
  // Receive a message from the mobile device (pkjs)
  var message = event.data;

  if (message.weather) {
    // Save the weather data
    weather = message.weather;

    // Request a redraw so we see the information
    rocky.requestDraw();
  }
});

// Redraw the screen
rocky.on('draw', function (event) {
  // Drawing canvas
  var ctx = event.context;

  // Clear the canvas
  // https://developer.pebble.com/docs/rockyjs/CanvasRenderingContext2D/#Canvas
  ctx.clearRect(0, 0, ctx.canvas.clientWidth, ctx.canvas.clientHeight);

  var timeFontSize = 14; // matches the '14px' default font size
  var line = 0;

  // Text formatting
  ctx.fillStyle = 'white';
  ctx.textAlign = 'start';

  ctx.fillText("$ datetime.sh", 0, timeFontSize * line);
  line++;

  // Time
  ctx.fillText(clockData.time, 0, timeFontSize * line);
  line++;

  // Date
  ctx.fillText(clockData.date, 0, timeFontSize * line);
  line++;
  line++;

  ctx.fillText("$ weather.sh", 0, timeFontSize * line);
  line++;
  
  // Weather
  if (weather) {
    drawWeather(ctx, weather, timeFontSize * line);
  } else {
    ctx.fillText("Loading...", 0, timeFontSize * line);
  }
  line++;
});

function drawWeather(ctx, weather, line) {
  // Create a string describing the weather
  var weatherString = weather.celcius + 'C - ' + weather.desc;
  // var weatherString = weather.fahrenheit + 'ºF, ' + weather.desc;

  ctx.fillText(weatherString, 0, line);
}