#include <vs-rc202.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <FS.h>

#define GO 0
#define LEFT 1
#define RIGHT 2
#define BACK 3
#define STOP 4
#define FUNC1 5
#define FUNC2 6
#define FUNC3 7
#define FUNC4 8

const char* ui_path = "/index.html";
const char* ssid = "SSID";
const char* password = "password";
#define BUF_SIZE 10240
uint8_t buf[BUF_SIZE];
int led_onoff_flag = 0;

ESP8266WebServer server(80);

int loadUI(){
  File ui = SPIFFS.open(ui_path, "r");
  if (!ui) {
    Serial.println("index.html does not exist.");
    return 0;
  }else if(ui.size() >= BUF_SIZE){
    Serial.print("Could not load file. Make file smaller.");
    return 0;
  }else{
    ui.read(buf,ui.size());
    ui.close();
    return 1;
  }
}

int home_position[1][11] = {
                      {800,0,0,0,0,0,0,0,0,0,0},
                    };

int go[9][11] = {
                  {200,0,0,0,-1000,0,0,0,0,0,0},
                  {300,0,200,0,-1000,0,0,0,0,0,0},
                  {200,0,200,0,1000,0,0,0,0,0,0},
                  {300,0,200,0,1000,1,0,0,0,0,0},
                  {50,0,0,0,1000,0,0,0,0,0,0},
                  {300,0,0,-200,1000,0,0,0,0,0,0},
                  {200,0,0,-200,-1000,0,0,0,0,0,0},
                  {300,0,0,-200,-1000,1,0,0,0,0,0},
                  {50,0,0,0,-1000,0,0,0,0,0,0}
                };
                

int right[9][11] = {
                    {300,0,350,-300,-500,0,0,0,0,0,0},
                    {300,0,1400,-300,-500,0,0,0,0,0,0},
                    {500,0,1400,-450,0,0,0,0,0,0,0},
                    {300,0,1400,-450,0,1,0,0,0,0,0},
                    {800,0,650,-600,0,100,0,0,0,0,0},
                    {300,0,650,-600,0,0,0,0,0,0,0},
                    {500,0,650,-600,0,0,0,0,0,0,0},
                    {500,0,0,0,0,0,0,0,0,0,0},
                    {500,0,0,0,0,1,0,0,0,0,0}
                    };


int left[9][11] = {
                    {300,0,350,-300,500,0,0,0,0,0,0},
                    {300,0,350,-1100,500,0,0,0,0,0,0},
                    {500,0,500,-1100,0,0,0,0,0,0,0},
                    {300,0,500,-1100,0,1,0,0,0,0,0},
                    {800,0,650,-600,0,0,0,0,0,0,0},
                    {300,0,650,-600,0,0,0,0,0,0,0},
                    {500,0,650,-600,0,1,0,0,0,0,0},
                    {500,0,0,0,0,0,0,0,0,0,0},
                    {500,0,0,0,0,1,0,0,0,0,0}
                    };

int back[9][11] = {
                  {300,0,-200,200,-1300,0,0,0,0,0,0},
                  {100,0,-400,200,-1300,0,0,0,0,0,0},
                  {500,0,-400,200,1300,0,0,0,0,0,0},
                  {500,0,-400,200,1300,1,0,0,0,0,0},
                  {100,0,-200,200,1300,0,0,0,0,0,0},
                  {100,0,-200,400,1300,0,0,0,0,0,0},
                  {500,0,-200,400,-1300,0,0,0,0,0,0},
                  {500,0,-200,400,-1300,1,0,0,0,0,0},
                  {100,0,-200,200,-1300,0,0,0,0,0,0}
                };

int f1[3][11] = {
                      {300,0,1500,-1400,0,0,0,0,0,0,0},
                      {1000,0,1500,-1400,0,1,0,0,0,0,0},
                      {800,0,0,0,0,0,0,0,0,0,0}
                    };

int f2[3][11] = {
                      {300,0,-1600,1800,0,0,0,0,0,0,0},
                      {800,0,-1600,1800,0,1,0,0,0,0,0},
                      {500,0,0,0,0,0,0,0,0,0,0},
                    };

