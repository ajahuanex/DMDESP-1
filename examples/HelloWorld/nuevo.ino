//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Test P10
/*
 * Original source code : https://github.com/busel7/DMDESP/blob/master/examples/TeksDiamdanJalan/TeksDiamdanJalan.ino by  busel7
 * Links to download libraries : https://github.com/busel7/DMDESP
*/

//----------------------------------------Include Library
//----------------------------------------see here: https://www.youtube.com/watch?v=8jMr94B8iN0 to add NodeMCU ESP8266 library and board
#include <DMDESP.h>
#include <fonts/Arial14.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//----------------------------------------
unsigned long currentTime=0;
unsigned long previousTime=0;

// Update these with values suitable for your network.

const char* ssid = "Paputec-2.4Ghz";
const char* password = "Eidan12345";
const char* mqtt_server = "192.168.100.254";
String data;
char *inData;
char aux;
int m = 0;
int le=0;
String myString="Hola";


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
bool p=false;

void setup_wifi() {
 currentTime=millis();
      if(currentTime-previousTime>10){
        previousTime=currentTime;
      }
 // delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    //delay(500);
     currentTime=millis();
      if(currentTime-previousTime>500){
        previousTime=currentTime;
      
      }
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

///////////////////////////////////callback/////////////////////////

void callback(char* topic, byte* payload, unsigned int length) {
  //String aux ="";
  char text[length];
  p=true;
  if(p){
    Serial.println("Puntero enviado memset + length");
    Serial.print(length);
    le=length+1;
    memset(text,0,length); 
  }
  
    
  Serial.print("Message arrived [");
  
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    text[i]=(char)payload[i];
    Serial.print((char)payload[i]);          
  }
  myString = String(text); 
  Serial.println();
  
  Serial.print(myString);

  Serial.print(length);

     

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic","Hola Mundo iniciado");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
       currentTime=millis();
      if(currentTime-previousTime>5000){
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      //delay(5000);
      previousTime=currentTime;
      }
    }
  }
}



//----------------------------------------DMD Configuration (P10 Panel)
#define DISPLAYS_WIDE 2 //--> Panel Columns
#define DISPLAYS_HIGH 1 //--> Panel Rows
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);  //--> Number of Panels P10 used (Column, Row)
//----------------------------------------

//========================================================================VOID SETUP()
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  //----------------------------------------DMDESP Setup
  Disp.start(); //--> Run the DMDESP library
  Disp.setBrightness(8); //--> Brightness level
  Disp.setFont(Arial14); //--> Determine the font used
  //----------------------------------------
}
//========================================================================

//========================================================================VOID LOOP()
void loop() {
  currentTime=millis();

    if (!client.connected()) {
    reconnect();
  }

  
  client.loop();

  unsigned long now = millis();
  char st[le];
  myString.toCharArray(st,le);
  
  /*if (now - lastMsg > 5000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "Hola mundo 2 #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    data=msg;       
    client.publish("outTopic",st );
  }*/

  
  
  Disp.loop(); //--> Run "Disp.loop" to refresh the LED
    if(p){
    Serial.println("P enviado a loop");
    m=1;
    p=false;
    }
  //Disp.drawString(0, 0, "            "); //--> Display text "Disp.drawText(x position, y position, text)"  
  
  Scrolling_Text(m, 50,myString); //--> Show running text "Scrolling_Text(y position, speed);"
  
  
}


//========================================================================
//char copy[100]
//data.toCharArray(copy,100)
//========================================================================Subroutines for scrolling Text
 //static char *Text[] ={} ;
 //data.toCharArray(Text[])

void Scrolling_Text(int n, uint8_t scrolling_speed,String data) {
  static uint32_t pM;
  static uint32_t a;
  static uint32_t x;
  static uint32_t y;
  static uint32_t r;
  static uint32_t s;
  static uint32_t t;
  unsigned long de;
  n=m;
  
  char b[le];
  data.toCharArray(b,le);
  
     
  int width = Disp.getWidth();
  
  Disp.setFont(Arial14);
  
 int fullScroll = Disp.getTextWidth(b) + width;
 int mid =round(width - Disp.getTextWidth(b))/2;
 
  
  
  switch (n) {
  case 1:
    // statements
    
    if((millis() - pM) > scrolling_speed) { 
    pM = millis();
    if (y< 16) {
      ++y;
      a=0;      
          
    } else {
      m=2;       
      y=0;
      Serial.print("en 1 s-->1 m=");
      Serial.print(m);                      
      return ;
    }
    Disp.clearScreen();
    Disp.drawString(mid,16-y,b);
    }
        
    break;
  case 2:
    // statements
        if ((millis() - t) > 100) {
     t=millis();
       if (a< 50) {
      ++a;      
       x=0;   
    } else {
      a=0;      
      m=3;
      Disp.clearScreen();
                          
      return ;
    }
    
  }

   
       break;
  case 3:

    if((millis() - s) > scrolling_speed) { 
    s = millis();
    if (x<fullScroll ) {
      ++x;
      a=0;      
          
    } else {
      x=0;
      m=0;                  
      return ;
    }
          
    Disp.drawString(mid-x,y,b);
    }
    
    

    break;
  default:
    // statements
  
    
  if ((millis() - r) > 100) {
     r=millis();
       if (a< 50) {
      ++a;      
          
    } else {
      a=0;
      m=0;
      Disp.clearScreen();
      Serial.println("No hay data 9 y m = ");
      Serial.println();                       
      return ;
    }
    
  }
    
  
    break;
 

  } 

}
//========================================================================
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
