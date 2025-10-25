float Sens=100; //mV/mT, this is 12.5 for A4 version
float Stc=0.0012; //%/Celcius
float Ta=25;
float Vq=5/2; //half of sensor input voltage
//float B1;
float B2;
float Distime[] = {};
float Vout1;
float Vout2;
//int HallPin1=A0;
//int HallPin2=A2; 
float Bact;

//******************CHANGE THIS**************************
float frequency=9;//Hz
const float flowrate=0.0074; // ml/s
//*****************************************************

const float dutyCycle=0.4;
float offTime=1000.0/frequency*(1-dutyCycle);
float onTime=1000.0/frequency*dutyCycle;

//float offTime=33;
//float onTime=12.5;

float power;
float time_on;
float time_off;
float total_time;
int  Writestatus = 200;
#define HallPin A4

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

//#include <WiFi.h>
//#include <HTTPClient.h>
//#include <HttpClient.h>

#include <ArduinoJson.h>
const char* ssid = "Fares1";
const char* password = "mariam98";

//#define STASSID "Fares1"
//#define STAPSK  "mariam98"

//Your Domain name with URL path or IP address with path
String serverName = "https://t1ly8glf7c.execute-api.us-east-1.amazonaws.com/prod-get/data"; //aws API Gateway url, Need htps:// pre-fix for ESP32 sketch
const char* API_KEY = "uzpjEVoFFz3TZQkzGS8GxRHjHAYGxN181OwDplr4"; //Your AWS API-Key

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;
unsigned long Uptime;
String json_post(float carbs);
double carbs_get=0;
double carbs_old=1000;




#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);


void setup() {
  Serial.begin(115200); 

  WiFi.begin(ssid, password);
  //WiFi.begin(STASSID, STAPSK);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  pinMode(2,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(16,OUTPUT);
  //pinMode(4,OUTPUT);
  digitalWrite(2,LOW);   
  digitalWrite(12,LOW);
  digitalWrite(14,LOW);
  digitalWrite(16,LOW); 



//#################################Display_Setup############################################
Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Welcome");

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 17);
  display.println("This is");
  display.println("a Splash");
  display.println("Screen");

  display.display();

  delay(5000);
  
}

String body;
bool flag=true;
void loop() {

  
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
     // Pump();
     
     carbs_get=json_get();
     while (carbs_get!= carbs_old){
     while (flag==true){
          carbs_get=json_get();
          Serial.println(carbs_get);
          carbs_old=carbs_get;

          String pred=json_post(carbs_get);

          Serial.println("Pred: ");
          Serial.println(pred);
          float pred_float=pred.toFloat();
      
          
      
  
      long   starttime = millis();
      long    endtime = starttime;
          //while ((endtime - starttime) <=(((pred_float/100)*60/flowrate)*1000)) // do this loop for up to 1000mS
         // while ( (endtime - starttime) <= (((10.0/100)*60/flowrate)*1000)+2000) // do this loop for up to 1000mS
         while ((endtime - starttime) <=(((pred_float/100)/flowrate)*1000)) // do this loop for up to 1000mS
            {

              Serial.println("Pumping");
              
              display.clearDisplay();

                display.setTextSize(2);
                display.setTextColor(WHITE);
                display.setCursor(0, 0);
                  
                display.println("Pumping"); 

                  display.setTextSize(1.9);
                  display.setTextColor(WHITE);
                  //display.setCursor(0, 0);
                  display.println("Consumed carbs = ");
                  display.print(carbs_get);
                  display.println(" grams ");
                  display.println("");
                  display.println(pred_float);
                  display.print("units of U100 insulin");
                  display.display();
                Pump();
                endtime = millis();
                Serial.println(endtime - starttime);
            }
          flag=false;

         

         }

      }

    }



    
}
}

