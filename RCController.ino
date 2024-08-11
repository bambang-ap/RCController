#include <ESP8266WiFi.h>

const char *ssid = "None";
const char *password = "ttttgggg";

int forwardPin = D1;
int backwardPin = D2;
int leftPin = D7;
int rightPin = D8;

int len = 4;
int outputPins[] = {forwardPin, backwardPin, leftPin, rightPin};
String charsPins[] = {"W", "S", "A", "D"};

WiFiServer server(80);

void setup()
{
  Serial.begin(9600);
  delay(10);

  for (byte i = 0; i < len; i = i + 1)
  {
    pinMode(outputPins[i], OUTPUT);
    digitalWrite(outputPins[i], LOW);
  }

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available())
  {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  for (byte i = 0; i < len; i = i + 1)
  {

    if (request.indexOf("/" + charsPins[i] + "=ON") != -1)
    {
      digitalWrite(outputPins[i], HIGH);
    }
    if (request.indexOf("/" + charsPins[i] + "=OFF") != -1)
    {
      digitalWrite(outputPins[i], LOW);
    }
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  // Start html
  sdhjgshjdgshgd
  // End html

  delay(100);
  Serial.println("Client disconnected");
  Serial.println("");
}