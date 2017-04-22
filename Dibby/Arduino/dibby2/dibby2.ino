// Version v1.02 Works with the Wemos D1 board R2 http://bit.ly/WEMOS_D1

#include <ESP8266WiFi.h> 
#include <ArduinoJson.h>

#define PIN_GATE_IN 2
#define IRQ_GATE_IN  0
#define PIN_LED_OUT 13
#define PIN_ANALOG_IN_ENVELOPE A0
#define AmpMax (1024 / 2)

const char* ssid     = "OrangeHorse";      // SSID of local network
const char* password = "bunuelito";   // Password on network


const double dBAnalogQuiet = 10; // envelope - calibrated value from analog input (48 dB equivalent)
const double dBAnalogModerate = 12;
const double dBAnalogLoud = 17;


WiFiClient client;
char servername[]="10.37.1.108";  // remote server we will connect to
String result;

  float dBSum=0;
  int secs=0;
  int period = 10;


void soundISR()
{
  int pin_val;
  pin_val = digitalRead(PIN_GATE_IN);
  digitalWrite(PIN_LED_OUT, pin_val);   
}


void setup() {
  Serial.begin(115200); 
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);

   //  Configure LED pin as output
  pinMode(PIN_LED_OUT, OUTPUT);
  
  // configure input to interrupt
  pinMode(PIN_ANALOG_IN_ENVELOPE, INPUT); 
  
  attachInterrupt(IRQ_GATE_IN, soundISR, CHANGE);

  // Display status
  Serial.println("Sound Initialized");

}

void loop() {

   int audio;
  int envelope;
  float dB;
  float decibelsValueQuiet = 35;
  float decibelsValueModerate = 60;
  float decibelsValueLoud = 80;
 
  
  // Check the envelope input
  envelope = analogRead(PIN_ANALOG_IN_ENVELOPE); 


  if (envelope < 13){
    dB = decibelsValueQuiet + (20 * log10(envelope/dBAnalogQuiet));
  }
  else if ((envelope > 13) && ( envelope <= 23) )
  {
    dB = decibelsValueModerate + (20 * log10(envelope/dBAnalogModerate));
  }
  else
  {
    dB = decibelsValueLoud + (20 * log10(envelope/dBAnalogLoud));
  }
  
 
  Serial.print("dB: ");
  Serial.println(dB);

  displayGettingData();
  delay(1000);
 
  
  publishDB(dB);

}

void publishDB(float db) //client function to send/receive GET request data.
{
  if (client.connect(servername, 8080)) {  //starts client connection, checks for connection
 

  String part1 = "GET /Dibby/api/verify?arg1=" ;
  String part2 =  String(db, 3);  ;
 
 

    //client.println("GET /Dibby/api/verify?arg1=668  HTTP/1.1");
    client.println(part1+part2);
    //client.print(db);
    client.print("  HTTP/1.1");
    client.println("Host: 10.37.1.108:8080");
    client.println("Accept: */*");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
 
    client.println();
  } 
  else {
    Serial.println("connection failed"); //error message if no client connect
   // Serial.println();
  }

  while(client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
      result = result+c;
    }

  client.stop(); //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
 Serial.println(result);

 result = "";
 

}
 

void displayGettingData()
{ 
  Serial.println("Getting data");
}