double json_get(){
double carbs;
   // WiFiClient client;
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
       //HTTPClient http;
      // Your Domain name with URL path or IP address with path
      String serverPath = "https://t1ly8glf7c.execute-api.us-east-1.amazonaws.com/prod-get/data";
      http.begin(client, serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        
        //StaticJsonBuffer<200> jsonBuffer;

       //JsonObject& root = jsonBuffer.parseObject(httpResponseCode);
       //const double carbs = payload["Item"]["carbs"];
        DynamicJsonDocument doc(2048);
        DeserializationError err = deserializeJson(doc,payload);

        if (err){
          Serial.print("Error: ");
          Serial.println(err.c_str());
     }

       carbs=doc["Item"]["carbs"] ;

      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();

      return carbs;
}


String json_post(float carbs){

    // HTTPClient client;
    WiFiClientSecure client;
    client.setInsecure(); 
    HTTPClient http;
     
     http.begin(client,"https://k86fih2os8.execute-api.us-east-1.amazonaws.com/default/xgboostcontainer1");
     http.addHeader("Content-Type","application/json");

     DynamicJsonDocument doc(2048);

     JsonObject object=doc.to<JsonObject>();
     object["BG"]=100;
     object["carbs"]=carbs;
     object["bas"]=0.01393333;
     
     char jsonOutput[700];
     
     serializeJson(doc,jsonOutput);

     int httpCode=http.POST(String(jsonOutput));

     if(httpCode>0){
      
      String payload=http.getString();
      //Serial.println("\nStatuscode: " + String(httpCode));
      //Serial.println(payload);
      http.end();

      DynamicJsonDocument doc2(2048);
      DeserializationError err = deserializeJson(doc2,payload);
      if (err){
          Serial.print("Error: ");
          Serial.println(err.c_str());
     }

     
     body=doc2["body"].as<String>();
     
     //Serial.println(body);
      
      }
    int firstBracket = body.indexOf('[');
    int lastBracket = body.indexOf(']');
    String pred=body.substring(firstBracket+1, lastBracket);  


    return pred;
 
  

}




void Pump()
{


  digitalWrite(14,LOW);
  digitalWrite(16,HIGH);
//  Serial.println("CLOSED");
//  Vout1=(analogRead(HallPin1)*5.0)/1023.0;
//  B1=(Vout1-Vq)/(Sens*(1+Stc*(Ta-25)));

    
  digitalWrite(2,LOW); 
  digitalWrite(12,HIGH);
//  Vout2=(analogRead(HallPin2)*5.0)/1023.0;
//  B2=(Vout2-Vq)/(Sens*(1+Stc*(Ta-25)));
  //Serial.println(Vout1);
//  Serial.print(',');
//  Serial.println(Vout2);
  

  delay(onTime);

  digitalWrite(2,LOW); 
  digitalWrite(12,LOW);
  //Vout1=(analogRead(HallPin1)*5.0)/1023.0;
//  B1=(Vout1-Vq)/(Sens*(1+Stc*(Ta-25)));


  digitalWrite(14,HIGH);
  digitalWrite(16,LOW);
//  Serial.println("NEUTRAL");
//  Vout2=(analogRead(HallPin2)*5.0)/1023.0;
//  B2=(Vout2-Vq)/(Sens*(1+Stc*(Ta-25)));
 // Serial.println(Vout1);
//  Serial.print(',');
//  Serial.println(Vout2);
  

  delay(offTime/10);

  
  digitalWrite(2,LOW); 
  digitalWrite(12,LOW);
//  Vout1=(analogRead(HallPin1)*5.0)/1023.0;
  Bact=(Vout1-Vq)/(Sens*(1+Stc*(Ta-25)));


  digitalWrite(14,LOW);
  digitalWrite(16,LOW);
//  Serial.println("NEUTRAL");
//  Vout2=(analogRead(HallPin2)*5.0)/1023.0;
//  B2=(Vout2-Vq)/(Sens*(1+Stc*(Ta-25)));
 // Serial.println(Vout1);
//  Serial.print(',');
//  Serial.println(Vout2);
  
  

  delay((9*offTime)/10);
}
