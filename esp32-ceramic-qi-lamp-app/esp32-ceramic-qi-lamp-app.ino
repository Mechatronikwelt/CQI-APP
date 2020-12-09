/************************************************************
  date:     20201206
  name:     Jonas Wackershauser, Hendrik Döring
  project:  webserver with AdaFruit WS2812 RGB-LED
  info:
          project for Informationstechnik at HsKa in term 7

          Green 0...<100% - 0...<255
          Red   0...<100% - 0...<255
          Blue  0...<100% - 0...<255
          using char 8 Bit

  version:
          0.1 initial release

************************************************************/



/************************************************************************************
  Credits:

  Webserver Template by Rui Santos
  https://randomnerdtutorials.com/esp32-web-server-arduino-ide/

  OLED Template by nhatuan84
  http://www.iotsharing.com/2017/05/how-to-use-arduino-esp32-to-display-oled.html

************************************************************************************/



/************************************************************
                          INCLUDES
************************************************************/

// load RGB LED libary
//v1.2.0
#include <Adafruit_NeoPixel.h>

// load Wi-Fi library
#include <WiFi.h>

// load for OLED display
#include <SPI.h>
#include <Wire.h>
//v1.10.3
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

// define output PIN and LED quantity
#define PIN       2    // DIN Data in of RGBLED
#define NUMPIXELS 8    // how many RGBLED are there
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

//PIN setting OLED
#define OLED_SDA 4
#define OLED_SCL 5
Adafruit_SH1106 display(4, 5);

/************************************************************
                         GLOBAL VAR
************************************************************/
///////////////////
// ATTENTION!!!! //
///////////////////
// replace with your network credentials and uncomment!
//const char* ssid = "";
//const char* password = "";
const char* ssid = "FRITZ!Box 7590 VL";
const char* password = "56616967766283031728";
// set web server port number to 80
WiFiServer server(80);
// variable to store the HTTP request
String header;
// current time
unsigned long currentTime = millis();
// previous time
unsigned long previousTime = 0;
// define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// global var to save current color, status
int act_R = 0;
int act_G = 0;
int act_B = 0;
int act_status = 0;
enum modi {constant,flash,strobe,smooth};
modi act_modi = constant;

/************************************************************
                          INIT
************************************************************/
void setup() {
  // initialize NeoPixel object
  strip.begin();
  // initialize all pixels to 'off'
  strip.show();

  Serial.begin(115200);
  // give me time to bring up serial monitor
  delay(1000);

  // initialize OLED with I2C address 0x3C
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  // OLED settings
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // serial print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  // display server ip address OLED
  display.println(WiFi.localIP());
  display.display();
}

/************************************************************
                          FUNCTION
************************************************************/

// declaration of LED function
void LEDStart(bool status, long color, int16_t brightness, modi modi);

