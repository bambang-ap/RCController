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
	client.println("<!DOCTYPE html>");
	client.println("<html lang=\"en\">");
	client.println("");
	client.println("	<head>");
	client.println("		<meta charset=\"UTF-8\">");
	client.println("		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
	client.println("		<title>RC Controller</title>");
	client.println("		<style>");
	client.println("			:root {");
	client.println("				--gap: 64px;");
	client.println("				--button-size: 100px;");
	client.println("				--text-size: 32px;");
	client.println("				--radius: 24px;");
	client.println("			}");
	client.println("");
	client.println("			* {");
	client.println("				user-select: none;");
	client.println("			}");
	client.println("");
	client.println("			html,");
	client.println("			body {");
	client.println("				margin: 0;");
	client.println("				width: 100%;");
	client.println("				height: 100%;");
	client.println("			}");
	client.println("");
	client.println("			body {");
	client.println("				padding-bottom: 32px;");
	client.println("				display: flex;");
	client.println("				gap: var(--gap);");
	client.println("			}");
	client.println("");
	client.println("			div {");
	client.println("				flex: 1;");
	client.println("				display: flex;");
	client.println("				gap: var(--gap);");
	client.println("				justify-content: center;");
	client.println("				align-items: flex-end;");
	client.println("			}");
	client.println("");
	client.println("			button {");
	client.println("				width: var(--button-size);");
	client.println("				height: var(--button-size);");
	client.println("				font-size: var(--text-size);");
	client.println("				border-radius: var(--radius);");
	client.println("			}");
	client.println("");
	client.println("			button:active {");
	client.println("				transform: scale(0.9);");
	client.println("				box-shadow: 5px 3px 33px 2px rgba(0, 0, 0, 0.24);");
	client.println("			}");
	client.println("");
	client.println("			#left {");
	client.println("				/* flex-direction: column; */");
	client.println("				");
	client.println("			}");
	client.println("");
	client.println("			#right {");
	client.println("			}");
	client.println("");
	client.println("			#space {");
	client.println("				width: 24px;");
	client.println("				flex: none;");
	client.println("			}");
	client.println("		</style>");
	client.println("	</head>");
	client.println("");
	client.println("	<body>");
	client.println("");
	client.println("		<div id=\"left\">");
	client.println("			<button value-in=\"A=ON\" value-out=\"A=OFF\">←</button>");
	client.println("			<button value-in=\"D=ON\" value-out=\"D=OFF\">→</button>");
	client.println("		</div>");
	client.println("");
	client.println("		<div id=\"right\">");
	client.println("			<button value-in=\"W=ON\" value-out=\"W=OFF\">↑</button>");
	client.println("			<button value-in=\"S=ON\" value-out=\"S=OFF\">↓</button>");
	client.println("		</div>");
	client.println("");
	client.println("		<script>");
	client.println("			let allValueOut = [], interval = [];");
	client.println("");
	client.println("			const isMobile = navigator.userAgent.match(/iPhone/i) || navigator.userAgent.match(/iPad/i) || navigator.userAgent.match(/Android/i);");
	client.println("");
	client.println("			const ip = !!location.port ? `http://192.168.1.10` : location.origin;");
	client.println("			const buttons = document.getElementsByTagName('button');");
	client.println("");
	client.println("			for (let i = 0; i < buttons.length; i++) {");
	client.println("				const btn = buttons.item(i);");
	client.println("				const valueIn = btn.getAttribute('value-in');");
	client.println("				const valueOut = btn.getAttribute('value-out');");
	client.println("");
	client.println("				allValueOut[i] = valueOut;");
	client.println("");
	client.println("				if (isMobile) {");
	client.println("					btn.ontouchstart = eventIn;");
	client.println("					btn.ontouchend = eventOut;");
	client.println("				} else {");
	client.println("					btn.onmousedown = eventIn;");
	client.println("					btn.onmouseup = eventOut;");
	client.println("				}");
	client.println("");
	client.println("				function eventIn() {");
	client.println("					fetchIn();");
	client.println("					interval[i] = setInterval(fetchIn, 500);");
	client.println("				};");
	client.println("				function eventOut() {");
	client.println("					allValueOut.map(valOut => fetch(`${ip}/${valOut}`));");
	client.println("					for (let o = 0; o < interval.length; o++) {");
	client.println("						clearInterval(interval[o]);");
	client.println("					}");
	client.println("				};");
	client.println("");
	client.println("				function fetchIn() { fetch(`${ip}/${valueIn}`); };");
	client.println("				function fetchOut() { fetch(`${ip}/${valueOut}`); };");
	client.println("			}			");
	client.println("		</script>");
	client.println("	</body>");
	client.println("");
	client.println("</html");
  // End html

  delay(100);
  Serial.println("Client disconnected");
  Serial.println("");
}