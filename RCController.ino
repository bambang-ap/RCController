#include <ESP8266WiFi.h>

const bool debug = false;

const char *ssidDebug = "None";
const char *passwordDebug = "ttttgggg";

const char *ssid = debug ? ssidDebug : "RC_CONTROLLER";
const char *password = debug ? passwordDebug : "113247CA";


int forwardPin = D1;
int backwardPin = D2;
int leftPin = D7;
int rightPin = D8;
int debugPin = LED_BUILTIN;

int len = 4;
int outputPins[] = {forwardPin, backwardPin, leftPin, rightPin, debugPin};
int OnStatus = LOW;
int OffStatus = HIGH;
String charsPins[] = {"W", "S", "A", "D"};

WiFiServer server(80);

void setup()
{
  Serial.begin(9600);
  delay(10);

  for (byte i = 0; i < len + 1; i = i + 1)
  {
    pinMode(outputPins[i], OUTPUT);
    digitalWrite(outputPins[i], OffStatus);
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
      digitalWrite(outputPins[i], OnStatus);
			if (debug) { digitalWrite(debugPin, OnStatus); }
    }
    if (request.indexOf("/" + charsPins[i] + "=OFF") != -1)
    {
      digitalWrite(outputPins[i], OffStatus);
			if (debug) { digitalWrite(debugPin, OffStatus); }
    }
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  // Start html
	client.println("<!doctype html><html lang=en><head><meta charset=UTF-8><meta name=viewport content=\"width=device-width,initial-scale=1\"><title>RC Controller</title><style>:root{--gap:64px;--button-size:100px;--text-size:32px;--radius:24px}*{user-select:none}body,html{width:100%;height:100%}body{display:flex;gap:var(--gap)}div{flex:1;display:flex;gap:var(--gap);justify-content:center;align-items:center}button{width:var(--button-size);height:var(--button-size);font-size:var(--text-size);border-radius:var(--radius)}button:active{transform:scale(.9);box-shadow:5px 3px 33px 2px rgba(0,0,0,.24)}#left{flex-direction:column}#right{gap:128px}#space{width:24px;flex:none}</style></head><body><div id=left><button value-in=\"W=ON\" value-out=\"W=OFF\">↑</button><button value-in=\"S=ON\" value-out=\"S=OFF\">↓</button></div><div id=right><button value-in=\"A=ON\" value-out=\"A=OFF\">←</button><button value-in=\"D=ON\" value-out=\"D=OFF\">→</button></div><div id=space></div><script>let allValueOut=[],interval=[];const isMobile=navigator.userAgent.match(/iPhone/i)||navigator.userAgent.match(/iPad/i)||navigator.userAgent.match(/Android/i),ip=location.port?\"http://192.168.1.10\":location.origin,buttons=document.getElementsByTagName(\"button\");for(let t=0;t<buttons.length;t++){const e=buttons.item(t),n=e.getAttribute(\"value-in\"),o=e.getAttribute(\"value-out\");function eventIn(){fetchIn(),interval[t]=setInterval(fetchIn,500)}function eventOut(){allValueOut.map((t=>fetch(`${ip}/${t}`)));for(let t=0;t<interval.length;t++)clearInterval(interval[t])}function fetchIn(){fetch(`${ip}/${n}`)}function fetchOut(){fetch(`${ip}/${o}`)}allValueOut[t]=o,isMobile?(e.ontouchstart=eventIn,e.ontouchend=eventOut):(e.onmousedown=eventIn,e.onmouseup=eventOut)}</script></body></html");
  // End html

  delay(100);
  Serial.println("Client disconnected");
  Serial.println("");
}