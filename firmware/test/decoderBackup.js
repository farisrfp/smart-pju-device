// Decode uplink function.
//
// Input is an object with the following fields:
// - bytes = Byte array containing the uplink payload, e.g. [255, 230, 255, 0]
// - fPort = Uplink fPort.
// - variables = Object containing the configured device variables.
//
// Output must be an object with the following fields:
// - data = Object representing the decoded payload.
function decodeUplink(input) {
  var bytes = input.bytes;

  if(bytes.length == 12) {
    var timeUnix = (bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3]);
  	var temperature = (bytes[4]<<8 | bytes[5]) / 100;
    var voltage = (bytes[6]<<8 | bytes[7]) / 100;
    var current = (bytes[8]<<8 | bytes[9]);
    var light = (bytes[10]<<8 | bytes[11]);
    
    return {
    	data : {
          'time' : timeUnix,
          'temp' : temperature,
          'volt' : voltage,
          'current' : current,
          'light' : light
        }
    }
  }
}

// Encode downlink function.
//
// Input is an object with the following fields:
// - data = Object representing the payload that must be encoded.
// - variables = Object containing the configured device variables.
//
// Output must be an object with the following fields:
// - bytes = Byte array containing the downlink payload.
function encodeDownlink(input) {
  return {
    bytes: [225, 230, 255, 0]
  };
}