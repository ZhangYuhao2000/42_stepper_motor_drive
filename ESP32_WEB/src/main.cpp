/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include <Arduino.h>
#include <Wire.h>
#include <string>

#define USE_MULTCORE 0

// Replace with your network credentials
const char *ssid = "SXF";
const char *password = "1234qwert";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String led1State = "off";
String motor1State = "stop";
String position1 = "0";

String led2State = "off";

String led3State = "off";

int step_con = 1;

String valueString_1 = String(5);
int speed_1 = 0;
int speed_mi1_1 = 0;
int speed_mi2_1 = 0; 

String angle1direction = "---";
String angleString_1 = String(0);
int angle_function_1 = 0;
int angle_1 = 0;
int angle_mi1_1 = 0;
int angle_mi2_1 = 0; 

String angle2direction = "---";
String angleString_2 = String(0);
int angle_function_2 = 0;
int angle_2 = 0;
int angle_mi1_2 = 0;
int angle_mi2_2 = 0;

String angle3direction = "---";
String angleString_3 = String(0);
int angle_function_3 = 0;
int angle_3 = 0;
int angle_mi1_3 = 0;
int angle_mi2_3 = 0;

// Assign output variables to GPIO pins
const int outputled1 = 2;

const int output_motor_a1 = 33;
const int output_motor_a2 = 25;
const int output_motor_b1 = 26;
const int output_motor_b2 = 27;

int motor_mode = 0;  // 0 -> stop 1 -> right 2 -> left
int motor_speed = 0; // 0 -> slow 1 -> mid 2 -> fast
int delay_time = 10;

int com_led = 0;

int interruptCounter = 0;
TwoWire I2Cone = TwoWire(0);

int readValue = 0;
byte readArray[2];
int poscon = 0;
double angle_res = 0.08789;
double angle_dee = 0;

int uart1_send = 0;
char uart1_content[] = "#0105000000003!";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 4*3600;
const int   daylightOffset_sec = 4*3600;
int hour = 0;
int minute = 0;
int second = 0;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    // Serial.println("Failed to obtain time");
    return;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  hour = timeinfo.tm_hour;
  minute = timeinfo.tm_min;
  second = timeinfo.tm_sec;
}

void motorSERVE(void *xTask1)
{
  while (1)
  {
    if (motor_mode == 0)
    {
      delay(delay_time); // ms
      delay(delay_time);
      delay(delay_time);
      delay(delay_time);

      delay(delay_time);
      delay(delay_time);
      delay(delay_time);
      delay(delay_time);
    }
    else if (motor_mode == 1)
    {
      // 每100次循环为360°，0.9°步距角，一次循环为4步距
      // for(int i = 0;i<100;i++)
      // {
      if ((step_con == 1)&&(motor_mode == 1))
      {
        // 2
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        delay(delay_time);
        // 3
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        delay(delay_time);
        // 4
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        delay(delay_time);
        // 5
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, LOW);
        delay(delay_time);
        step_con = 2;
      }
      if ((step_con == 2)&&(motor_mode == 1))
      {
        // 6
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        delay(delay_time);
        // 7
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        delay(delay_time);
        // 8
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        delay(delay_time);
        // 1
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, LOW);
        delay(delay_time);
        step_con = 1;
      }
      // }
      // motor_mode = 0;
    }
    else if (motor_mode == 2)
    {
      // for(int i = 0;i<100;i++)
      // {
      if ((step_con == 1)&&(motor_mode == 2))
      {
        // 2
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        delay(delay_time);
        // 3
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        delay(delay_time);
        // 4
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        delay(delay_time);
        // 5
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, LOW);
        delay(delay_time);
        step_con = 2;
      }
      if ((step_con == 2)&&(motor_mode == 2))
      {
        // 6
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        delay(delay_time);
        // 7
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        delay(delay_time);
        // 8
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        delay(delay_time);
        // 1
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, LOW);
        delay(delay_time);
        step_con = 1;
      }
      // }
      // motor_mode = 0;
    }
    // Serial.println(step_con);
  }
}

