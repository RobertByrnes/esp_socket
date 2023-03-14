const https = require('https');
const fs = require('fs');
const WebSocket = require('ws');

// create HTTPS server with SSL certificate and private key
const server = https.createServer({
  cert: fs.readFileSync('.ssl/localhost.crt'),
  key: fs.readFileSync('.ssl/localhost.key')
});

// create WebSocket server instance by passing the HTTP(S) server object and options object
const wss = new WebSocket.Server({ server });

// listen to the 'connection' event which occurs when a client connects
wss.on('connection', (ws) => {
  console.log('Client connected');
  ws.send('Welcome to the WebSocket server!');

  // listen to the 'message' event which occurs when the client sends a message
  ws.on('message', (data) => {
    console.log(`Received message: ${data}`);
    if (data === 'Connected') {
        ws.send('reboot');
    }

    // send a response to the client
    // ws.send(`You said: ${data}`);
  });

  // listen to the 'close' event which occurs when the connection is closed
  ws.on('close', () => {
    console.log('Client disconnected');
  });

});

// start the HTTPS server and listen for incoming connections
server.listen(81, () => {
  console.log(`Server listening at wss://192.168.0.89`);
});
