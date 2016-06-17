

var options = JSON.parse(localStorage.getItem('options'));
//console.log('read options: ' + JSON.stringify(options));
if (options === null) options = { "use_gps" : "true",
									"location" : "",
									"units" : "fahrenheit",
									"bluetoothvibe" : "false",
									"hourlyvibe" : "false",
									"dateformat1" : "",
									"dateformat2" : "",
									"dateformat3" : ""};

function getWeatherFromLatLong(latitude, longitude) {
  var response;
  var location_name = "";
  var url = "http://maps.googleapis.com/maps/api/geocode/json?latlng=" + latitude + "," + longitude;
  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function(e) {
    if (req.readyState == 4) {
      if (req.status == 200) {
        response = JSON.parse(req.responseText);
        if (response) {
          location_name = response.results[0].formatted_address;
          getWeatherFromLocation(location_name);
        }
      } else {
        console.log("Error LatLong");
      }
    }
  };
  req.send(null);
}

function getWeatherFromLocation(location_name) {
  var response;
  var woeid = -1;

  var query = encodeURI("select woeid from geo.places(1) where text=\"" + location_name + "\"");
  var url = "http://query.yahooapis.com/v1/public/yql?q=" + query + "&format=json";
  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function(e) {
    if (req.readyState == 4) {
      if (req.status == 200) {
        // console.log(req.responseText);
        response = JSON.parse(req.responseText);
        if (response) {
          woeid = response.query.results.place.woeid;
          getWeatherFromWoeid(woeid);
        }
      } else {
        console.log("Error Location");
      }
    }
  };
  req.send(null);
}


function getWeatherFromWoeid(woeid) {
  var celsius = options['units'] == 'celsius';
  var query = encodeURI("select item.condition from weather.forecast where woeid = " + woeid +
                        " and u = " + (celsius ? "\"c\"" : "\"f\""));
  var url = "http://query.yahooapis.com/v1/public/yql?q=" + query + "&format=json";

  var response;
  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function(e) {
    if (req.readyState == 4) {
      if (req.status == 200) {
        response = JSON.parse(req.responseText);
        if (response) {
          var condition = response.query.results.channel.item.condition;
          temperature = condition.temp + (celsius ? "\u00B0" : "\u00B0");
    //      icon = imageId[condition.code];
           console.log("temp " + temperature);
  //         console.log("icon " + icon);
   //        console.log("condition " + condition.text);
          Pebble.sendAppMessage({
         //   "icon" : icon,
		//	  "condition" : condition.text,
            "temperature" : temperature,
			"bluetoothvibe" : (options["bluetoothvibe"] == "true" ? 1 : 0),
            "hourlyvibe" : (options["hourlyvibe"] == "true" ? 1 : 0),
			"dateformat1" : options["dateformat1"],
			"dateformat2" : options["dateformat2"],
			"dateformat3" : options["dateformat3"],

          });
        }
      } else {
        console.log("Error");
      }
    }
  };
  req.send(null);
}

function updateWeather() {
  if (options['use_gps'] == "true") {
    window.navigator.geolocation.getCurrentPosition(locationSuccess,
                                                    locationError,
                                                    locationOptions);
  } else {
    getWeatherFromLocation(options["location"]);
  }
}

var locationOptions = { "timeout": 15000, "maximumAge": 60000 };

function locationSuccess(pos) {
  var coordinates = pos.coords;
  getWeatherFromLatLong(coordinates.latitude, coordinates.longitude);
}

function locationError(err) {
  console.warn('location error (' + err.code + '): ' + err.message);
  Pebble.sendAppMessage({
    "temperature":"   "	 
  });
}

Pebble.addEventListener('showConfiguration', function(e) {
  var uri = 'http://www.themapman.com/pebblewatch/flipboard.html?' +
    'use_gps=' + encodeURIComponent(options['use_gps']) +
    '&location=' + encodeURIComponent(options['location']) +
    '&units=' + encodeURIComponent(options['units']) +
	'&bluetoothvibe=' + encodeURIComponent(options['bluetoothvibe']) +
    '&hourlyvibe=' + encodeURIComponent(options['hourlyvibe']) +
    '&dateformat1=' + encodeURIComponent(options['dateformat1']) +
    '&dateformat2=' + encodeURIComponent(options['dateformat2']) +
    '&dateformat3=' + encodeURIComponent(options['dateformat3']);
  //console.log('showing configuration at uri: ' + uri);

  Pebble.openURL(uri);
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (e.response) {
    options = JSON.parse(decodeURIComponent(e.response));
    localStorage.setItem('options', JSON.stringify(options));
    //console.log('storing options: ' + JSON.stringify(options));
    updateWeather();
  } else {
    console.log('no options received');
  }
});

Pebble.addEventListener("ready", function(e) {
  //console.log("connect!" + e.ready);
  updateWeather();
  setInterval(function() {
    //console.log("timer fired");
    updateWeather();
  }, 900000); // 15 minutes
  console.log(e.type);
});