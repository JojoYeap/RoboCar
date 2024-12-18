#include "esp_camera.h"
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

WebSocketsServer webSocket = WebSocketsServer(81);
AsyncWebServer server(80);
uint8_t cam_num;
bool connected = false;

String index_html =   R"(
<!DOCTYPE html>
<html>
<head>
<title> WebSockets Client</title>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<script src='http://code.jquery.com/jquery-1.9.1.min.js'></script>
</head>
<body scroll="no" style="overflow: hidden;position: fixed;
font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif ;
color:rgb(128, 128, 128);
font-size: xx-large;">
    <p style="text-align: center;">
        X: <span id="x_coordinate"> </span>
        Y: <span id="y_coordinate"> </span> <br> <br>
        <img id='live' src='' display>
    </p>
<canvas id="canvas"></canvas>
</body>
</html>
<script>
jQuery(function($){
    if (!('WebSocket' in window)) {
        alert('Your browser does not support web sockets');
    } else {
        setup();
    }

    function setup() {
        var host = 'ws://server_ip:81';
        var socket = new WebSocket(host);
        socket.binaryType = 'arraybuffer';

        if (socket) {
            socket.onopen = function() {
            }

            socket.onmessage = function(msg) {
                var bytes = new Uint8Array(msg.data);
                var binary= '';
                var len = bytes.byteLength;
                for (var i = 0; i < len; i++) {
                    binary += String.fromCharCode(bytes[i])
                }

                var img = document.getElementById('live');
                img.src = 'data:image/jpg;base64,'+window.btoa(binary);
            }

            socket.onclose = function() {
                showServerResponse('The connection has been closed.');
            }
        }

        var x_relative, y_relative;

        setInterval(function() {
            var x = x_relative; // get x value here
            var y = y_relative; // get y value here
            send(x, y);
        }, 100);

        function send(x,y){
            var data = {x,y};
            data = JSON.stringify(data);
            console.log(data);
            socket.send(data);
        }
        
        var canvas, ctx;

        window.addEventListener('load', () => {

            canvas = document.getElementById('canvas');
            ctx = canvas.getContext('2d');          
            resize(); 

            document.addEventListener('mousedown', startDrawing);
            document.addEventListener('mouseup', stopDrawing);
            document.addEventListener('mousemove', Draw);

            document.addEventListener('touchstart', startDrawing);
            document.addEventListener('touchend', stopDrawing);
            document.addEventListener('touchcancel', stopDrawing);
            document.addEventListener('touchmove', Draw);
            window.addEventListener('resize', resize);

            document.getElementById("x_coordinate").innerText = 0;
            document.getElementById("y_coordinate").innerText = 0;
        });

      


        var width, height, radius, x_orig, y_orig;
        function resize() {
            width = window.innerWidth;
            radius = window.innerWidth/4;
            height = radius * 6;
            ctx.canvas.width = width;
            ctx.canvas.height = height;
            background();
            joystick(width / 2, height / 3);
        }

        function background() {
            x_orig = width / 2;
            y_orig = height / 3;

            ctx.beginPath();
            ctx.arc(x_orig, y_orig, radius + 15, 0, Math.PI * 2, true);
            ctx.fillStyle = '#ECE5E5';
            ctx.fill();
        }

        function joystick(width, height) {
            ctx.beginPath();
            ctx.arc(width, height, radius, 0, Math.PI * 2, true);
            ctx.fillStyle = '#3D85C6';
            ctx.fill();
            ctx.strokeStyle = '#9FC5E8';
            ctx.lineWidth = 8;
            ctx.stroke();
        }

        let coord = { x: 0, y: 0 };
        let paint = false;

        function getPosition(event) {
            var mouse_x = event.clientX || event.touches[0].clientX;
            var mouse_y = event.clientY || event.touches[0].clientY;
            coord.x = mouse_x - canvas.offsetLeft;
            coord.y = mouse_y - canvas.offsetTop;
        }

        function is_it_in_the_circle() {
            var current_radius = Math.sqrt(Math.pow(coord.x - x_orig, 2) + Math.pow(coord.y - y_orig, 2));
            if (radius >= current_radius) return true
            else return false
        }


        function startDrawing(event) {
            paint = true;
            getPosition(event);
            if (is_it_in_the_circle()) {
                ctx.clearRect(0, 0, canvas.width, canvas.height);
                background();
                joystick(coord.x, coord.y);
                Draw();
            }
        }


        function stopDrawing() {
            paint = false;
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            background();
            joystick(width / 2, height / 3);
            document.getElementById("x_coordinate").innerText = 0;
            document.getElementById("y_coordinate").innerText = 0;
            x_relative = 0;
            y_relative = 0;            

        }

        function Draw(event) {

            if (paint) {
                ctx.clearRect(0, 0, canvas.width, canvas.height);
                background();
                var angle_in_degrees, x, y;
                var angle = Math.atan2((coord.y - y_orig), (coord.x - x_orig));

                if (Math.sign(angle) == -1) {
                    angle_in_degrees = Math.round(-angle * 180 / Math.PI);
                }
                else {
                    angle_in_degrees =Math.round( 360 - angle * 180 / Math.PI);
                }

                if (is_it_in_the_circle()) {
                    joystick(coord.x, coord.y);
                    x = coord.x;
                    y = coord.y;
                }
                else {
                    x = radius * Math.cos(angle) + x_orig;
                    y = radius * Math.sin(angle) + y_orig;
                    joystick(x, y);
                }

            
                getPosition(event);

                x_relative = Math.round(x - x_orig);
                y_relative = Math.round(y - y_orig);

                document.getElementById("x_coordinate").innerText =  x_relative;
                document.getElementById("y_coordinate").innerText =  y_relative ;
            }
        } 
    }
});
</script>
)";

