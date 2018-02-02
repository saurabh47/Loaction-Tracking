
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

#define PIN_TX    10
#define PIN_RX    11

#define PHONE_NUMBER "9960946502"  

char MESSAGE[300];

char lat[12];
char lon[12];
char wspeed[12];

boolean flag=false;


SoftwareSerial mySerial(PIN_TX,PIN_RX);
DFRobot_SIM808 sim808(&mySerial);//Connect RX,TX,PWR,

//DFRobot_SIM808 sim808(&Serial);

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  //******** Initialize sim808 module *************
  while(!sim808.init()) { 
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }

  //************* Turn on the GPS power************
  if( sim808.attachGPS())
      Serial.println("Open the GPS power success");
  else 
      Serial.println("Open the GPS power failure");
  
}

void loop() {
   //************** Get GPS data *******************
   if (sim808.getGPS()) {
    Serial.print(sim808.GPSdata.year);
    Serial.print("/");
    Serial.print(sim808.GPSdata.month);
    Serial.print("/");
    Serial.print(sim808.GPSdata.day);
    Serial.print(" ");
    Serial.print(sim808.GPSdata.hour);
    Serial.print(":");
    Serial.print(sim808.GPSdata.minute);
    Serial.print(":");
    Serial.print(sim808.GPSdata.second);
    Serial.print(":");
    Serial.println(sim808.GPSdata.centisecond);
    Serial.print("latitude :");
    Serial.println(sim808.GPSdata.lat);
    Serial.print("longitude :");
    Serial.println(sim808.GPSdata.lon);
    Serial.print("speed_kph :");
    Serial.println(sim808.GPSdata.speed_kph);
    Serial.print("heading :");
    Serial.println(sim808.GPSdata.heading);
    Serial.println();


  float la = sim808.GPSdata.lat;
  float lo = sim808.GPSdata.lon;
  float ws = sim808.GPSdata.speed_kph;

  dtostrf(la, 4, 6, lat); //put float value of la into char array of lat. 4 = number of digits before decimal sign. 6 = number of digits after the decimal sign.
  dtostrf(lo, 4, 6, lon); //put float value of lo into char array of lon
  dtostrf(ws, 6, 2, wspeed);  //put float value of ws into char array of wspeed

  sprintf(MESSAGE, "Latitude : %s\nLongitude : %s\nWind Speed : %s kph\nSIM808 Module Is Working. To get My Location Try With This Link.\nhttp://maps.google.com/maps?q=%s,%s\n-saurabh", lat, lon, wspeed, lat, lon);

    //TO send SMS only once
    if(flag ==false){
      sendSMS();
      flag=true;
    }

    //************* Turn off the GPS power ************
    sim808.detachGPS();
  }

}

void sendSMS()
{
  Serial.println("Start to send message ...");
  
  Serial.println(MESSAGE);
  
  sim808.sendSMS(PHONE_NUMBER,MESSAGE);
}

