#include <DMDESP.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <fonts/Arial14.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/Arial_Black_16_ISO_8859_1.h>
#include <fonts/Arial_bold_14.h>
#include <fonts/Droid_Sans_16.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/Droid_Sans_12.h>
#include <fonts/DejaVuSansBold9.h>
#include <fonts/Corsiva_12.h>
//----------------------------------------
unsigned long currentTime=0;
unsigned long previousTime=0;

//----------------------------------------DMD Configuration (P10 Panel)
#define DISPLAYS_WIDE 2 //--> Panel Columns
#define DISPLAYS_HIGH 1 //--> Panel Rows
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);  //--> Number of Panels P10 used (Column, Row)
uint8_t scrolling_speed = 50; //
int scrolling_ligth; //
//----------------------------------------

// Update these with values suitable for your network.
const char* ssid = "Paputec-2.4Ghz";
const char* password = "Eidan12345";

const char* mqtt_server = "192.168.10.11";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

  int lenPrice=2;
  int lenDesc=2;
  int lenTime=2;
  int numberScroll =0;
  int aux=8;
  String dataPrice = "";
  String dataDesc = "";
  String dataTime = "";
  String tipoLetra = "2";

 bool monostable=false;
 bool enableTipoLetra=false;
//-----------------Setup Wifi----------------------------

void setup_wifi() {
  // We start by connecting to a WiFi network
  delay(5);
  Serial.println();
  Serial.print("Connectando a ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);  

while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");            
  }

  randomSeed(micros());
  
  Serial.println("");             
  Serial.println("WiFi connectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //numberScroll =1;
}
//-----------------end Setup Wifi----------------------------

void setup_panel(){
  Disp.start(); //--> Run the DMDESP library
  Disp.setBrightness(10); //--> Brightness level
  Disp.setFont(Arial14);
  //Disp.drawString(0,0,"Wifi Ok !");
}

//----------------------MQTT------------
void setup_mqtt(){
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback); 
 
}
///////////////////////////////////callback/////////////////////////

void callback(char* topic, byte* payload, unsigned int length) {

  
  char text[length];
  
  
  Serial.print("Message arrived [");  
  Serial.print(topic);
  Serial.print("] ");
  String myTopicString(topic);
  String str;
 
  for (int i = 0; i < length; i++) {
    text[i]=(char)payload[i];
    Serial.print((char)payload[i]);
    str+=(char)payload[i];          
  }
  if (myTopicString=="inPanelLuz"){    
    scrolling_ligth = str.toInt();   
  }
   if (myTopicString=="inPanelLetra"){
    tipoLetra = str;      
  }
  if(myTopicString=="inPrice"){
    monostable=true;
    dataPrice = str;
    lenPrice = length;
    Serial.println();  
    Serial.print("lenPrice : ");
    Serial.println(lenPrice);
  }
    if(myTopicString=="inTime"){
    dataTime = str;
    lenTime = length;
   
  }
  if(myTopicString=="inDesc"){
    dataDesc = str;
    lenDesc = length;
    Serial.println();  
    Serial.print("lenDesc : ");
    Serial.println(lenDesc);
  }
    if(myTopicString=="inTipoLetra"){
      monostable=true;
      enableTipoLetra=true;
    tipoLetra = str;
    dataPrice = "Agua-123";
    dataDesc = "";
    lenPrice=8;
    lenDesc=1;
    Serial.println();  
    Serial.print("lenTipoLetra : ");
    Serial.println(lenPrice);
  }
    if(myTopicString=="inLuz"){
      monostable=true;
    scrolling_ligth = str.toInt();
    dataPrice = "Luz "+ str;
    dataDesc = "";
    lenPrice=length+4;
    lenDesc=1;
    Serial.println();  
    Serial.print("lenLuz : ");
    Serial.println(lenPrice);
  }

}


/////////////////////reconect//////////////////////////
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection..... ");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic","Hola Mundo iniciado Ok");
      client.publish("outTopic","start");
      // ... and resubscribe
      client.subscribe("inPrice");
      client.subscribe("inTipoLetra");
      client.subscribe("inLuz");
      client.subscribe("inDesc");
      client.subscribe("inTime");
      client.subscribe("inDate");
       client.subscribe("inPanelLuz");
      client.subscribe("inPanelLetra");
      client.subscribe("outTopic");
    } else {
      //  currentTime=millis();
      // if(currentTime-previousTime>5000){
      // Serial.print("failed, rc=");
      // Serial.print(client.state());
      // Serial.println(" try again in 5 seconds");
      // // Wait 5 seconds before retrying
      // //delay(5000);
      // previousTime=currentTime;
      // }

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);

    }
  }
}

