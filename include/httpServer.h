#include "Arduino.h"
#include <WiFi.h>

class HTTPServer
{
public:
  HTTPServer(char *ssid, char *passphrase);
  ~HTTPServer();
  void loop();

private:
  WiFiServer _server;
};

HTTPServer::HTTPServer(char *ssid, char *passphrase)
{
  _server = WiFiServer(80);
  Serial.println("start connecting wifi");
  WiFi.begin(ssid, passphrase);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
  }
  Serial.print("\nWifi Connected.\nIP addr: ");
  Serial.print(WiFi.localIP());
  Serial.println("\nstarting HTTP Server.");
  _server.begin();
}

HTTPServer::~HTTPServer()
{
  _server.stopAll();
  _server.end();
}

const char *httpResponse[] = {
    "HTTP/1.1 200 OK",
    "Content-Type: text/html",
    "Connection: close",
    "Refresh: 10",
    "\n"
    "<!DOCTYPE HTML>",
    "<html>",
    "<body>",
    "  <h1>test page</h1>",
    "</body>",
    "</html>",
    NULL};

void HTTPServer::loop()
{
  WiFiClient client = _server.available();
  if (!client)
    return;
  Serial.println("Web Client connected ");
  String request = client.readStringUntil('\r');
  for (int i = 0; httpResponse[i]; i++)
  {
    client.println(httpResponse[i]);
  }
}