/************************************************************
                          MAIN
************************************************************/
void loop() {

/************************************************************
                      WEBSERVER
************************************************************/
  
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Call the color functions in these if statements
            if (header.indexOf("GET /Red/on") >= 0) {
              Serial.println("Red called");
              //Insert Red #FF0000 on condition here
              LEDStart(true, 0xFF0000, 0, act_modi);
            } else if (header.indexOf("GET /Orange/on") >= 0) {
              Serial.println("Orange called");
              //Insert Orange #FF8500 on condition here
              LEDStart(true, 0xFF4500, 0, act_modi);
            } else if (header.indexOf("GET /Yellow/on") >= 0) {
              Serial.println("Yellow called");
              //Insert Yellow #FFEF00 on condition here
              LEDStart(true, 0xFF7500, 0, act_modi);
            } else if (header.indexOf("GET /Lime/on") >= 0) {
              Serial.println("Lime called");
              //Insert Lime #00FF00 on condition here
              LEDStart(true, 0x00FF00, 0, act_modi);
            } else if (header.indexOf("GET /Green/on") >= 0) {
              Serial.println("Green called");
              //Insert Green #008000 on condition here
              LEDStart(true, 0x009020, 0, act_modi);
            } else if (header.indexOf("GET /Cyan/on") >= 0) {
              Serial.println("Cyan called");
              //Insert Cyan #00FFFF on condition here
              LEDStart(true, 0x00d6FF, 0, act_modi);
            } else if (header.indexOf("GET /Blue/on") >= 0) {
              Serial.println("Blue called");
              //Insert Blue #0000FF on condition here
              LEDStart(true, 0x0000FF, 0, act_modi);
            } else if (header.indexOf("GET /Pink/on") >= 0) {
              Serial.println("Pink called");
              //Insert Pink #FF00FF on condition here
              LEDStart(true, 0xFF00FF, 0, act_modi);
            } else if (header.indexOf("GET /Purple/on") >= 0) {
              Serial.println("Purple called");
              //Insert Purple #800080 on condition here
              LEDStart(true, 0x8900fc, 0, act_modi);
            } else if (header.indexOf("GET /Offswitch/on") >= 0) {
              Serial.println("Offswitch called");
              //Insert Off condition here
              LEDStart(false, 0, 0, constant);
            } else if (header.indexOf("GET /Onswitch/on") >= 0) {
              Serial.println("Onswitch called");
              //Insert Off condition here
              LEDStart(true, 0, 0, act_modi);
            } else if (header.indexOf("GET /Br.UP") >= 0) {
              Serial.println("Br.UP called");
              //Insert Off condition here
              LEDStart(true, 0, 10, act_modi);
            } else if (header.indexOf("GET /Br.DN") >= 0) {
              Serial.println("Br.DN called");
              //Insert Off condition here
              LEDStart(true, 0, -10, act_modi);
            } else if (header.indexOf("GET /Flash/on") >= 0) {
              Serial.println("Flash on called");
              //Insert Off condition here
              LEDStart(true, 0, 0, flash);
            } else if (header.indexOf("GET /Flash/off") >= 0) {
              Serial.println("Flash off called");
              //Insert Off condition here
              LEDStart(true, 0, 0, constant);
            } else if (header.indexOf("GET /Strobe/on") >= 0) {
              Serial.println("Strobe on called");
              //Insert Off condition here
              LEDStart(true, 0, 0, strobe);
            } else if (header.indexOf("GET /Strobe/off") >= 0) {
              Serial.println("Strobe off called");
              //Insert Off condition here
              LEDStart(true, 0, 0, constant);
            } else if (header.indexOf("GET /Smooth/on") >= 0) {
              Serial.println("Smooth on called");
              //Insert Off condition here
              LEDStart(true, 0, 0, smooth);
            } else if (header.indexOf("GET /Smooth/off") >= 0) {
              Serial.println("Smooth off called");
              //Insert Off condition here
              LEDStart(true, 0, 0, constant);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // Button color and style definitions.
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;  margin-left: auto; margin-right: auto;} ");
            client.println(".button_grey { background-color: #555555; border: 2px solid #000000; color: white; border-radius: 50%; width: 85px; height: 85px;} "); 
            client.println(".button_white{ background-color: #FFFFFF; border: 2px solid #000000; color: black; border-radius: 50%; width: 85px; height: 85px;} "); 
            client.println(".button_black{ background-color: #000000; border: 2px solid #555555; color: white; border-radius: 50%; width: 85px; height: 85px;} ");
            client.println(".button_red { background-color: #FF0000; border: 2px solid #000000; color: white; border-radius: 50%; width: 85px; height: 85px;} ");
            client.println(".button_orange { background-color: #FFa500; border: 2px solid #000000; color: black; border-radius: 50%; width: 85px; height: 85px;} ");
            client.println(".button_yellow { background-color: #FFFF00; border: 2px solid #000000; color: black; border-radius: 50%; width: 85px; height: 85px;} ");
            client.println(".button_lime { background-color: #00FF00; border: 2px solid #000000; color: black; border-radius: 50%; width: 85px; height: 85px;} ");
            client.println(".button_green { background-color: #008000; border: 2px solid #000000; color: white; border-radius: 50%; width: 85px; height: 85px;} ");
            client.println(".button_cyan { background-color: #00FFFF; border: 2px solid #000000; color: black; border-radius: 50%; width: 85px; height: 85px;} ");
            client.println(".button_blue { background-color: #0000FF; border: 2px solid #000000; color: white; border-radius: 50%; width: 85px; height: 85px;} ");
            client.println(".button_pink { background-color: #FF00FF; border: 2px solid #000000; color: black; border-radius: 50%; width: 85px; height: 85px;} ");
            client.println(".button_purple { background-color: #800080; border: 2px solid #000000; color: white; border-radius: 50%; width: 85px; height: 85px;} ");

            // Additional stuff that's apparently needed to render the website, don't touch
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<style> table.center {margin-left: auto; margin-right: auto;} </style>");
            client.println("<body><h1>Ceramic QI Lamp</h1>");
            client.println("<table class=center><tr>");
            client.println("<td><p><a href=\"/Br.UP\"><button class=\"button_white\">Br. UP </button></a></p></td>");
            client.println("");
            client.println("<td><p><a href=\"/Br.DN\"><button class=\"button_white\">Br. DN </button></a></p></td>");
            client.println("");
            client.println("<td><p><a href=\"/Offswitch/on\"><button class=\"button_black\">OFF</button></a></p></td>");
            client.println("");
            client.println("<td><p><a href=\"/Onswitch/on\"><button class=\"button_red\">ON</button></a></p></td>");
            client.println("</tr>");
            client.println("<tr>");
            client.println("<td><p><a href=\"/Red/on\"><button class=\"button_red\">R</button></a></p></td>");
            client.println("");
            client.println("<td><p><a href=\"/Green/on\"><button class=\"button_green\">G</button></a></p></td>");
            client.println("");
            client.println("<td><p><a href=\"/Blue/on\"><button class=\"button_blue\">B</button></a></p></td>");
            client.println("");
            if (act_modi!=flash) 
            {
              client.println("<td><p><a href=\"/Flash/on\"><button class=\"button_grey\">Flash</button></a></p></td>");
            }
            else
            {
              client.println("<td><p><a href=\"/Flash/off\"><button class=\"button_grey\">Flash</button></a></p></td>");
            }
            client.println("</tr>");
            client.println("<tr>");
            client.println("<td><p><a href=\"/Orange/on\"><button class=\"button_orange\"> </button></a></p></td>");
            client.println("");
            client.println("<td><p><a href=\"/Lime/on\"><button class=\"button_lime\"> </button></a></p></td>");
            client.println("");
            client.println("<td><p><a href=\"/Cyan/on\"><button class=\"button_cyan\"> </button></a></p></td>");
            client.println("");
            if (act_modi!=strobe) 
            {
              client.println("<td><p><a href=\"/Strobe/on\"><button class=\"button_grey\">Strobe</button></a></p></td>");
            }
            else
            {
              client.println("<td><p><a href=\"/Strobe/off\"><button class=\"button_grey\">Strobe</button></a></p></td>");
            }
            client.println("</tr>");
            client.println("<tr>");
            client.println("<td><p><a href=\"/Yellow/on\"><button class=\"button_yellow\"> </button></a></p></td>");
            client.println("");
            client.println("<td><p><a href=\"/Pink/on\"><button class=\"button_pink\"> </button></a></p></td>");
            client.println("");
            client.println("<td><p><a href=\"/Purple/on\"><button class=\"button_purple\"> </button></a></p></td>");
            client.println("");
            if (act_modi!=smooth)
            {
              client.println("<td><p><a href=\"/Smooth/on\"><button class=\"button_grey\">Smooth</button></a></p></td>");
            }
            else
            {
              client.println("<td><p><a href=\"/Smooth/off\"><button class=\"button_grey\">Smooth</button></a></p></td>");
            }
            client.println("");
            client.println("</tr></table>");
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        // if you got anything else but a carriage return character,
        } else if (c != '\r') {  
          // add it to the end of the currentLine
          currentLine += c;      
        }
      }
      //add LED break
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

