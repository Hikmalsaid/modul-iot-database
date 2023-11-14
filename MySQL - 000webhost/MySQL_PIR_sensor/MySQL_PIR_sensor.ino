#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const int sensePin = 23; // the number of the tilt switch pin
boolean senseState = 0; // variable for reading the tilt switch status

// Replace with your network credentials
const char* ssid     = "m";
const char* password = "11112222";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://overhanging-governm.000webhostapp.com/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

String sensorName = "PIR Sensor";
String sensorLocation = "BB202";

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

    // initialize the tilt switch pin as an input:
  pinMode(sensePin, INPUT); 
}

void loop() {
  
  senseState = digitalRead(sensePin);
  String senseStat = "Gak_ada_Gerakan";
  
   if (senseState == 1)
    {
    senseStat.replace("Gak_ada_Gerakan", "Gerak");
    }

  if(WiFi.status()== WL_CONNECTED){
    WiFiClientSecure *client = new WiFiClientSecure;
    client->setInsecure(); //don't use SSL certificate
    HTTPClient https;
    
    // Your Domain name with URL path or IP address with path
    https.begin(*client, serverName);
    
    // Specify content-type header
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName + "&location=" + sensorLocation + "&value1=" + senseStat +  "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = https.POST(httpRequestData);
   
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    https.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 10 seconds
  delay(3000);  
}