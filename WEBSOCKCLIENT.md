# ESPRESSIF ESP-IDF

To achieve this in C++, you need to use a networking library, such as the popular Espressif IoT Development Framework (ESP-IDF), which provides a high-level WebSocket API for establishing a client WebSocket connection.

Here are the basic steps to create a WebSocket client on an ESP32 using ESP-IDF:

Configure your network: Before establishing the WebSocket connection, you need to connect your ESP32 to a WiFi network or other network that can provide internet access.

Install the WebSocket library: The ESP-IDF comes with a built-in WebSocket library that simplifies creating and managing WebSocket client connections. You can add this to your project by adding USE_ESP_WEBSOCKET_CLIENT to your ESP-IDF build flags in your Makefile or CMakeLists.txt.

Create WebSocket Connection: Use the provided API to create a WebSocket client connection, specifying the appropriate options such as destination URL, WebSocket protocol version, and any custom headers or request parameters.

Send Data: Once the WebSocket connection is established successfully, you can use the esp_websocket_client_send() function to send data to the server.

Receive Data: To receive data from the server, you need to set up an event handler that listens for incoming messages through the WebSocket. This can be done using the ESP_EVENT_ANY_ID macro or you can specificy a dedicated event ID when calling the esp_event_handler_register() funciton.

Close the WebSocket Connection: To close the WebSocket connection cleanly, you can use the esp_websocket_client_close() function.

Here is some example code that demonstrates establishing a simple WebSocket client connection:

```c++
#include "esp_websocket_client.h"

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void app_main(void)
{
    // Configure network here
    
    esp_websocket_client_handle_t client = NULL;
    
    esp_websocket_client_config_t config = {};
    config.uri = "wss://your.server.net/ws";
    config.disable_auto_reconnect = false;
    config.cert_pem = server_cert_pem_start;
    
    esp_websocket_client_handle_t client = esp_websocket_client_init(&config);
    
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, event_handler, NULL);
    
    esp_websocket_client_start(client);
    
    esp_websocket_client_send_text(client, "Hello from ESP32!");
    
    while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    
    esp_websocket_client_stop(client);
    esp_websocket_client_destroy(client);
}
```
This basic code establishes a websocket connection to the specified uri and sends a text message ("Hello from ESP32!"). This should give a foundation for how to properly set up a WebSocket client connection on an ESP32 device.

# ARDUINO FRAMEWORK
To use ESP32 as a WebSocket client on the Arduino framework, you can make use of the WebSocketsClient library which provides a simple way to establish and manage WebSocket connections from an ESP32.

## Here is an example:

Install the WebSocketsClient library from the Arduino IDE's Library Manager or by downloading the latest release from its Github repository.

- Include the WebSocketsClient library in your sketch:

```C++
#include <WebSocketsClient.h>
```
- Create a global instance of WebSocketsClient:

```C++
WebSocketsClient webSocket;
```

- Define a callback function to handle incoming WebSocket messages:

```C++
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("[WebSocket] Disconnected");
      break;
      
    case WStype_CONNECTED:
      Serial.println("[WebSocket] Connected");
      // send a message when connected
      webSocket.sendTXT("Hello, server!");
      break;
      
    case WStype_TEXT:
      Serial.printf("[WebSocket] Received text: %s\n", payload);
      // do something with the incoming message
      break;
      
    case WStype_BIN:
      // handle binary data
      break;
    
    default:
      break;
  }
}
```

This function will be called whenever a WebSocket event occurs, such as connecting, disconnecting, or receiving data.

In your ```setup()``` function, configure the WiFi connection and connect to the WebSocket server using the ```begin()``` method with the server URL and port:

```C++
void setup() {
  // Connect to WiFi
  WiFi.begin("your_network_name", "your_network_password");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("[WiFi] Connecting...");
  }

  // Connect to WebSocket server
  webSocket.begin("ws://your.websocket.server.com", 80);
  webSocket.onEvent(webSocketEvent);

  Serial.println("[WebSocket] Connecting...");
}
```

Make sure to replace your_network_name, your_network_password, your.websocket.server.com, and 80 with your actual WiFi network credentials and WebSocket server details.

Finally, in your loop() function, call the loop() method of the WebSocketsClient object to handle incoming and outgoing WebSocket events:

```C++
void loop() {
  webSocket.loop();
  
  // Do other stuff...
}
```
That's it! Your ESP32 is now connected to the WebSocket server as a client and can send and receive messages. You can use the ```sendTXT()``` method of the webSocket object to send text messages, or the sendBIN() method to send binary data.

## A complementary script in php8.0 to run on the server to receive the connection

```php
<?php
use Ratchet\WebSocket\WsServer;
use Ratchet\Http\HttpServer;
use Ratchet\Server\IoServer;
use MyApp\WebSocket;

require_once __DIR__ . '/vendor/autoload.php';

$server = IoServer::factory(
    new HttpServer(
        new WsServer(
            new WebSocket()
        )
    ),
    8080
);

$server->run();
```

This script creates an instance of the IoServer class that listens on port 8080, handles HTTP requests with the HttpServer class, and passes WebSockets requests to the WebSocket class. The WebSocket class should handle incoming messages and manage client connections.

Note: The MyApp namespace needs to be replaced with your own namespace where you define your own WebSocket class.

Here's an example code for a WebSocket class implementation in JavaScript:

