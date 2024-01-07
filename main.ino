/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// Replace with your network credentials
const char *ssid     = "FunBox2-A91D";
const char *password = "opelvectra";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
HTTPClient http;
WiFiClient client;

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

const char* serverName = "http://192.168.1.10/heater/";

bool isHeating = false;

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);

    // Connect to Wi-Fi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

// Initialize a NTPClient to get time
    timeClient.begin();
    // Set offset time in seconds to adjust for your timezone, for example:
    // GMT +1 = 3600
    // GMT +8 = 28800
    // GMT -1 = -3600
    // GMT 0 = 0
    timeClient.setTimeOffset(3600);
}

void loop() {
    timeClient.update();

    int currentHour = timeClient.getHours();
    Serial.print("Hour: ");
    Serial.println(currentHour);

    int currentMinute = timeClient.getMinutes();
    Serial.print("Minutes: ");
    Serial.println(currentMinute);

    int currentSecond = timeClient.getSeconds();
    Serial.print("Seconds: ");
    Serial.println(currentSecond);

    String weekDay = weekDays[timeClient.getDay()];
    Serial.print("Week Day: ");
    Serial.println(weekDay);

    if (weekDay == "Sunday"){ //weekDay == 1 || weekDay == 2
      if (currentHour == 4){
        if (currentMinute == 45){
          if (currentSecond < 5 && isHeating == false){
            sendMessage("heat=ON");
          }
        }
      }

      if (currentHour == 7){
        if (currentMinute == 0){
          if (currentSecond < 5 && isHeating == true){
            sendMessage("heat=OFF");
          }
        }
      }
    }

    Serial.println("");

    delay(2000);
}

void sendMessage(String message) {
  http.begin(client, serverName);
  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");        
  // Send HTTP POST request
  int httpResponseCode = http.PATCH(message);
  Serial.println(httpResponseCode);
}