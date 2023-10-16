#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "fs_tools.h"

const String ssid = "MM67987";
const String password = "XXYJXZ316";
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

void web_animPage(void)
{
    File f = SPIFFS.open("/index.html", "r");
    web_server.streamFile(f, "text/html");
}

void web_animPage_css(void)
{
    File f = SPIFFS.open("/style.css", "r");
    web_server.streamFile(f, "text/css");
}

void web_animPage_js(void)
{
    File f = SPIFFS.open("/main.js", "r");
    web_server.streamFile(f, "application/javascript");
}

void web_handleGETTemp(void)
{
    web_server.sendHeader("Content-Type", "text/html; charset=utf-8");
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
    web_server.on("/anim", web_animPage);
    web_server.on("/style.css", web_animPage_css);
    web_server.on("/main.js", web_animPage_js);
    web_server.on("/data/temp", web_handleGETTemp);
    web_server.begin();
    Serial.println("HTTP server started!");
}

void loop() {
  // put your main code here, to run repeatedly:
    web_server.handleClient();
}
