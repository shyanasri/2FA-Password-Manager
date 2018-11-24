#include <ESP8266WiFi.h>
#include <SD.h>
#include <SPI.h>
#include <RFID.h>

const int cs = 6;

RFID rfid(D8, D2);

/*
  This integer should be the code of Your Mifare card / tag
*/

int cards[][5] = {
  {61, 84, 23, 194, 188},
  {186, 111, 112, 41, 140},
  {136, 4, 67, 146, 93}
};

bool access = false;
bool gotaccess = false;
//WIFI access
const char* ssid     = "SM-G950W3483";
const char* password = "12345678";


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
void setup() {
  
Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  
  Serial.print("Initializing card...\n");
  SPI.begin();
  rfid.init();

  /*
    define VMA100 (UNO) pins 7 & 8 as outputs and put them LOW

      pinMode(led, OUTPUT);
      pinMode (power,OUTPUT);
      digitalWrite(led, LOW);
      digitalWrite (power,LOW);
  */

}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
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

     
  if (rfid.isCard()) {

    if (rfid.readCardSerial()) {
      Serial.print(rfid.serNum[0]);
      Serial.print(" ");
      Serial.print(rfid.serNum[1]);
      Serial.print(" ");
      Serial.print(rfid.serNum[2]);
      Serial.print(" ");
      Serial.print(rfid.serNum[3]);
      Serial.print(" ");
      Serial.print(rfid.serNum[4]);
      Serial.println("");

      for (int x = 0; x < sizeof(cards); x++) {
        for (int i = 0; i < sizeof(rfid.serNum); i++ ) {
          if (rfid.serNum[i] != cards[x][i]) {
            access = false;
            break;
          } else {
            access = true;
          }
        }
        if (access) break;
      }
    }

    if (access) {
      Serial.println("Welcome Mr. Bury");
      /*
        Valid card : Switch ON the LED for 1000 ms (1 second)
      */
      delay(500);
      
    } else {
      /*
        NON-Valid card : switch ON and OFF the LED twice for 0,5 seconds
      */
      Serial.println("Not allowed!");
      delay(500);
    }
  }
  rfid.halt();
   
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            if (access) {
              client.println("<p> Welcome Mr. Bury</p>");
              access = false;
            }
            else {
                client.println("<p>Not Allowed!</p>");
              }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

