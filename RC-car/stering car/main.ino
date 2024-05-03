#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "index.h"

const char* ssid = "~virus~~~";
const char* password = "bablu100";

ESP8266WebServer server(80);

const char motor1Pin1 = 4;  // D2, GPIO 4
const char motor1Pin2 = 0;  // D3, GPIO 0
const char motor2Pin1 = 13; // D7, GPIO 13
const char motor2Pin2 = 15; // D8, GPIO 15

String lastButtonPressed = ""; // Variable to store the last button pressed
// Function to move the robot forward
void moveForward() {
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

// Function to move the robot backward
void moveBackward() {
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

// Function to turn the robot left
void turnLeft() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
}

// Function to turn the robot right
void turnRight() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
}

void Complete_stop() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

void directional_stop(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
}

void forward_back_stop(){
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);


}

void setup() {
  Serial.begin(9600);
  delay(10);

   // Set motor pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  Complete_stop();

  // Connect to Wi-Fi
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

  // Initialize the server
  server.on("/car", HTTP_GET, handleRoot);
  server.on("/control", HTTP_GET, handleControl);
  server.begin();
  Serial.println("Server started");
  Serial.println("IP address : ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
}


void handleRoot() {
  String html = HTML_CONTENT; 
  server.send(200, "text/html", html);
}

void handleControl() {
  if (server.hasArg("action")) {
    lastButtonPressed = server.arg("action");
    Serial.println("Button  pressed: " + lastButtonPressed);
    // Add your code to handle the button press here
  }

  char command = lastButtonPressed.charAt(0); // Get the first character of the String

  switch(command) {
      case 'f':
        moveForward();
        break;
      case 'b':
        moveBackward();
        break;
      case 'l':
        turnLeft();
        break;
      case 'r':
        turnRight();
        break;
      case 's':
        Complete_stop();
        break;
      case 'd':
        directional_stop();
        break;
      default:
        Serial.println("Invalid command : "+ command);
        break;
    }

  server.sendHeader("Location", "/car");
  server.send(303);
}
