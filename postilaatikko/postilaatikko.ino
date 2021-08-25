/*

  Requires additional boards: http://arduino.esp8266.com/stable/package_esp8266com_index.json
*/
#include <ESP8266WiFi.h>
#include <EEPROM.h>

// WiFi settings
const char* ssid = "xxx";
const char* password = "xxx";
// Host
const char* host = "maker.ifttt.com";


// constants won't change. They're used here to set pin numbers:
const int mailInPin = 4;
const int mailOutPin = 5;
const int ledPin =  LED_BUILTIN;      // the number of the LED pin

const int NV_resetcounter = 0; // Reset counter
const int NV_mode = 1; // Mode of operation


const int MODE_wait = 0; // Waiting for reset with pressed button
const int MODE_monitor = 1; // Monitoring



// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int c;




// Send message to dweet
void sendMsg(String msg) {

  digitalWrite(ledPin, LOW);

  Serial.println("Sending message '" + msg + "'");

  // Connect to WIFI
  Serial.println("Connecting WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());

  // Logging data to cloud
  //Serial.print("Connecting to ");
  //Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // This will send the request to the server
  client.print(String("GET /trigger/" + msg + "/with/key/c4BONSEUexOcYnPIiYFprr")  + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);


    // This will send the request to the server
    //client.print(String("GET /dweet/for/koivikkoposti?message=" + msg) + " HTTP/1.1\r\n" +
    //             "Host: " + host + "\r\n" +
    //             "Connection: close\r\n\r\n");

    // Read all the lines of the reply from server and print them to Serial
    //while (client.available()) {
    //  String line = client.readStringUntil('\r');
    //  Serial.print(line);

  }

  // Wait to prevent duplicate messages
  Serial.println("Waiting");
  delay(2000);
  Serial.println("Message sent");

  digitalWrite(ledPin, HIGH);
}


void setup() {

  EEPROM.begin(512);

  // Serial
  Serial.begin(115200);
  Serial.println("\nStarting");

  c = EEPROM.read(0);
  c = c + 1;
  EEPROM.write(0, c);

  Serial.print("Reset counter ");
  Serial.println(c);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(mailInPin, INPUT);
  pinMode(mailOutPin, INPUT);

  digitalWrite(ledPin, HIGH);

  //buttonState = digitalRead(mailInPin);
  // digitalWrite(ledPin, HIGH);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  //if (digitalRead(mailInPin) == HIGH) {
  //  Serial.println("Kansi on auki");
  // turn LED on:
  //  digitalWrite(ledPin, LOW);
  //}

  //if (digitalRead(mailOutPin) == HIGH) {
  //  Serial.println("Luukku on auki");
  //  // turn LED on:
  //  digitalWrite(ledPin, LOW);
  // }

  EEPROM.commit();

}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(mailInPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:

    sendMsg("mail_IN");
  }

  buttonState = digitalRead(mailOutPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    //digitalWrite(ledPin, LOW);
    sendMsg("mail_OUT");
  }


}