/************************************************************
                      TOUCH INPUT
************************************************************/
  //read touch input Touch3=GPIO15 for manual on/off
  if (touchRead(T3) < 10) //20
  {
    Serial.print("Touch value:");
    Serial.println(touchRead(T3));
    // invert act_status
    act_status = !act_status;
    // activate or deactivate LED
    LEDStart(act_status, 0, 0, act_modi);
    // wait as debouncing
    delay(500);
  }

/************************************************************
                      FUNCTION MODE SELECT
************************************************************/
  // var for timed interrupt
  static unsigned long delayTime = 0;
  static unsigned long startTime = 0;
  currentTime = millis();
  if(act_modi!=constant&&currentTime>=delayTime+startTime)
  {
    // get selected mode
    switch (act_modi)
    {
      // not needed neglected with if statement
      case constant:
        break;
        
      // flash mode
      case flash:
        // switch between on/off with delay
        if(!act_status)
        {
          LEDStart(true, 0, 0, flash);      
        }
        else
        {
          LEDStart(false, 0, 0, flash); 
        }
        // delay in ms
        delayTime = 500;
        break;
        
      // strobe mode
      case strobe:
        // switch between on/off with fast delay
        if(!act_status)
        {
          LEDStart(true, 0, 0, strobe);      
        }
        else
        {
          LEDStart(false, 0, 0, strobe); 
        }
        // delay in ms
        delayTime = 125;
        break;
        
      // flash mode
      case smooth:
        delayTime = 30;
        // value to add/substract
        static int factor = 5;
        // var to show direction
        static bool BrUp = 0;
        
        // only red values
        if(!act_G&&!act_B&&!BrUp)
        {
          if(0<act_R-factor)
          {
            act_R = act_R - factor;
            for (int numpix = 0; numpix < NUMPIXELS; numpix++)
            {
              strip.setPixelColor(numpix, 0, act_R, 0);
              // update pixel colors to the hardware
              strip.show();
            }
            Serial.println("smooth down");    
          }
          // set red value to lowest value possible
          else
          {
            LEDStart(true, 0x010000, 0, smooth);
            // now smooth up
            BrUp = 1;
          }
        }
        else if(!act_G&&!act_B&&BrUp)
        {
          if(255>act_R+factor)
          {
            act_R = act_R + factor;
            for (int numpix = 0; numpix < NUMPIXELS; numpix++)
            {
              strip.setPixelColor(numpix, 0, act_R, 0);
              // update pixel colors to the hardware
              strip.show(); 
            } 
            Serial.println("smooth up");   
          }
          // set red value to highest value possible
          else
          {
            LEDStart(true, 0xFF0000, 0, smooth);
            // now smooth down
            BrUp = 0;
          }
        }

        // only green values
        else if(!act_R&&!act_B&&!BrUp)
        {
          if(0<act_G-factor)
          {
            act_G = act_G - factor;
            for (int numpix = 0; numpix < NUMPIXELS; numpix++)
            {
              strip.setPixelColor(numpix, act_G, 0, 0);
              // update pixel colors to the hardware
              strip.show();
            }
            Serial.println("smooth down");    
          }
          // set green value to lowest value possible
          else
          {
            LEDStart(true, 0x000100, 0, smooth);
            // now smooth up
            BrUp = 1;
          }
        }
        else if(!act_R&&!act_B&&BrUp)
        {
          if(255>act_G+factor)
          {
            act_G = act_G + factor;
            for (int numpix = 0; numpix < NUMPIXELS; numpix++)
            {
              strip.setPixelColor(numpix, act_G, 0, 0);
              // update pixel colors to the hardware
              strip.show(); 
            } 
            Serial.println("smooth up");   
          }
          // set green value to highest value possible
          else
          {
            LEDStart(true, 0x00FF00, 0, smooth);
            // now smooth down
            BrUp = 0;
          }
        }
        else if(!act_R&&!act_G&&!BrUp)
        {
          if(0<act_B-factor)
          {
            act_B = act_B - factor;
            for (int numpix = 0; numpix < NUMPIXELS; numpix++)
            {
              strip.setPixelColor(numpix, 0, 0, act_B);
              // update pixel colors to the hardware
              strip.show();
            }
            Serial.println("smooth down");    
          }
          // set blue value to highest value possible
          else
          {
            LEDStart(true, 0x000001, 0, smooth);
            // now smooth up
            BrUp = 1;
          }
        }

        // only blue values
        else if(!act_R&&!act_G&&BrUp)
        {
          if(255>act_B+factor)
          {
            act_B = act_B + factor;
            for (int numpix = 0; numpix < NUMPIXELS; numpix++)
            {
              strip.setPixelColor(numpix, 0, 0, act_B);
              // update pixel colors to the hardware
              strip.show(); 
            } 
            Serial.println("smooth up");   
          }
          // set blue value to lowest value possible
          else
          {
            LEDStart(true, 0x0000FF, 0, smooth);
            // now smooth down
            BrUp = 0;
          }
        }

        // other values, not working!
        else
        {
          
        }
        break;
      default:
        break;
    }
    // transfer current time to var for interrupt
    startTime = currentTime;
  }
  
}


