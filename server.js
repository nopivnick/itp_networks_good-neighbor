/*
   Attributions:

   - Tom Igoe's RestTest example code:
     https://github.com/tigoe/NodeExamples/tree/master/RestTest

   REST Test
   A minimal server to test a RESTful API.

   The RESTful API in this case is for a networked device that handles noise complaints without involving the police.

   The endpoints are:
   /data         - all data as json object
   /playState    - playing, paused
   /decibels     - noise in decibels, a float
   /warning      - 3 (high), 2 (medium), 1 (low), 0 (done)
   /volume       - volume level on the device, a float // float may depend on implementation
   /track        - a specific track from a playlist
   /datetime     - the current date and time

   You can GET the value of any of the endpoints, or
   you can POST to them.
*/

let express = require('express'); // include the express library
let server = express(); // create a server using express
let bodyParser = require('body-parser'); // include body-parser
let logger = require('morgan');
server.use('/', express.static('public')); // serve static files from /public
server.use(logger("common"));

// get the time:
let now = new Date();

// make a data object representing all your parameters:
let killswitch = {
  data: 0, // all data as json object
  playState: 0, // playing, paused
  decibels: 0, // noise in decibels, a float
  warning: 3, // 3 (high), 2 (medium), 1 (low), 0 (done)
  volume: 0, // volume level on the device, a float // may depend on implementation
  track: 0, // a specific track from a playlist
  datetime: now // the current date and time
}

// you need a couple of parsers for the body of a POST request:
server.use(bodyParser.json()); // for application/json
server.use(bodyParser.urlencoded({
  extended: true
})); // for application/x-www-form-urlencoded


// this handles all GET requests. You may want to make
// separate functions for each API endpoint instead:
function handleGetRequest(request, response) {
  // print out the info from the request:
  printRequestInfo(request);
  let result = '';
  // find out the REST API path, and get the appropriate property:
  switch (request.path) {
    case '/data':
      result = killswitch.data;
      break;
    case '/playState':
      result = killswitch.playState;
      break;
    case '/decibels':
      result = killswitch.decibels;
      break;
    case '/warning':
      result = killswitch.warning;
      break;
    case '/volume':
      result = killswitch.volume;
      break;
    case '/track':
      result = killswitch.track;
      break;
    case '/datetime':
      result = killswitch.datetime;
      break;
  }
  response.json(killswitch);
  // response.end(result.toString());
}

// this handles all POST requests. You may want to make
// separate functions for each API endpoint instead.
// NOTE: This does NOT check that your parameter values are valid. You should do that.

function handlePostRequest(request, response) {
  printRequestInfo(request);
  let result = '';
  // iterate over the properties in request.params:
  for (property in request.params) {
    // set the killswitch item with the same name as the
    // param that's set in request.params:
    killswitch[property] = parseInt(request.params[property]);
    // save the result so you can reply to the client:

    result = parseInt(request.params[property]);

    // if (property != "datetime") {
    //   result = parseInt(result);
    // }
    console.log(result);
    console.log(typeof(result));
  }
  response.end(result.toString());
}

// this pulls put elements of the request which you may want:
function printRequestInfo(request) {
  // the IP address of the client, the request method, and the path:
  console.log('Client IP address: ' + request.ip);
  console.log('Request method: ' + request.method);
  // the path is the API endpoint:
  console.log('Request path: ' + request.path);

  // if it's a POST request, you might want the params or
  // the body:
  if (request.method === 'POST') {
    console.log('Request parameters: ');
    console.log(request.params);
    console.log('Request body: ');
    console.log(request.body);
  }
  // If it's a GET, you can only query. It's not
  // very RESTful, but here's how you do it:
  if (request.method === 'GET') {
    console.log('Request Query: ');
    console.log(request.query);
  }
}

// here are all your endpoints. The pattern is:
// GET the  current value, or
// POST the new value as a request param:
server.get('/data', handleGetRequest);
server.post('/data/:data', handlePostRequest);
server.get('/playState', handleGetRequest);
server.post('/playState/:playState', handlePostRequest);
server.get('/decibels', handleGetRequest);
server.post('/decibels/:decibels', handlePostRequest);
server.get('/warning', handleGetRequest);
server.post('/warning/:warning', handlePostRequest);
server.get('/volume', handleGetRequest);
server.post('/volume/:volume', handlePostRequest);
server.get('/track', handleGetRequest);
server.post('/track/:track', handlePostRequest);
server.get('/datetime', handleGetRequest);
server.post('/datetime/:datetime', handlePostRequest);

server.listen(8080); // start the server
