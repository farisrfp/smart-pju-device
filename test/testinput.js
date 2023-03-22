const hexCode = "5b313637393134373339312c32362e32352c3232302c3530302c34305d";

const hexCode2 = "7b2274223a32372e32352c2276223a3232302c2263223a3530302c226c223a34307d";

const array = [
    123,34,116,34,58,50,55,46,53,44,34,118,34,58,50,50,48,44,34,99,34,58,53,48,48,44,34,108,34,58,52,48,125
];

function bash64toJSON(input) {
  var buff = new Buffer(input, "base64");
  var text = buff.toString("ascii");
  var obj = JSON.parse(text);
  return obj;
}

function hexTouint8_t(input) {
  var hex = input.toString();
  var str = "";
  for (var n = 0; n < hex.length; n += 2) {
    str += String.fromCharCode(parseInt(hex.substr(n, 2), 16));
  }
  return str;
}

function intToJSON(input) {
    var str = "";
    for (var n = 0; n < input.length; n++) {
        str += String.fromCharCode(input[n]);
    }
    return JSON.parse(str);
}

// function intToJSON(input) {
//     var arr = input.map(function(num) {
//       return String(num);
//     });
//     var str = arr.join('');
//     return str;
// }

// function intToJSON(input) {
//     var str = "";
//     for (var n = 0; n < input.length; n++) {
//       str += input[n].toString();
//     }
//     return JSON.parse('"' + str + '"');
//   }
  
  

console.log(intToJSON(array));