/************************************************************
                      LEDSTART FUNCTION
************************************************************/
// definition of LED function
void LEDStart(bool status, long color, int16_t brightness, modi modi) {
  // define variables
  int r = 0; // red
  int g = 0; // green
  int b = 0; // blue
  int numpix = 0; // count RGB-LED

  // init act color for first start
  if (0 == act_R && 0 == act_G && 0 == act_B)
  {
    act_R = 100;
    act_G = 100;
    act_B = 100;
  }
  
  // convert HTML color to RGB
  if (!color)
  {
    r = act_R;
    g = act_G;
    b = act_B;
  }
  else
  {
    // convert to rgb
    r = (color & 0xFF0000) >> 16;
    g = (color & 0x00FF00) >> 8;
    b = (color & 0x0000FF);
  }
  // save as actual color
  act_R = r;
  act_G = g;
  act_B = b;
  // serial info of color
  Serial.println();
  Serial.println("set color");
  Serial.print("r := ");
  Serial.println(r);
  Serial.print("g := ");
  Serial.println(g);
  Serial.print("b := ");
  Serial.println(b);
  Serial.println();

  // check brightness
  if (brightness > 0)
  {
    if (r + brightness < 255 & g + brightness < 255 & b + brightness < 255)
    {
      r = int(int16_t(r) + brightness);
      g = int(int16_t(g) + brightness);
      b = int(int16_t(b) + brightness);
      Serial.println();
      Serial.println("brightness up");
      Serial.println(brightness);
      Serial.print("r :=");
      Serial.println(r);
      Serial.print("g :=");
      Serial.println(g);
      Serial.print("b :=");
      Serial.println(b);
      Serial.println();
      // save the new color
      act_R = r;
      act_G = g;
      act_B = b;
    }
    else
    {
      Serial.println("error brightness up");
    }
  }
  else if (brightness < 0)
  {
    if (r - brightness > 0 & r + brightness > 0 & b + brightness > 0)
    {
      r = int(r + brightness);
      g = int(g + brightness);
      b = int(b + brightness);
      Serial.println();
      Serial.println("brightness down");
      Serial.println(int(brightness));
      Serial.print("r :=");
      Serial.println(r);
      Serial.print("g :=");
      Serial.println(g);
      Serial.print("b :=");
      Serial.println(b);
      Serial.println();
      // save the new color
      act_R = r;
      act_G = g;
      act_B = b;
    }
    else
    {
      Serial.println("error brightness down");
    }
  }

  // check if on/off and set color
  if (status)
  {
    Serial.println();
    Serial.println("LED Status:ON");  // serial message
    Serial.println();

    for (numpix = 0; numpix < NUMPIXELS; numpix++)
    {
      strip.setPixelColor(numpix, act_G, act_R, act_B);
      strip.show();    // update pixel colors to the hardware
    }
  }
  else
  {
    Serial.println();
    Serial.println("LED Status:OFF");  // serial message
    Serial.println();

    for (numpix = 0; numpix < NUMPIXELS; numpix++) {
      strip.setPixelColor(numpix, 0, 0, 0);
      strip.show();    // update pixel colors to the hardware
    }
  }
  // save actual status in global variable
  act_status = status;

  // save actual modi in global variable
  Serial.print("modi := ");
  Serial.println(modi);
  act_modi=modi;
  
}
