void handle_NotFound() { server.send(404, "text/plain", "Not found"); }

void handle_OnConnect() {
  for (byte i = 0; i < len; i = i + 1) {
    pinStatus[i] = false;
  }
  server.send(200, "text/html", SendHTML());
}

void serverHandler() {
  for (byte i = 0; i < len + 1; i = i + 1) {
    String txt = charsPins[i];
    String txtStatus = server.arg(txt);

    int index = indexOf(charsPins, txt);

    if (txtStatus == "ON") {
      pinStatus[index] = true;
    }

    if (txtStatus == "OFF") {
      pinStatus[index] = false;
    }

    server.send(200, "text/html", SendHTML());
  }
}