var util  = require('util'),
    spawn = require('child_process').spawn,
    proc    = spawn('../modbus/read', []);


var status;

proc.stdout.on('data', function (data) {
  console.log('stdout: ' + data);
  status = JSON.parse(data);
});

proc.stderr.on('data', function (data) {
  console.log('stderr: ' + data);
});

proc.on('exit', function (code) {
  console.log('child process exited with code ' + code);
});




var express = require('express')
var app = express()

app.get('/', function (req, res) {
  res.json(status);
})

app.listen(3000)


