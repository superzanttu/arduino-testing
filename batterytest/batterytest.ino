// Library
// Requires additional boards: http://arduino.esp8266.com/stable/package_esp8266com_index.json
#include <ESP8266WiFi.h>
#include <EEPROM.h>

// WiFi settings
const char* ssid = "xxx";
const char* password = "xxx";

// Time to sleep (in seconds):
const int sleepTimeS = 60;

// Host
const char* host = "dweet.io";

int c0, c1, c2;
const int resetButton = 4;

void setup()
{
  // Serial
  Serial.begin(115200);
  Serial.println("ESP8266 booted");

  pinMode(resetButton, INPUT);

  EEPROM.begin(512);
  c0 = EEPROM.read(0);
  c1 = EEPROM.read(1);
  c2 = EEPROM.read(2);

  if (digitalRead(resetButton) == HIGH) {
    Serial.println("Counter reset requested");
    c0 = 0;
    c1 = 0;
    c2 = 0;
  }


  if (c0 < 255) {
    c0 = c0 + 1;
  }
  else
  {
    c0 = 0;
    if (c1 < 255) {
      c1 = c1 + 1;
    }
    else
    {
      c1 = 0;
      if (c2 < 255) {
        c2 = c2 + 1;
      }
    }
  }

  EEPROM.write(0, c0);
  EEPROM.write(1, c1);
  EEPROM.write(2, c2);
  EEPROM.commit();



  Serial.print("Boot counters:");
  Serial.print(c2);
  Serial.print(" ");
  Serial.print(c1);
  Serial.print(" ");
  Serial.println(c0);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.println(WiFi.localIP());

  // Logging data to cloud
  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // This will send the request to the server
  client.print(String("GET /dweet/for/koivikkoposti?c0=") + String(c0) + "&c1=" + String(c1) + "&c2=" + String(c2) + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(10);

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");

  // Sleep
  Serial.println("ESP8266 going to deep deep sleep mode");
  ESP.deepSleep(sleepTimeS * 1000000);


}

void loop()
{

}