void webSERVE(void *xTask2)
{
  while (1)
  {
    WiFiClient client = server.available(); // Listen for incoming clients

    if (client)
    { // If a new client connects,
      currentTime = millis();
      previousTime = currentTime;
      Serial.println("New Client."); // print a message out in the serial port
      String currentLine = "";       // make a String to hold incoming data from the client
      while (client.connected() && currentTime - previousTime <= timeoutTime)
      { // loop while the client's connected
        currentTime = millis();
        if (client.available())
        {                         // if there's bytes to read from the client,
          char c = client.read(); // read a byte, then
          Serial.write(c);        // print it out the serial monitor
          header += c;
          if (c == '\n')
          { // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0)
            {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              // turns the GPIOs on and off
              angle_function_1 = 0;
              // motor1
              if (header.indexOf("GET /LED1/on") >= 0)
              {
                led1State = "on";
                digitalWrite(outputled1, HIGH);
              }
              else if (header.indexOf("GET /LED1/off") >= 0)
              {
                led1State = "off";
                digitalWrite(outputled1, LOW);
              }

              if (header.indexOf("GET /MOTOR1/stop") >= 0)
              {
                motor1State = "stop";
                motor_mode = 0;
              }
              else if (header.indexOf("GET /MOTOR1/left") >= 0)
              {
                motor1State = "left";
                motor_mode = 2;
              }
              else if (header.indexOf("GET /MOTOR1/right") >= 0)
              {
                motor1State = "right";
                motor_mode = 1;
              }

              if (header.indexOf("GET /POS1") >= 0)
              {
                poscon = 1;
              }

              if (header.indexOf("GET /?value=") >= 0)
              {
                speed_mi1_1 = header.indexOf('=');
                speed_mi2_1 = header.indexOf('&');
                valueString_1 = header.substring(speed_mi1_1 + 1, speed_mi2_1);
                speed_1 = valueString_1.toInt();
                delay_time = speed_1;
                // Serial.println(valueString_1);
              }

              if (header.indexOf("GET /?angle1left=") >= 0)
              {
                angle_function_1 = 1;
                angle1direction = "left";
                angle_mi1_1 = header.indexOf('=');
                angle_mi2_1 = header.indexOf('&');
                angleString_1 = header.substring(angle_mi1_1 + 1, angle_mi2_1);
                angle_1 = angleString_1.toInt();
                Serial.println(angle1direction);
                Serial.println(angleString_1);
              }
              else if (header.indexOf("GET /?angle1right=") >= 0)
              {
                angle_function_1 = 2;
                angle1direction = "right";
                angle_mi1_1 = header.indexOf('=');
                angle_mi2_1 = header.indexOf('&');
                angleString_1 = header.substring(angle_mi1_1 + 1, angle_mi2_1);
                angle_1 = angleString_1.toInt();
                Serial.println(angle1direction);
                Serial.println(angleString_1);
              }

              if (header.indexOf("GET /A1MOVE") >= 0)
              {
                Serial.println("motor1 move");
              }

              if (header.indexOf("GET /MOTOR1/reset") >= 0)
              {
              }

              // motor2

              if (header.indexOf("GET /LED2/on") >= 0)
              {
                led2State = "on";
                uart1_content[1] = '0';  // on
                uart1_content[13] = '3';
                uart1_send = 1;
              }
              else if (header.indexOf("GET /LED2/off") >= 0)
              {
                led2State = "off";
                uart1_content[1] = '1';  // off
                uart1_content[13] = '3';
                uart1_send = 1;
              }

              if (header.indexOf("GET /?angle2left=") >= 0)
              {
                uart1_content[6] = '1';
                angle2direction = "left";
                angle_mi1_2 = header.indexOf('=');
                angle_mi2_2 = header.indexOf('&');
                angleString_2 = header.substring(angle_mi1_2 + 1, angle_mi2_2);
                angle_2 = angleString_2.toInt();

                int angle2_1 = 0;
                int angle2_2 = 0;
                int angle2_3 = 0;
                int angle2_4 = 0;
                int angle2_5 = 0;

                angle2_1 = angle_2 / 10000;
                angle2_2 = (angle_2 % 10000) / 1000;
                angle2_3 = (angle_2 % 1000) / 100;
                angle2_4 = (angle_2 % 100) / 10;
                angle2_5 = angle_2 % 10;

                if(angle2_1 == 0){uart1_content[7] = '0';}
                else if(angle2_1 == 1){uart1_content[7] = '1';}
                else if(angle2_1 == 2){uart1_content[7] = '2';}
                else if(angle2_1 == 3){uart1_content[7] = '3';}
                else if(angle2_1 == 4){uart1_content[7] = '4';}
                else if(angle2_1 == 5){uart1_content[7] = '5';}
                else if(angle2_1 == 6){uart1_content[7] = '6';}
                else if(angle2_1 == 7){uart1_content[7] = '7';}
                else if(angle2_1 == 8){uart1_content[7] = '8';}
                else if(angle2_1 == 9){uart1_content[7] = '9';}

                if(angle2_2 == 0){uart1_content[8] = '0';}
                else if(angle2_2 == 1){uart1_content[8] = '1';}
                else if(angle2_2 == 2){uart1_content[8] = '2';}
                else if(angle2_2 == 3){uart1_content[8] = '3';}
                else if(angle2_2 == 4){uart1_content[8] = '4';}
                else if(angle2_2 == 5){uart1_content[8] = '5';}
                else if(angle2_2 == 6){uart1_content[8] = '6';}
                else if(angle2_2 == 7){uart1_content[8] = '7';}
                else if(angle2_2 == 8){uart1_content[8] = '8';}
                else if(angle2_2 == 9){uart1_content[8] = '9';}

                if(angle2_3 == 0){uart1_content[9] = '0';}
                else if(angle2_3 == 1){uart1_content[9] = '1';}
                else if(angle2_3 == 2){uart1_content[9] = '2';}
                else if(angle2_3 == 3){uart1_content[9] = '3';}
                else if(angle2_3 == 4){uart1_content[9] = '4';}
                else if(angle2_3 == 5){uart1_content[9] = '5';}
                else if(angle2_3 == 6){uart1_content[9] = '6';}
                else if(angle2_3 == 7){uart1_content[9] = '7';}
                else if(angle2_3 == 8){uart1_content[9] = '8';}
                else if(angle2_3 == 9){uart1_content[9] = '9';}

                if(angle2_4 == 0){uart1_content[10] = '0';}
                else if(angle2_4 == 1){uart1_content[10] = '1';}
                else if(angle2_4 == 2){uart1_content[10] = '2';}
                else if(angle2_4 == 3){uart1_content[10] = '3';}
                else if(angle2_4 == 4){uart1_content[10] = '4';}
                else if(angle2_4 == 5){uart1_content[10] = '5';}
                else if(angle2_4 == 6){uart1_content[10] = '6';}
                else if(angle2_4 == 7){uart1_content[10] = '7';}
                else if(angle2_4 == 8){uart1_content[10] = '8';}
                else if(angle2_4 == 9){uart1_content[10] = '9';}

                if(angle2_5 == 0){uart1_content[11] = '0';}
                else if(angle2_5 == 1){uart1_content[11] = '1';}
                else if(angle2_5 == 2){uart1_content[11] = '2';}
                else if(angle2_5 == 3){uart1_content[11] = '3';}
                else if(angle2_5 == 4){uart1_content[11] = '4';}
                else if(angle2_5 == 5){uart1_content[11] = '5';}
                else if(angle2_5 == 6){uart1_content[11] = '6';}
                else if(angle2_5 == 7){uart1_content[11] = '7';}
                else if(angle2_5 == 8){uart1_content[11] = '8';}
                else if(angle2_5 == 9){uart1_content[11] = '9';}

                Serial.println(angle2direction);
                Serial.println(angleString_2);
              }
              else if (header.indexOf("GET /?angle2right=") >= 0)
              {
                uart1_content[6] = '2';
                angle2direction = "right";
                angle_mi1_2 = header.indexOf('=');
                angle_mi2_2 = header.indexOf('&');
                angleString_2 = header.substring(angle_mi1_2 + 1, angle_mi2_2);
                angle_2 = angleString_2.toInt();
                
                int angle2_1 = 0;
                int angle2_2 = 0;
                int angle2_3 = 0;
                int angle2_4 = 0;
                int angle2_5 = 0;

                angle2_1 = angle_2 / 10000;
                angle2_2 = (angle_2 % 10000) / 1000;
                angle2_3 = (angle_2 % 1000) / 100;
                angle2_4 = (angle_2 % 100) / 10;
                angle2_5 = angle_2 % 10;

                if(angle2_1 == 0){uart1_content[7] = '0';}
                else if(angle2_1 == 1){uart1_content[7] = '1';}
                else if(angle2_1 == 2){uart1_content[7] = '2';}
                else if(angle2_1 == 3){uart1_content[7] = '3';}
                else if(angle2_1 == 4){uart1_content[7] = '4';}
                else if(angle2_1 == 5){uart1_content[7] = '5';}
                else if(angle2_1 == 6){uart1_content[7] = '6';}
                else if(angle2_1 == 7){uart1_content[7] = '7';}
                else if(angle2_1 == 8){uart1_content[7] = '8';}
                else if(angle2_1 == 9){uart1_content[7] = '9';}

                if(angle2_2 == 0){uart1_content[8] = '0';}
                else if(angle2_2 == 1){uart1_content[8] = '1';}
                else if(angle2_2 == 2){uart1_content[8] = '2';}
                else if(angle2_2 == 3){uart1_content[8] = '3';}
                else if(angle2_2 == 4){uart1_content[8] = '4';}
                else if(angle2_2 == 5){uart1_content[8] = '5';}
                else if(angle2_2 == 6){uart1_content[8] = '6';}
                else if(angle2_2 == 7){uart1_content[8] = '7';}
                else if(angle2_2 == 8){uart1_content[8] = '8';}
                else if(angle2_2 == 9){uart1_content[8] = '9';}

                if(angle2_3 == 0){uart1_content[9] = '0';}
                else if(angle2_3 == 1){uart1_content[9] = '1';}
                else if(angle2_3 == 2){uart1_content[9] = '2';}
                else if(angle2_3 == 3){uart1_content[9] = '3';}
                else if(angle2_3 == 4){uart1_content[9] = '4';}
                else if(angle2_3 == 5){uart1_content[9] = '5';}
                else if(angle2_3 == 6){uart1_content[9] = '6';}
                else if(angle2_3 == 7){uart1_content[9] = '7';}
                else if(angle2_3 == 8){uart1_content[9] = '8';}
                else if(angle2_3 == 9){uart1_content[9] = '9';}

                if(angle2_4 == 0){uart1_content[10] = '0';}
                else if(angle2_4 == 1){uart1_content[10] = '1';}
                else if(angle2_4 == 2){uart1_content[10] = '2';}
                else if(angle2_4 == 3){uart1_content[10] = '3';}
                else if(angle2_4 == 4){uart1_content[10] = '4';}
                else if(angle2_4 == 5){uart1_content[10] = '5';}
                else if(angle2_4 == 6){uart1_content[10] = '6';}
                else if(angle2_4 == 7){uart1_content[10] = '7';}
                else if(angle2_4 == 8){uart1_content[10] = '8';}
                else if(angle2_4 == 9){uart1_content[10] = '9';}

                if(angle2_5 == 0){uart1_content[11] = '0';}
                else if(angle2_5 == 1){uart1_content[11] = '1';}
                else if(angle2_5 == 2){uart1_content[11] = '2';}
                else if(angle2_5 == 3){uart1_content[11] = '3';}
                else if(angle2_5 == 4){uart1_content[11] = '4';}
                else if(angle2_5 == 5){uart1_content[11] = '5';}
                else if(angle2_5 == 6){uart1_content[11] = '6';}
                else if(angle2_5 == 7){uart1_content[11] = '7';}
                else if(angle2_5 == 8){uart1_content[11] = '8';}
                else if(angle2_5 == 9){uart1_content[11] = '9';}

                Serial.println(angle2direction);
                Serial.println(angleString_2);
              }

              if (header.indexOf("GET /A2MOVE") >= 0)
              {
                Serial.println("motor2 move");
                uart1_content[13] = '3';
                uart1_send = 1;
              }

              if (header.indexOf("GET /MOTOR2/reset") >= 0)
              {
                uart1_content[12] = '1';
                uart1_content[13] = '3';
                uart1_send = 1;
              }

              // motor3

              if (header.indexOf("GET /LED3/on") >= 0)
              {
                led3State = "on";
                uart1_content[1] = '0';  // on
                uart1_content[13] = '4';
                uart1_send = 1;
              }
              else if (header.indexOf("GET /LED3/off") >= 0)
              {
                led3State = "off";
                uart1_content[1] = '1';  // off
                uart1_content[13] = '4';
                uart1_send = 1;
              }

              if (header.indexOf("GET /?angle3left=") >= 0)
              {
                uart1_content[6] = '1';
                angle3direction = "left";
                angle_mi1_3 = header.indexOf('=');
                angle_mi2_3 = header.indexOf('&');
                angleString_3 = header.substring(angle_mi1_3 + 1, angle_mi2_3);
                angle_3 = angleString_3.toInt();

                int angle2_1 = 0;
                int angle2_2 = 0;
                int angle2_3 = 0;
                int angle2_4 = 0;
                int angle2_5 = 0;

                angle2_1 = angle_3 / 10000;
                angle2_2 = (angle_3 % 10000) / 1000;
                angle2_3 = (angle_3 % 1000) / 100;
                angle2_4 = (angle_3 % 100) / 10;
                angle2_5 = angle_3 % 10;

                if(angle2_1 == 0){uart1_content[7] = '0';}
                else if(angle2_1 == 1){uart1_content[7] = '1';}
                else if(angle2_1 == 2){uart1_content[7] = '2';}
                else if(angle2_1 == 3){uart1_content[7] = '3';}
                else if(angle2_1 == 4){uart1_content[7] = '4';}
                else if(angle2_1 == 5){uart1_content[7] = '5';}
                else if(angle2_1 == 6){uart1_content[7] = '6';}
                else if(angle2_1 == 7){uart1_content[7] = '7';}
                else if(angle2_1 == 8){uart1_content[7] = '8';}
                else if(angle2_1 == 9){uart1_content[7] = '9';}

                if(angle2_2 == 0){uart1_content[8] = '0';}
                else if(angle2_2 == 1){uart1_content[8] = '1';}
                else if(angle2_2 == 2){uart1_content[8] = '2';}
                else if(angle2_2 == 3){uart1_content[8] = '3';}
                else if(angle2_2 == 4){uart1_content[8] = '4';}
                else if(angle2_2 == 5){uart1_content[8] = '5';}
                else if(angle2_2 == 6){uart1_content[8] = '6';}
                else if(angle2_2 == 7){uart1_content[8] = '7';}
                else if(angle2_2 == 8){uart1_content[8] = '8';}
                else if(angle2_2 == 9){uart1_content[8] = '9';}

                if(angle2_3 == 0){uart1_content[9] = '0';}
                else if(angle2_3 == 1){uart1_content[9] = '1';}
                else if(angle2_3 == 2){uart1_content[9] = '2';}
                else if(angle2_3 == 3){uart1_content[9] = '3';}
                else if(angle2_3 == 4){uart1_content[9] = '4';}
                else if(angle2_3 == 5){uart1_content[9] = '5';}
                else if(angle2_3 == 6){uart1_content[9] = '6';}
                else if(angle2_3 == 7){uart1_content[9] = '7';}
                else if(angle2_3 == 8){uart1_content[9] = '8';}
                else if(angle2_3 == 9){uart1_content[9] = '9';}

                if(angle2_4 == 0){uart1_content[10] = '0';}
                else if(angle2_4 == 1){uart1_content[10] = '1';}
                else if(angle2_4 == 2){uart1_content[10] = '2';}
                else if(angle2_4 == 3){uart1_content[10] = '3';}
                else if(angle2_4 == 4){uart1_content[10] = '4';}
                else if(angle2_4 == 5){uart1_content[10] = '5';}
                else if(angle2_4 == 6){uart1_content[10] = '6';}
                else if(angle2_4 == 7){uart1_content[10] = '7';}
                else if(angle2_4 == 8){uart1_content[10] = '8';}
                else if(angle2_4 == 9){uart1_content[10] = '9';}

                if(angle2_5 == 0){uart1_content[11] = '0';}
                else if(angle2_5 == 1){uart1_content[11] = '1';}
                else if(angle2_5 == 2){uart1_content[11] = '2';}
                else if(angle2_5 == 3){uart1_content[11] = '3';}
                else if(angle2_5 == 4){uart1_content[11] = '4';}
                else if(angle2_5 == 5){uart1_content[11] = '5';}
                else if(angle2_5 == 6){uart1_content[11] = '6';}
                else if(angle2_5 == 7){uart1_content[11] = '7';}
                else if(angle2_5 == 8){uart1_content[11] = '8';}
                else if(angle2_5 == 9){uart1_content[11] = '9';}

                Serial.println(angle3direction);
                Serial.println(angleString_3);
              }
              else if (header.indexOf("GET /?angle3right=") >= 0)
              {
                uart1_content[6] = '2';
                angle3direction = "right";
                angle_mi1_3 = header.indexOf('=');
                angle_mi2_3 = header.indexOf('&');
                angleString_3 = header.substring(angle_mi1_3 + 1, angle_mi2_3);
                angle_3 = angleString_3.toInt();
                
                int angle2_1 = 0;
                int angle2_2 = 0;
                int angle2_3 = 0;
                int angle2_4 = 0;
                int angle2_5 = 0;

                angle2_1 = angle_3 / 10000;
                angle2_2 = (angle_3 % 10000) / 1000;
                angle2_3 = (angle_3 % 1000) / 100;
                angle2_4 = (angle_3 % 100) / 10;
                angle2_5 = angle_3 % 10;

                if(angle2_1 == 0){uart1_content[7] = '0';}
                else if(angle2_1 == 1){uart1_content[7] = '1';}
                else if(angle2_1 == 2){uart1_content[7] = '2';}
                else if(angle2_1 == 3){uart1_content[7] = '3';}
                else if(angle2_1 == 4){uart1_content[7] = '4';}
                else if(angle2_1 == 5){uart1_content[7] = '5';}
                else if(angle2_1 == 6){uart1_content[7] = '6';}
                else if(angle2_1 == 7){uart1_content[7] = '7';}
                else if(angle2_1 == 8){uart1_content[7] = '8';}
                else if(angle2_1 == 9){uart1_content[7] = '9';}

                if(angle2_2 == 0){uart1_content[8] = '0';}
                else if(angle2_2 == 1){uart1_content[8] = '1';}
                else if(angle2_2 == 2){uart1_content[8] = '2';}
                else if(angle2_2 == 3){uart1_content[8] = '3';}
                else if(angle2_2 == 4){uart1_content[8] = '4';}
                else if(angle2_2 == 5){uart1_content[8] = '5';}
                else if(angle2_2 == 6){uart1_content[8] = '6';}
                else if(angle2_2 == 7){uart1_content[8] = '7';}
                else if(angle2_2 == 8){uart1_content[8] = '8';}
                else if(angle2_2 == 9){uart1_content[8] = '9';}

                if(angle2_3 == 0){uart1_content[9] = '0';}
                else if(angle2_3 == 1){uart1_content[9] = '1';}
                else if(angle2_3 == 2){uart1_content[9] = '2';}
                else if(angle2_3 == 3){uart1_content[9] = '3';}
                else if(angle2_3 == 4){uart1_content[9] = '4';}
                else if(angle2_3 == 5){uart1_content[9] = '5';}
                else if(angle2_3 == 6){uart1_content[9] = '6';}
                else if(angle2_3 == 7){uart1_content[9] = '7';}
                else if(angle2_3 == 8){uart1_content[9] = '8';}
                else if(angle2_3 == 9){uart1_content[9] = '9';}

                if(angle2_4 == 0){uart1_content[10] = '0';}
                else if(angle2_4 == 1){uart1_content[10] = '1';}
                else if(angle2_4 == 2){uart1_content[10] = '2';}
                else if(angle2_4 == 3){uart1_content[10] = '3';}
                else if(angle2_4 == 4){uart1_content[10] = '4';}
                else if(angle2_4 == 5){uart1_content[10] = '5';}
                else if(angle2_4 == 6){uart1_content[10] = '6';}
                else if(angle2_4 == 7){uart1_content[10] = '7';}
                else if(angle2_4 == 8){uart1_content[10] = '8';}
                else if(angle2_4 == 9){uart1_content[10] = '9';}

                if(angle2_5 == 0){uart1_content[11] = '0';}
                else if(angle2_5 == 1){uart1_content[11] = '1';}
                else if(angle2_5 == 2){uart1_content[11] = '2';}
                else if(angle2_5 == 3){uart1_content[11] = '3';}
                else if(angle2_5 == 4){uart1_content[11] = '4';}
                else if(angle2_5 == 5){uart1_content[11] = '5';}
                else if(angle2_5 == 6){uart1_content[11] = '6';}
                else if(angle2_5 == 7){uart1_content[11] = '7';}
                else if(angle2_5 == 8){uart1_content[11] = '8';}
                else if(angle2_5 == 9){uart1_content[11] = '9';}

                Serial.println(angle3direction);
                Serial.println(angleString_3);
              }

              if (header.indexOf("GET /A3MOVE") >= 0)
              {
                Serial.println("motor3 move");
                uart1_content[13] = '4';
                uart1_send = 1;
              }

              if (header.indexOf("GET /MOTOR3/reset") >= 0)
              {
                uart1_content[12] = '1';
                uart1_content[13] = '4';
                uart1_send = 1;
              }

              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              // CSS to style the on/off buttons
              // Feel free to change the background-color and font-size attributes to fit your preferences
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}");
              client.println(".slider { width: 250px; }");
              client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 8px 20px;");
              client.println("text-decoration: none; font-size: 15px; margin: 2px; cursor: pointer;}");
              client.println(".button3 {background-color: #4CAF50;}");
              client.println(".button4 {background-color: #FF6000;}");
              client.println(".button2 {background-color: #555555;}</style></head>");
              client.println("<script src=\"http://libs.baidu.com/jquery/2.0.0/jquery.js\"></script>");

              // Web Page Heading
              client.println("<body><h1>ESP32 MOTOR TEST</h1>");

              //******************************************************************************************//
              //motor1//
              client.println("<div id=\"motor1\" style=\"background-color:#FFD700;height:100px;width:350px;float:left;\">");

              client.println("<p>LED 1 - State " + led1State + "</p>");
              if (led1State == "off")
              {
                client.println("<p><a href=\"/LED1/on\"><button class=\"button\">ON</button></a></p>");
              }
              else
              {
                client.println("<p><a href=\"/LED1/off\"><button class=\"button button2\">OFF</button></a></p>");
              }

              client.println("<p>MOTOR 1 - State " + motor1State + "</p>");

              if (motor1State == "stop")
              {
                client.println("<p><a href=\"/MOTOR1/left\"><button class=\"button button2\">LEFT</button></a>  <a href=\"/MOTOR1/stop\"><button class=\"button\">STOP</button></a>  <a href=\"/MOTOR1/right\"><button class=\"button button2\">RIGHT</button></a></p>");
              }
              else if (motor1State == "left")
              {
                client.println("<p><a href=\"/MOTOR1/left\"><button class=\"button\">LEFT</button></a>  <a href=\"/MOTOR1/stop\"><button class=\"button button2\">STOP</button></a>  <a href=\"/MOTOR1/right\"><button class=\"button button2\">RIGHT</button></a></p>");
              }
              else if (motor1State == "right")
              {
                client.println("<p><a href=\"/MOTOR1/left\"><button class=\"button button2\">LEFT</button></a>  <a href=\"/MOTOR1/stop\"><button class=\"button button2\">STOP</button></a>  <a href=\"/MOTOR1/right\"><button class=\"button\">RIGHT</button></a></p>");
              }

              client.println("<p>SPEED 1 </p>");
              client.println("<p>speed set: <span id=\"servoPos\"></span></p>");
              client.println("<input type=\"range\" min=\"1\" max=\"20\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString_1+"\"/>");
              
              client.println("<script>var slider = document.getElementById(\"servoSlider\");");
              client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
              client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
              client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
              client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");

              if (poscon == 1)
              {
                position1 = String(readValue);
                client.println("<p>POSITION1 VALUE: " + position1 + "</p>");
                client.println("<p><a href=\"/POS1\"><button class=\"button\">POS</button></a></p>");
                poscon = 0;
              }
              else
              {
                client.println("<p>POSITION1 VALUE: " + position1 + "</p>");
                client.println("<p><a href=\"/POS1\"><button class=\"button\">POS</button></a></p>");
              }

              client.println("<p>ANGLE 1 </p>");
              client.println("<p id=\"angel1\">set angle: <span id=\"angle1set\"></span></p>");
              client.println("<input type=\"text\" id=\"motor1anginput\"><a href=\"/A1MOVE\"><button class=\"button button3\">MOVE</button></a>");
              client.println("<script>var ang1 = document.getElementById(\"angle1set\"); ang1.innerHTML = \"" + angle1direction + " " + angleString_1 + "\";");
              client.println("function angle1left() {var x1 = document.getElementById(\"motor1anginput\").value;");
              client.println("$.get(\"/?angle1left=\" + x1 + \"&\"); {Connection: close};}");
              client.println("function angle1right() {var x1 = document.getElementById(\"motor1anginput\").value;");
              client.println("$.get(\"/?angle1right=\" + x1 + \"&\"); {Connection: close};}</script>");
              client.println("<p><button class=\"button button3\" onclick=\"angle1left()\">LEFT</button> <button class=\"button button3\" onclick=\"angle1right()\">RIGHT</button> <a href=\"/MOTOR1/reset\"><button class=\"button button4\">RESET</button></a></p>");

              client.println("</div>");

              //******************************************************************************************//
              //motor2 ID = 3//

              client.println("<div id=\"motor2\" style=\"float:left;\">");
              client.println("<p>LED 2 - State " + led2State + "</p>");
              if (led2State == "off")
              {
                client.println("<p><a href=\"/LED2/on\"><button class=\"button\">ON</button></a></p>");
              }
              else
              {
                client.println("<p><a href=\"/LED2/off\"><button class=\"button button2\">OFF</button></a></p>");
              }

              client.println("<p>MOTOR 2</p>");
              
              client.println("<p>ANGLE 2 </p>");
              client.println("<p id=\"angel2\">set angle: <span id=\"angle2set\"></span></p>");
              client.println("<input type=\"text\" id=\"motor2anginput\"><a href=\"/A2MOVE\"><button class=\"button button3\">MOVE</button></a>");
              client.println("<script>var ang2 = document.getElementById(\"angle2set\"); ang2.innerHTML = \"" + angle2direction + " " + angleString_2 + "\";");
              client.println("function angle2left() {var x2 = document.getElementById(\"motor2anginput\").value;");
              client.println("$.get(\"/?angle2left=\" + x2 + \"&\"); {Connection: close};}");
              client.println("function angle2right() {var x2 = document.getElementById(\"motor2anginput\").value;");
              client.println("$.get(\"/?angle2right=\" + x2 + \"&\"); {Connection: close};}</script>");
              client.println("<p><button class=\"button button3\" onclick=\"angle2left()\">LEFT</button> <button class=\"button button3\" onclick=\"angle2right()\">RIGHT</button> <a href=\"/MOTOR2/reset\"><button class=\"button button4\">RESET</button></a></p>");


              //******************************************************************************************//
              //motor3 ID = 4//

              client.println("<p>=======================================</p>");
              client.println("<p>LED 3 - State " + led3State + "</p>");
              if (led3State == "off")
              {
                client.println("<p><a href=\"/LED3/on\"><button class=\"button\">ON</button></a></p>");
              }
              else
              {
                client.println("<p><a href=\"/LED3/off\"><button class=\"button button2\">OFF</button></a></p>");
              }

              client.println("<p>MOTOR 3</p>");
              
              client.println("<p>ANGLE 3 </p>");
              client.println("<p id=\"angel3\">set angle: <span id=\"angle3set\"></span></p>");
              client.println("<input type=\"text\" id=\"motor3anginput\"><a href=\"/A3MOVE\"><button class=\"button button3\">MOVE</button></a>");
              client.println("<script>var ang3 = document.getElementById(\"angle3set\"); ang3.innerHTML = \"" + angle3direction + " " + angleString_3 + "\";");
              client.println("function angle3left() {var x3 = document.getElementById(\"motor3anginput\").value;");
              client.println("$.get(\"/?angle3left=\" + x3 + \"&\"); {Connection: close};}");
              client.println("function angle3right() {var x3 = document.getElementById(\"motor3anginput\").value;");
              client.println("$.get(\"/?angle3right=\" + x3 + \"&\"); {Connection: close};}</script>");
              client.println("<p><button class=\"button button3\" onclick=\"angle3left()\">LEFT</button> <button class=\"button button3\" onclick=\"angle3right()\">RIGHT</button> <a href=\"/MOTOR3/reset\"><button class=\"button button4\">RESET</button></a></p>");

              client.println("</div>");
              
              // The HTTP response ends with another blank line
              client.println();
              // Break out of the while loop
              break;
            }
            else
            { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          }
          else if (c != '\r')
          {
            // if you got anything else but a carriage return character,
            currentLine += c; // add it to the end of the currentLine
          }
        }
      }
      // Clear the header variable
      header = "";
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
  }
}

void codSERVE(void *xTask3)
{
  while (1)
  {
    I2Cone.requestFrom(0X36, (uint8_t)2);
    for (byte i = 0; i < 2; i++)
    {
      readArray[i] = I2Cone.read();
    }

    // depending on the sensor architecture there are different combinations of
    // LSB and MSB register used bits
    // AS5600 uses 0..7 LSB and 8..11 MSB
    // AS5048 uses 0..5 LSB and 6..13 MSB
    // readValue = ( readArray[1] &  lsb_mask );
    // readValue += ( ( readArray[0] & msb_mask ) << lsb_used );
    readValue = readArray[0] * 256 + readArray[1];
    angle_dee = readValue * angle_res;
    // Serial.println(readValue);
    // Serial.println(angle_dee);
    delay(1);
  }
}

void uartSERVE(void *xTask4)
{
  while (1)
  {
    if(uart1_send == 1)
    {
      Serial1.println(uart1_content);  // ID = 3
      Serial.println(uart1_content);

      if (uart1_content[5] == '1')
      {
        uart1_content[5] = '0';
      }
      for (int i = 0; i<7; i++)
      {
        uart1_content[i + 6] = '0';
      }

      uart1_send = 0;
    }
    delay(10);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  // Initialize the output variables as outputs
  pinMode(outputled1, OUTPUT);
  pinMode(output_motor_a1, OUTPUT);
  pinMode(output_motor_a2, OUTPUT);
  pinMode(output_motor_b1, OUTPUT);
  pinMode(output_motor_b2, OUTPUT);
  // Set outputs to LOW
  digitalWrite(outputled1, LOW);
  digitalWrite(output_motor_a1, LOW);
  digitalWrite(output_motor_a2, LOW);
  digitalWrite(output_motor_b1, LOW);
  digitalWrite(output_motor_b2, LOW);

  I2Cone.begin(21, 22, 100000);
  I2Cone.beginTransmission(0X36);
  I2Cone.write(0X0C);
  I2Cone.endTransmission(false);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (com_led == 0)
    {
      digitalWrite(outputled1, LOW);
      com_led = 1;
    }
    if (com_led == 1)
    {
      digitalWrite(outputled1, HIGH);
      com_led = 0;
    }
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  digitalWrite(outputled1, LOW);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

#if USE_MULTCORE
  xTaskCreate(motorSERVE, "motorSERVE", 4096, NULL, 1, NULL);
  xTaskCreate(webSERVE, "webSERVE", 4096, NULL, 2, NULL);
  xTaskCreate(codSERVE, "codSERVE", 4096, NULL, 3, NULL);
  xTaskCreate(uartSERVE, "uartSERVE", 4096, NULL, 4, NULL);

#else
  //最后一个参数至关重要，决定这个任务创建在哪个核上.PRO_CPU 为 0, APP_CPU 为 1,或者 tskNO_AFFINITY 允许任务在两者上运行.
  xTaskCreatePinnedToCore(motorSERVE, "motorSERVE", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(webSERVE, "webSERVE", 4096, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(codSERVE, "codSERVE", 4096, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(uartSERVE, "uartSERVE", 4096, NULL, 4, NULL, 1);

#endif
}

void loop()
{
}
