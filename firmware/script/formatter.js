input = {
  method: "setValueDimmer",
  params: 100,
  additionalInfo: null,
};

var methods = ["setValueDimmer", "setValueSwitch"];

function encodeDownlink(input) {
  return {
    bytes: [methods.indexOf(input.data.method), input.data.params, input.data.additionalInfo],
    fPort: 1,
  };
}

function decodeDownlink(input) {
  var method = methods[input.bytes[0]];
  var params = input.bytes[1];
  return {
    method: method,
    params: params,
    additionalInfo: null,
  };
}