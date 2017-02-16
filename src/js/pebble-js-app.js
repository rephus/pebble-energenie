/**
* Makes a HTTP request to a REST API
* url : URL to hit
* type: GET (default)
* success: successful callback function with JSON result
* error (optional): trigger callback function with no result on error (if defined)
*/

var addresses = [
  6916878, //air pump
  6916870, //Aquarium lights
  6916874,  //Socket 3
  6916866 // film lights
];


var xhrRequest = function (url, type, success, error) {
  console.log("Making request to "+type +": "+url);
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
      if (xhr.readyState === 4) {
        console.log("Received response: "+ xhr.status);
        if (xhr.status === 200) {
        //  var json = JSON.parse(this.responseText);
        //  success(json);
          success(this.responseText);
        } else {
          console.log('Error, status code: '+ xhr.status);
          if (error) error();
        }
      }
    };
    xhr.open(type, url);
    xhr.send();
  };

// false = off, true = on
var devices = [false, false, false, false];

function triggerEnergenie(device) {

  if (device === 0) {
    for (var i=0; i< addresses.length; i++) triggerEnergenie(i);
    return;
  }

  var address = addresses[device-1];
  if (devices[device] === false) address ++;
  console.log("Triggering energenie socket " + device + " "+ status);

  //var url = 'http://192.168.2.150:5001/switch?socket='+device + "&status="+ newStatus;
  var url = 'http://192.168.2.56/cmd?code='+address+":24";
  xhrRequest(url, 'GET', function(response){
    //console.log("Returned response:" + response );
    // we don't actually need to reply back
    devices[device] = !devices[device];
/*
    Pebble.sendAppMessage({
      'WEATHER_ICON_KEY': 1,
    });*/
  });
}

Pebble.addEventListener('ready', function (e) {
  console.log('connect!' + e.ready);
  //window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError,  locationOptions);
  console.log(e.type);
});

Pebble.addEventListener('appmessage', function (e) {
  console.log("addEventListener appmessage: "+ e.type + JSON.stringify(e));
  var device = e.eventPhase;
  if (!device) {
    var firstKey = Object.keys(e.data)[0];
    device = e.data[firstKey];
  }
  //window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError,  locationOptions);
  triggerEnergenie(device);
});

Pebble.addEventListener('webviewclosed', function (e) {
  console.log('webview closed');
  console.log(e.type);
  console.log(e.response);
});
