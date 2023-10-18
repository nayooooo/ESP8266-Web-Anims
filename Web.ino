#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "fs_tools.h"

const String ssid = "XXXX";
const String password = "XXXX";
ESP8266WebServer web_server(80);

void web_homePage(void)
{
    web_server.sendHeader("Content-Type", "text/html; charset=utf-8");
    String message;
    fs_tools_readFile("/home.html", message);
    web_server.send(200, "text/html", message.c_str());
    
    Serial.println("用户访问了主页。");
}

void web_echoPage(void)
{
    web_server.sendHeader("Content-Type", "text/html; charset=utf-8");
    String message;
    fs_tools_readFile("/echo.html", message);
    web_server.send(200, "text/html", message.c_str());
}

void web_animPage_weatherwatch(void)
{
    File f = SPIFFS.open("/anim/WeatherWatch/index.html", "r");
    web_server.streamFile(f, "text/html");
}

void web_animPage_weatherwatch_css(void)
{
    File f = SPIFFS.open("/anim/WeatherWatch/style.css", "r");
    web_server.streamFile(f, "text/css");
}

void web_animPage_weatherwatch_js(void)
{
    File f = SPIFFS.open("/anim/WeatherWatch/main.js", "r");
    web_server.streamFile(f, "application/javascript");
}

void web_animPage_flowstair(void)
{
    File f = SPIFFS.open("/anim/FlowStair/index.html", "r");
    web_server.streamFile(f, "text/html");
}

void web_animPage_flowstair_css(void)
{
    File f = SPIFFS.open("/anim/FlowStair/style.css", "r");
    web_server.streamFile(f, "text/css");
}

void web_handleGETTemp(void)
{
    web_server.sendHeader("Content-Type", "text/plain; charset=utf-8");
    web_server.send(200, "text/plain", "20℃");
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200UL);

    if (fs_tools_FS_begin()) {
        Serial.println("SPIFFS start success!");
    } else {
        Serial.println("SPIFFS start failed!");
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    uint8_t err_times = 0;
    while (WiFi.status() != WL_CONNECTED) {
        err_times++;
        if (err_times >= 100) while (true);
        Serial.print(".");
        delay(200);
    }
    Serial.println();
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    web_server.on("/", web_homePage);
    web_server.on("/home", web_homePage);
    web_server.on("/echo", web_echoPage);
    web_server.on("/anim/weatherwatch", web_animPage_weatherwatch);
    web_server.on("/anim/weatherwatch/style.css", web_animPage_weatherwatch_css);
    web_server.on("/anim/weatherwatch/main.js", web_animPage_weatherwatch_js);
    web_server.on("/anim/flowstair", web_animPage_flowstair);
    web_server.on("/anim/flowstair/style.css", web_animPage_flowstair_css);
    web_server.on("/data/temp", web_handleGETTemp);
    web_server.begin();
    Serial.println("HTTP server started!");
}

void loop() {
  // put your main code here, to run repeatedly:
    web_server.handleClient();
}