void configCamera(){
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG;

  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 15;
  config.fb_count = 100;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void liveCam(uint8_t num){
  //capture a frame
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
      Serial.println("Frame buffer could not be acquired");
      return;
  }
  //replace this with your own function
  webSocket.sendBIN(num, fb->buf, fb->len);

  //return the frame buffer back to be reused
  esp_camera_fb_return(fb);
}

String message = "";

void handleWebSocketMessage(uint8_t *data, size_t len) {
    data[len] = 0;
    message = (char*)data;
    Serial.println(message);
    moveMotor();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            cam_num = num;
            connected = true;
            break;
        case WStype_TEXT:
            handleWebSocketMessage(payload, length);
            break;
        case WStype_BIN:
        case WStype_ERROR:      
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            break;
    }
}

int x_reading = 0;
int y_reading = 0;


//--------------------------------------------------------Motor
// Motor A
const int motor1Pin1 = 15;
const int motor1Pin2 = 3;
const int enable1Pin = 13;

//Motor B
const int motor2Pin1 = 14;
const int motor2Pin2 = 2;
const int enable2Pin = 12;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel_1 = 0;
const int pwmChannel_2 = 1;
const int motorResolution = 8;
int dutyCycle1 = 200;
int dutyCycle2 = 200;

//----------------------------------------------------FUNCTIONS--------------------------------------------------------//


// variables for motor drivers
#define minspd 80
#define maxspd 150

int leftMotor =0, rightMotor=0;
int sped(int dSpeed){
  int nspd;
  if (abs(dSpeed)>100){
    dSpeed > 0 ? dSpeed = 100 : dSpeed = -100;
  }
  if (dSpeed > 0){
    nspd = map(dSpeed, 0, 100, minspd, maxspd);
  }else if (dSpeed < 0){
    nspd = map(dSpeed, -100, 0, -maxspd, -minspd);
  }
  else{
    nspd = 0;
  }
  return nspd;
}

void motor1(int Hspd) {
  int spd = sped(Hspd);
  if (spd > 5) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
  }else if (spd < -5) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    spd = -spd;
  }else{
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
  }
  ledcWrite(pwmChannel_1, spd);
}

void motor2(int Hspd) {
  int spd = sped(Hspd);
  if (spd > 5) {
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
  }else if (spd < -5) {
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    spd = -spd;
  }else {
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
  }
  ledcWrite(pwmChannel_2, spd);
}

void stopAll() {
  motor1(0);
  motor2(0);
}

void moveMotor(){
  if(message.length()>2){
    x_reading = message.substring(message.indexOf(':')+1,message.indexOf(',')).toInt();
    Serial.println(x_reading);
    y_reading = message.substring(message.indexOf(',')+5,message.length()).toInt();
    Serial.println(y_reading);
  }
  if (abs(y_reading)>15){
    leftMotor = y_reading;
    rightMotor = y_reading;
  }else{
    leftMotor = 0, rightMotor= 0;
  }

  if (abs(x_reading)>20){
    rightMotor = rightMotor - x_reading;
    leftMotor = leftMotor + x_reading;
  }

  motor1(leftMotor);
  motor2(rightMotor);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("Wifi", "WifiPassword");
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  String IP = WiFi.localIP().toString();
  Serial.print("IP address: " + IP);
  index_html.replace("server_ip", IP);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/html", index_html);
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  configCamera();

  //sets the pins for motors as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  //configure LED PWM functionalitites
  ledcSetup(pwmChannel_1, freq, motorResolution);
  ledcSetup(pwmChannel_2, freq, motorResolution);
  //attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel_1);
  ledcAttachPin(enable2Pin, pwmChannel_2);
}

void loop() {
  //http_resp();
  webSocket.loop();
  liveCam(cam_num);

}