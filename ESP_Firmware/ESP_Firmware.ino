//This code can be expanded with Kris Winer's implementation of sensor fusion
//Specifically the quaternionFilters.ino file which contains the math to
//produce yaw, pitch, and roll
//https://github.com/kriswiner/MPU9250

#include <ESP8266WiFi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include <string.h>
#include <inttypes.h>
#include <Wire.h>
#include "AccelUtil.h"

//Enter your network ssid and password here
const char* ssid = "";
const char* password = "";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Use the IP address which is printed in the PyQTGraph script
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

//Use the IP of the computer running the PlotMPUData.py
//script in the byte_server array
  byte server[] = {192, 168, 0, 23};
  if (client.connect(server, 12345)) {
      Serial.println("Socket connected");
  }

  //Setup the I2C wire on pins 1 and 2
  Wire.begin(1,2);

  //Startup the Accelerometer
  setupMPU();

  //Start the timer
  int time = millis();
}

void loop() {

  //Get Accel Data
  readData();

  //send the 16bit values to server
  client.print(accelDataX);
  client.print(accelDataY);
  client.print(accelDataZ);
  client.print(gyroDataX);
  client.print(gyroDataY);
  client.print(gyroDataZ);

}
