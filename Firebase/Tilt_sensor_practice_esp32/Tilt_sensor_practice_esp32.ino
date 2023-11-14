const int sigPin = 2; // the number of the tilt switch pin
boolean sigState = 0; // variable for reading the tilt switch status

#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "m"
#define WIFI_PASSWORD "11112222"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyDRZxXa-VNZ5dBAaiZcmHy42sZGvb4wn_Q"

/* 3. Define the RTDB URL */
#define DATABASE_URL "nyoba-atas-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "hikmal.saidfar1@gmail.com"
#define USER_PASSWORD "hijikata678"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //insialisasi WiFi
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) //menghubungkan ESP ke WiFi
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  Firebase.begin(&config, &auth);

// initialize the tilt switch pin as an input:
  pinMode(sigPin, INPUT); 

}

void loop() {
  
   sigState = digitalRead(sigPin);
  
   if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    
    if (sigState == HIGH)
    {
    Serial.printf("Set digital... %s\n", Firebase.setString(fbdo, F("/test/kondisi_digital"), "Terdeteksi") ? "ok" : fbdo.errorReason().c_str());    
    }else
    {
    Serial.printf("Set digital... %s\n", Firebase.setString(fbdo, F("/test/kondisi_digital"), "Tidak Terdeteksi") ? "ok" : fbdo.errorReason().c_str());    
    }
    
    FirebaseJson json;

  }
}
