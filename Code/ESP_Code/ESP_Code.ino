#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height, in pixels - DO NOT CHANGE
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaration for SSD1306 display connected using software SPI - DO NOT CHANGE
#define OLED_MOSI   23
#define OLED_CLK   18
#define OLED_DC    4
#define OLED_CS    5
#define OLED_RESET 0

// The pins the servo motors are connected to - DO NOT CHANGE
#define SERVO1 1
#define SERVO2 2

// The pins the motor controller is connected to - DO NOT CHANGE
#define PHA_A 25  // Left motor
#define EN_A 26

#define PHA_B 27  // Right motor
#define EN_B 14

#define STANDBY 12

// Motor channels
#define MOTOR_L 0
#define MOTOR_R 0

// PWM Settings - DO NOT CHANGE
#define PWM_FREQUENCY 500
#define PWM_RESOLUTION 8
#define PWM_MAX_DC 255

// Change ssid and password to the ones of the network you want to connect to
char* ssid = "*****";
char* password = "*****";
char* mqtt_server = "mqtt.luytsm.be";

// Variables to save the data recieved from node-red dashboard
String mode = "";
String driveDirection = "";
byte driveSpeed = 0;

// Objects for Wifi, MQTT, servos and OLED
WiFiClient espClient;
PubSubClient client(espClient);
Servo barrier1;
Servo barrier2;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  barrier1.attach(SERVO1);
  barrier2.attach(SERVO2);

  ledcSetup(MOTOR_L, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcSetup(MOTOR_R, PWM_FREQUENCY, PWM_RESOLUTION);

  ledcAttachPin(EN_A, MOTOR_L);
  ledcAttachPin(EN_B, MOTOR_R);

  Wire.begin();
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String data;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
  Serial.println();

  if(String(topic) == "/jelte/mode")
    mode = data;
    
  else if (String(topic) == "/jelte/standby") {
    if(data == "1")
      digitalWrite(STANDBY, LOW);
    else
      digitalWrite(STANDBY, HIGH);  
  }
      
  else if (String(topic) == "/jelte/barriers") {
    if(data == "omhoog1")
      barrier1.write(90);
    else if (data == "omlaag1")
      barrier1.write(0);

    if(data == "omhoog2")
      barrier2.write(90);
    else if (data == "omlaag2")
      barrier2.write(0);
  }
  
  else if (String(topic) == "/jelte/speed")
    driveSpeed = data.toInt();
    
  else if (String(topic) == "/jelte/dir")
    driveDirection = data;
    
  else {
    Serial.println("Unknown topic...");
    return;
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe("/jelte/mode");
      client.subscribe("/jelte/standby");
      client.subscribe("/jelte/barriers");
      client.subscribe("/jelte/dir");
      client.subscribe("/jelte/speed");
      Serial.println("Subscribed");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 
  if(mode == "manueel")
    manualControl();
    
  else if (mode == "automatisch")
    automaticControl();
}

// Code for when you want to control the car manually
void manualControl() {
  driveSpeed = map(driveSpeed, 0, 100, 0, 255); // Change te recieved percent value to byte
  
  if(driveDirection == "stop") {
    // Stop both motors
    ledcWrite(MOTOR_L, 0);
    ledcWrite(MOTOR_R, 0);
  }

  else if(driveDirection == "vooruit") {
    // Both motors spin forwards
    digitalWrite(PHA_A, HIGH);
    digitalWrite(PHA_B, HIGH);

    // Turn motors at recieved speeds
    ledcWrite(MOTOR_L, driveSpeed);
    ledcWrite(MOTOR_R, driveSpeed);
  }

  else if(driveDirection == "links") {
    // Both motors spin forwards
    digitalWrite(PHA_A, HIGH);
    digitalWrite(PHA_B, HIGH);

    // Turn right motor at recieved speed
    ledcWrite(MOTOR_L, 0);
    ledcWrite(MOTOR_R, driveSpeed);
  }

  else if(driveDirection == "rechts") {
    // Both motors spin forwards
    digitalWrite(PHA_A, HIGH);
    digitalWrite(PHA_B, HIGH);

    // Turn left motor at recieved speed
    ledcWrite(MOTOR_L, driveSpeed);
    ledcWrite(MOTOR_R, 0);
  }

  else if(driveDirection == "achteruit") {
    // Both motors spin backwards
    digitalWrite(PHA_A, LOW);
    digitalWrite(PHA_B, LOW);

    // Turn motors at recieved speeds
    ledcWrite(MOTOR_L, driveSpeed);
    ledcWrite(MOTOR_R, driveSpeed);
  }

  else if(driveDirection == "links-vooruit") {
    // Both motors spin forwards
    digitalWrite(PHA_A, HIGH);
    digitalWrite(PHA_B, HIGH);

    // Turn right motor at recieved speed and left motor at half
    ledcWrite(MOTOR_L, driveSpeed/2);
    ledcWrite(MOTOR_R, driveSpeed);
  }

  else if(driveDirection == "rechts-vooruit") {
    // Both motors spin forwards
    digitalWrite(PHA_A, HIGH);
    digitalWrite(PHA_B, HIGH);

    // Turn left motor at recieved speed and right motor at half
    ledcWrite(MOTOR_L, driveSpeed);
    ledcWrite(MOTOR_R, driveSpeed/2);
  }

  else if(driveDirection == "links-achter") {
    // Both motors spin backwards
    digitalWrite(PHA_A, LOW);
    digitalWrite(PHA_B, LOW);

    // Turn right motor at recieved speed and left motor at half
    ledcWrite(MOTOR_L, driveSpeed/2);
    ledcWrite(MOTOR_R, driveSpeed);
  }

  else if(driveDirection == "rechts-achter") {
    // Both motors spin backwards
    digitalWrite(PHA_A, LOW);
    digitalWrite(PHA_B, LOW);

    // Turn left motor at recieved speed and right motor at half
    ledcWrite(MOTOR_L, driveSpeed);
    ledcWrite(MOTOR_R, driveSpeed/2);
  }
}

// Code for when you want to control the car automatically
void automaticControl() {
  Wire.requestFrom(8, 3); // 3 bytes from device 8

  while(Wire.available()) {
    char sensorData = Wire.read();
    Serial.print(sensorData);
  }
}
