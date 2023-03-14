# PlatformIO C++ ESP32 Arduino Example - README
## Introduction
This is a code snippet written in C++ for the ESP32 Arduino, using the PlatformIO development environment. The code creates an instance of AsyncWebSocket and waits for connection requests. When a Websocket connection is established to /url, the ESP8266 will receive and send data.

## Prerequisites
To run this code, you will need the following:

## ESP32 device
An installation of Arduino IDE (or any other compatible IDE)
PlatformIO plugin for your IDE
## How to Use Code
Clone or download and extract the repository containing the code.
Open the project in which you want to use the code in PlatformIO
Add the header file "ESPSocket.h" and source file "esp32_socket_webpage.h" to your project.
Make sure the favicon image file is included in your project's root directory and named as "favicon.h".
Copy and paste the code into your project.
Compile and Upload the code to your ESP32 device.
## Functions
There are several functions declared in the code:

- begin(AsyncWebServer *server, int defaultCallback, const char *url): Creates a new instance of AsyncWebSocket and sets it to listen for client connections.
Arguments:
server: A pointer to an instance of AsyncWebServer
defaultCallback: Integer value to determine whether a callback function has been set for the onEvent listener. To use the default (recvMsg()), pass in 1 as the argument. By default, no callback function is set.
url: A character array representing the URL path on the server that will be used for this WebSocket connection.
- msgCallback(RecvMsgHandler _recv): Sets the message handler callback function for the WebSocket connection.
Argument:
_recv: A function pointer to the function that will handle incoming WebSocket messages.
- print(): Sends data to all clients connected to the WebSocket.
- printf(const char *format, ...): Sends formatted text to all clients connected to the WebSocket.
- printBatteryInfo(TCallBattery &battery): Prints battery charge percentage and battery voltage for the TTGO T-Call ESP32 and other supported boards.
Argument:
battery: A reference to an instance of TCallBattery.
- printWiFiInfo(bool verbose): Prints WiFi RSSI, host, and gateway to the websocket.
Argument:
verbose: Boolean value to determine whether additional information about the network connection should be displayed.
- printESPInfo(bool verbose): Prints chip information for ESP32.
Argument:
verbose: Boolean value to determine whether additional information about the chip should be displayed.

# UI Customisation Utility
Creating a C Array for the ESP32 Webpage
To use this tool to create a C array for serving the html/css/javascript for a webpage served by an ESP32 web socket server, follow these steps:

Create the html/ directory that houses all HTML files, CSS, and JavaScript.
Edit the html/index.html file with your own HTML, CSS, and JavaScript code.
Save any changes to the HTML files, CSS or JavaScript located within the html directory.
In order to create a C array for ESP32 web server, run the python script named create_hex_array.py.
Open the terminal in the project folder and type: python create_hex_array.py
The Python script will then create a C array file esp32_socket_webpage.h located in src folder, which is used to serve the webpage from an ESP32 web socket server on a GET request to /.

PROGMEM is a way in Arduino to store data in read-only memory (ROM) instead of Random Access Memory (RAM), which is necessary when the data cannot fit into the available memories. It allows you to use flash memory, where information is stored during sketch uploading. PROGMEM keyword should be used with a variable declaration to define in which memory location to put the data.

Using PROGMEM has the following benefits:

Saves SRAM space: As the program use the ROM, it will not encroach on the limited RAM available. This is especially useful when a large amount of data needs to be stored (e.g., image or sound).
Optimized program performance: Accessing data from FLASH takes longer than it does from SRAM, so accessing data with PROGMEM may take more clock cycles than with direct RAM access. However, this can still reduce the program’s processing time because accessing data from PROGMEM is faster than loading it dynamically into RAM.
Prolongs EEPROM lifespan: Declaring variables as PROGMEM can also improve the life of EEPROM (electrically erasable programmable read-only memory). When a variable is defined as PROGMEM, it will keep its values even after power resets and will not need to be written again to the EEPROM, thus, prolonging the lifespan of the EEPROM.
Therefore, using PROGMEM saves valuable space for variables and, at the same time, improves program performance and delays the wearing out of EEPROM. All of which eventually leads to a better user experience.

# Front End Functionality - ES6 Javascript
Explanation of the JavaScript code
This code defines a class Socket that creates a new WebSocket object and handles the communication with a server over that socket. The class constructor takes in a socketDestination parameter which is the URL of the server that will be connected to.

Properties of the Socket class:
destination: stores the socketDestination passed as an argument to the class constructor.
ws: a WebSocket object created for the specified socketDestination.
state: identifies the current connection state.
serialBuffer: stores incoming data from the server before it can be updated asynchronously on the front-end.
terminal, batteryTextDiv, wifiTextDiv, espTextDiv, sendBtn are all HTML elements representing sections in the web page served by the WebSocket's GET request.
Methods of the Socket class:
sendData(): method sends data to the server when called.
updateTerminal(): method updates the user interface i.e, an HTML element or textarea, with new data received from the server.
The rest of the code outside the Socket class definition creates an instance of this class, passing the URL of the server to connect to on instantiation.
Furthermore, also it gets references to HTML elements (the input box, Send button, and BATTERY/WIFI/ESP displays), adds event listeners, and executes appropriate callbacks when conditionals get satisfied.

When the websocket connection opens, 'open' event triggers - this leads to setting this.state to 'connected'.

When it closes (due to errors or expected reasons), 'close' event triggers - this leads to logging message WebSocket closed to the console, resetting the this.state property to 'closed', and adding an automatic retry after two seconds delay by calling a new WebSocket setTimeout().

If there are any errors(e.g server not running), after trying to connect WebSocket, "error" event triggers and logs the error to the console.

Finally, when the "message" event triggers, log messages are displayed depending on the type of message received, in order to change the content of HTML elements (BATTERY/WIFI/ESP displays). If any other message is received, it'll be appended to serialBuffer then updateTerminal() will refresh the HTML element (i.e, the terminal) with the already appended information.

Overall, the Javascript code in question can serve as a template that establishes and maintains a WebSocket between a client (browser) and a server, updates the user interface with new data received from the server, while also being mindful of the server's response times to avoid latency issues.

# Conclusion
This code provides an example of how to implement a basic WebSocket server in C++ using the ESP32 Arduino platform. By using this code snippet as a foundation, you can develop more complex applications that take advantage of the ESP32's capabilities.