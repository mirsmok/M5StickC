#include <M5StickC.h>
#include <WiFi.h>
#include <Preferences.h>
Preferences preferences;

////AP definitions
//#define AP_SSID "FunBox2-5751"
//#define AP_PASSWORD "E57D71222F4916E965F4D57C3E"


//#define AP_SSID "Xperia Z2_2993"
//#define AP_PASSWORD "mirsmok82"

#define AP_SSID "NETIASPOT-C61350"
#define AP_PASSWORD "wiktorpatryk"


//thinkspeak.com settings
const char* host = "api.thingspeak.com";
//const int channelID = X
String writeAPIKey = "GVU85RRPYXGN35LS"; // write API key for your ThingSpeak Channel
static int interruptCounter=0;

  unsigned int counter ;//= preferences.getUInt("counter", 0);


void setup() {
  
  
  // Initialize the M5StickC object
  //M5.begin();
  
//  M5.Axp.begin();
  
  preferences.begin("my-app", false);  
  counter = preferences.getUInt("counter", 0);
  counter++;
  preferences.putUInt("counter", counter);
  preferences.end();
  // LCD display 
//  M5.Lcd.print("Hello World  :");
//  M5.Lcd.print(counter);
/*  
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("WILGOC :");
  M5.Lcd.setCursor(0, 15);
  pinMode(26,OUTPUT); 
  digitalWrite(26,HIGH); */
}

void loop() {
   if (counter > 39 ){ 
    //M5.begin();
    //M5.Axp.ScreenBreath(7);
    //M5.Lcd.fillScreen(BLACK);
    sendResults();
    preferences.begin("my-app", false);  
    preferences.putUInt("counter", 0);
    preferences.end();
    //M5.Lcd.fillScreen(BLACK);
   }
  M5.Axp.DeepSleep(SLEEP_SEC(30));
 
//  M5.Axp.SetSleep() ;
/*
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.println(analogRead(36));
  delay(1000);*/
  
}

int wifiConnect()
{
///    Serial.print("Connecting to AP");
    WiFi.begin(AP_SSID,AP_PASSWORD);
    int connectAttemps=0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
///      Serial.print(".");
      connectAttemps++;
      if(connectAttemps > 10){
///        Serial.println("Can't connect to Wifi go to Sleep for 20 minuts...");
       // M5.Axp.DeepSleep(SLEEP_SEC(600));
       return 0;
        }
    }  
///  Serial.println("");
///  Serial.println("WiFi connected");  
}

void sendResults()
{  
   WiFiClient client;
   wifiConnect(); 
   while(!client.connect(host, 80)) {
    wifiConnect(); 
  }
 double vbat = 0.0;
int discharge,charge;
double temp = 0.0;
double bat_p = 0.0;


  vbat      = M5.Axp.GetVbatData() * 1.1 / 1000;
  vbat      = ((vbat -3.0)/1.2)*100;
  charge    = M5.Axp.GetIchargeData() / 2;
  discharge = M5.Axp.GetIdischargeData() / 2;
  temp      =  -144.7 + M5.Axp.GetTempData() * 0.1;
  bat_p     =  M5.Axp.GetPowerbatData() * 1.1 * 0.5 /1000;

// thinkSpeak
  
  /*  M5.Lcd.setCursor(0, 0, 1);  
    M5.Lcd.printf("vbat:%.3fV\r\n",vbat);  //battery voltage
  M5.Lcd.printf("icharge:%dmA\r\n",charge);  //battery charging current
  M5.Lcd.printf("idischg:%dmA\r\n",discharge);  //battery output current
  M5.Lcd.printf("temp:%.1fC\r\n",temp);  //axp192 inside temp
  M5.Lcd.printf("pbat:%.3fmW\r\n",bat_p);  //battery power
  M5.Lcd.printf("CoIn :%d\r\n",M5.Axp.GetCoulombchargeData());  //coulomb in current
  M5.Lcd.printf("CoOut:%d\r\n",M5.Axp.GetCoulombdischargeData());  //coulomb out current
  M5.Lcd.printf("CoD:%.2fmAh\r\n",M5.Axp.GetCoulombData());  //value of coulomb after calculate
  M5.Lcd.printf("Vin:%.3fmV\r\n",M5.Axp.GetVinData() * 1.7);  // ACIN vin voltage
  M5.Lcd.printf("Iin:%.3fmA\r\n",M5.Axp.GetIinData() * 0.625);  // ACIN vin current
  M5.Lcd.printf("Vuin:%.3fmV\r\n",M5.Axp.GetVusbinData() * 1.7);  //usb vin voltage
  M5.Lcd.printf("Iuin:%.3fmA\r\n",M5.Axp.GetIusbinData() * 0.375);  // usb vin current*/
 String body = "api_key="+writeAPIKey + "&field1=";
           body += String(vbat);
           body += String("&field2=");
           body += String(charge);
           body += String("&field3=");
           body += String(discharge);
           body += String("&field4=");
           body += String(temp);
           body += String("&field5=");
           body += String(bat_p);
    client.print("GET https://api.thingspeak.com/update?"+body);//api_key=GVU85RRPYXGN35LS&field1=0");
    client.print("\nHost: api.thingspeak.com\n");
   /* client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(body.length());
    client.print("\n\n");
    client.print(body);
    client.print("\n\n");
    */
    Serial.println(body);

  delay(2000);
  while(client.available()){
    String line = client.readStringUntil('\r');
  //  Serial.print(line);
  }
  client.flush();
  client.stop();
///  Serial.println();
///  Serial.println("Connection closed");    

}