```Javascript
class WebSocketServer {
  constructor(port) {
    this.port = port;
    this.clients = new Set();
  }

  listen() {
    const server = require('http').createServer();
    const io = require('socket.io')(server, { transports: ['websocket'] });
    
    io.on('connection', (client) => {
      console.log(`Client connected to WebSocket on port ${this.port}`)

      this.clients.add(client);

      client.on('message', (data) => {
        console.log(`Received data from client: ${data}`);
        // Do something with the received data
      });

      client.on('close', () => {
        console.log(`Client disconnected from WebSocket on port ${this.port}`)
        this.clients.delete(client);
      });
    });

    server.listen(this.port, () => {
      console.log(`WebSocket server is listening on port ${this.port}`);
    });
  }

  broadcast(data) {
    this.clients.forEach((client) => {
      if (client.readyState === 1) {
        client.send(data);
      }
    });
  }
}
```

In this implementation, the WebSocketServer class accepts a port number as an argument in the constructor, and exposes two methods - listen, which sets up the WebSocket server and listens for incoming connections, and broadcast, which sends the supplied data to all connected clients.

To use this class, you can create an instance of the WebSocketServer class and call the listen method:

```Javascript
const port = 8080;
const wsServer = new WebSocketServer(port);

wsServer.listen();
```

This will create a WebSocket server on port 8080 and start listening for incoming connections. You can then use the broadcast method to send data to all connected clients:

```Javascript
wsServer.broadcast("Hello, world!");
This will send the string "Hello, world!" to all currently connected WebSocket clients.
```

Certainly, this is an example implementation of a WebSocket server in PHP 8.0:

```php
/**
 * This WebSocket class can be used by creating a new
 * instance of it and starting the server using the 
 * start() method. Connections can be handled using
 * the handleClient() method and sending
 * data back to the client.
 */
class WebSocket
{
    private $address;
    private $ports;
    private $clients = [];

    public function __construct($address = "localhost", $ports = [8080])
    {
        $this->address = $address;
        $this->ports = $ports;
    }

    public function start()
    {
        foreach ($this->ports as $port) {
            $socketUrl = "tcp://{$this->address}:{$port}";
            echo "Starting server at {$socketUrl}\n";
            $socket = stream_socket_server($socketUrl, $errno, $errstr);
            if (!$socket) {
                echo "{$errstr} ({$errno})\n";
            } else {
                echo "Server started.\n";
                $this->bindClients($socket);
            }
        }
    }

    private function bindClients($socket)
    {
        while ($client = stream_socket_accept($socket, -1)) {
            stream_set_blocking($client, false);
            
            $id = md5(uniqid());
            $key = null;
            $headers = "";
            do {
                $header = fgets($client);
                $headers .= $header;

                if (strpos($header, "Sec-WebSocket-Key") !== false) {
                    $key = trim(substr($header, strlen("Sec-WebSocket-Key:")));
                }
            } while (!empty(trim($header)));

            $responseHeaders = [
                "HTTP/1.1 101 Switching Protocols",
                "Upgrade: websocket",
                "Connection: Upgrade",
                "Sec-WebSocket-Accept: " . base64_encode(sha1($key . "258EAFA5-E914-47DA-95CA-C5AB0DC85B11", true)),
            ];
            
            fwrite($client, implode("\r\n", $responseHeaders) . "\r\n\r\n");
            $this->clients[$id] = [
                "resource" => $client,
                "connected_at" => time(),
            ];
            echo "New client connected with ID '$id'.\n";

            $this->handleClient($id);
        }
    }

    public function handleClient($clientId)
    {
        $client = &$this->clients[$clientId]["resource"];
        $data = "";

        while (!feof($client)) {
            $buffer = fread($client, 1024);

            if (!$buffer) {
                break;
            }

            $length = ord($buffer[1]) & 127;
            if ($length === 126) {
                $masks = substr($buffer, 4, 4);
                $data = substr($buffer, 8);
            } else if ($length === 127) {
                $masks = substr($buffer, 10, 4);
                $data = substr($buffer, 14);
            } else {
                $masks = substr($buffer, 2, 4);
                $data = substr($buffer, 6);
            }

            $unwrappedData = "";
            $maskIndex = 0;
            for ($i = 0; $i < strlen($data); $i++) {
                $unwrappedData .= $data[$i] ^ $masks[$maskIndex];
                $maskIndex = ($maskIndex + 1) % 4;
            }

            $fullPayload = json_decode($unwrappedData, true);
            if (!empty($fullPayload)) {
                $parsedPayload = $fullPayload["message"];
                
                // Implement your custom logic to handle parsed payload here

                $respPayload = json_encode(["success" => true]);
                $this->send($clientId, $respPayload);
            }
        }

        unset($this->clients[$clientId]);
        fclose($client);
    }

    public function send($clientId, $payload)
    {
        if (isset($this->clients[$clientId])) {
            $client = &$this->clients[$clientId]["resource"];
            $len = strlen($payload);
            $firstByte = chr(129);
            $lengthField = "";
            if ($len <= 125) {
                $secondByte = chr($len);
            } elseif ($len <= 65535) {
                $secondByte = chr(126);
                $lengthField = pack("n", $len);
            } else {
                $secondByte = chr(127);
                $lengthField = pack("xxxxN", $len);
            }
            fwrite($client, $firstByte . $secondByte . $lengthField . $payload);
        }
    }
}