//------------------end MQTT-------
void setup() {
  Serial.begin(9600);
  Serial.println();
  setup_wifi();    
  setup_mqtt();
  setup_panel();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  client.loop();
  if (!client.connected()) {
    reconnect();
  }
  Disp.loop();
 Disp.setBrightness(255*scrolling_ligth/100);
 typeFont(tipoLetra);
 Scrolling_Text(dataPrice,dataDesc,tipoLetra, dataTime);
}

 void Scrolling_Text(String dataPrice,String dataDesc, String tipoLetra,String dataTime){
   
   char priceBuffer[lenPrice+1];
   dataPrice.toCharArray(priceBuffer,lenPrice+1);

   char timeBuffer[lenTime+1];
   dataTime.toCharArray(timeBuffer,lenTime+1);
   
   char descBuffer[lenDesc+1];
   dataDesc.toCharArray(descBuffer,lenDesc+1);
   
   int width = Disp.getWidth();

   
  // typeFont(tipoLetra);

 int fullScroll = Disp.getTextWidth(priceBuffer) + width;
 int midPrice =round((width - Disp.getTextWidth(priceBuffer))/2); 
 int midTime =round((width - Disp.getTextWidth(timeBuffer))/2); 
 int fullScrollDesc = Disp.getTextWidth(descBuffer+1);



 static uint32_t a;
 static uint32_t x;
 static uint32_t y;


 if (monostable){
    numberScroll =1; 
    monostable=false;
  }
 
//typeFont(tipoLetra);
 

 switch (numberScroll) {
  case 1:
    // statements    
    if((millis() - previousTime) > scrolling_speed) {
    previousTime = millis();
    if (y< 16) {
      ++y;
      a=0;          
    } else {
      numberScroll=2;      
      y=0;
    
      return ;
    }
    Disp.clearScreen();
    Disp.drawString(midPrice,16-y,dataPrice);
   
    }        
    break;
  case 2:
    // statements
     if ((millis() - previousTime) > 100) {
     previousTime=millis();
       if (a< 50) {
      ++a;      
       x=0;  
      } else {
      a=0;      
      numberScroll=3;
      Disp.clearScreen();                        
      return ;
       }    
     }  
     break;
  case 3:
    if((millis() - previousTime) > scrolling_speed) {
    previousTime = millis();
    if (x<fullScroll+fullScrollDesc) {
      ++x;
      a=0;          
    } else {
      x=0;
      numberScroll=0;
      return ;
    }
    Disp.clearScreen();           
    Disp.drawString(midPrice-x,y,dataPrice);
    Disp.drawString(width-x,y,dataDesc);
    }
    break;

    case 4:
    if((millis() - previousTime) > scrolling_speed) {
    previousTime = millis();
    if (y< 16) {
      ++y;
      a=0;          
    } else {
      numberScroll=5;      
      y=0;
    
      return ;
    }
    Disp.clearScreen();
    
    if(enableTipoLetra){
      aux =midTime;
      enableTipoLetra=false;
    }

    Disp.drawString(aux,16-y,dataTime);
      
    //Disp.drawString(width-x,y,dataDesc);
    }
    break;

    case 5:
    if((millis() - previousTime) > scrolling_speed) {
    previousTime = millis();
    if (x<fullScroll+fullScrollDesc) {
      ++x;
      a=0;          
    } else {
      x=0;
      previousTime =0;
      return ;
    }
    Disp.clearScreen();           
    Disp.drawString(midTime,y,dataTime);           
    
    }
    break;
   
  default:
    // statements      
    if ((millis() - previousTime) > 100) {
       previousTime=millis();
         if (a< 50) {
        ++a;            
      } else {
        a=0;
        numberScroll=4;
           Disp.clearScreen();
       
        return ;
      }      
     }          
      break;  
    }
   

   
  }

  ///////////////type font/////////////

  void typeFont(const String& tipoLetra){
      switch (tipoLetra.toInt()){
  case 0:
    Disp.setFont(Arial14);
    break;
  case 1:
    Disp.setFont(Corsiva_12);
    break;
  case 2:
    Disp.setFont(Droid_Sans_16);
    break;
  case 3:
    Disp.setFont(Arial_bold_14);
    break;
  case 4:
    Disp.setFont(Arial_Black_16_ISO_8859_1);
    break;
  case 5:    
    Disp.setFont(Arial_Black_16);
    break;

  default:
    Disp.setFont(Arial14);
    break;
  }

  }

void timeReloj(const String& hour,const String& minute,const String& second){

}