int f3[6][11] = {
                      {300,0,0,0,1300,0,0,0,0,0,0},
                      {300,0,0,800,1300,0,0,0,0,0,0},
                      {300,0,0,-1300,1300,0,0,0,0,0,0},
                      {300,0,0,-1300,1300,1,0,0,0,0,0},
                      {300,0,0,0,1300,0,0,0,0,0,0},
                      {300,0,0,0,0,0,0,0,0,0,0}
                    };

int f4[2][11] = {
                      {800,0,0,0,1000,0,0,0,0,0,0},
                      {800,0,0,0,-1000,0,0,0,0,0,0},
                    };

void selectMotion(){
  
  switch(getMotionNumber()){
    case GO:
      playMotion(go, 9);
      break;
    case LEFT:
      playMotion(left, 9);
      break;
    case RIGHT:
      playMotion(right, 9);
      break;
    case BACK:
      playMotion(back, 9);
      break;
    case STOP:
      stopServo();
      delay(200);
      playMotionOnce(home_position, 1);
      break;
   case FUNC1:
      playMotionOnce(f1, 3);
      break;
   case FUNC2:
      playMotionOnce(f2, 3);
      break;
   case FUNC3:
      playMotionOnce(f3, 6);
      break;
   case FUNC4:
      playMotion(f4, 2);
      break;
  }
}

//Send UI page
void handleRoot(){
  server.send(200, "text/html", (char *)buf);
}

void Go(){
  setMotionNumber(GO);
  Serial.println("Go");
  server.send(200, "text/html","Go");
}

void Left(){
  setMotionNumber(LEFT);
  Serial.println("Left");
  server.send(200, "text/html","Left");
}

void Right(){
  setMotionNumber(RIGHT);
  Serial.println("Right");
  server.send(200, "text/html","Right");
}

void Back(){
  setMotionNumber(BACK);
  Serial.println("Back");
  server.send(200, "text/html","Back");
}

void Stop(){
  setMotionNumber(STOP);
  Serial.println("Stop");
  server.send(200, "text/html","Stop");
}

void F1(){
  setMotionNumber(FUNC1);
  Serial.println("F1");
  server.send(200, "text/html","F1");
}

void F2(){
  setMotionNumber(FUNC2);
  Serial.println("F2");
  server.send(200, "text/html","F2");
}

void F3(){
  setMotionNumber(FUNC3);
  Serial.println("F3");
  server.send(200, "text/html","F3");
}

void F4(){
  setMotionNumber(FUNC4);
  Serial.println("F4");
  server.send(200, "text/html","F4");
}

void Sens(){

  int sens_data[3];

  //Sensor data from lpc
  int data1 = readSens(1);
  int data2 = readSens(2);
  int data3 = readSens(3);
 
  //Sonic sensor data
  double dist = getDist();

  //Power supply voltage
  int power = readPow();

  //Convert numeric to string
  String st_data1 = String(data1);
  String st_data2 = String(data2);
  String st_data3 = String(data3);  
  String st_data4 = String(dist);
  String st_data5 = String(power);
  
  String res = String(st_data1+","+st_data2+","+st_data3+","+st_data4+","+st_data5);
  server.send(200, "text/html",res);
}

void setup() {

  //debug
  Serial.begin(115200);
  delay(500);

  //Init robot lib
  initLib();
  delay(10);

  //SV2 - 4 servo mode
  servoEnable(2, 1);        //Enable SV2 PWM
  servoEnable(3, 1);        //Enable SV3 PWM
  servoEnable(4, 1);        //Enable SV4 PWM

  //Offset
  setServoOffset(2,0);
  setServoOffset(3,0);
  setServoOffset(4,0);

  //Read html file
  SPIFFS.begin();
  if(!loadUI()){
    return;
  }

  //Connect to AP
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  Serial.println("Connecting to Access Point");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID()); 
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Set function
  server.on("/", handleRoot);
  server.on("/go/", Go);
  server.on("/left/", Left);
  server.on("/right/", Right);
  server.on("/back/", Back);
  server.on("/stop/", Stop);
  server.on("/f1/", F1);
  server.on("/f2/", F2);
  server.on("/f3/", F3);
  server.on("/f4/", F4);
  server.on("/sens/", Sens);

  //Start server
  server.begin();
}

void loop() {
  
  server.handleClient();
  //Play motion
  selectMotion();
}
