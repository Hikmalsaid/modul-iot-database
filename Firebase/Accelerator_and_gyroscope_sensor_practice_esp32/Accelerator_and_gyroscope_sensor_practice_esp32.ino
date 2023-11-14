
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

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

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

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
   mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  float aks_x = a.acceleration.x;
  float aks_y = a.acceleration.y;
  float aks_z = a.acceleration.z;
  
  float gyro_x = g.gyro.x;
  float gyro_y = g.gyro.y;
  float gyro_z = g.gyro.z;
  
  Serial.println("");
  delay(500);

    Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/test/akselerasi/x"), aks_x) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get float... %s\n", Firebase.getFloat(fbdo, F("/test/akselerasi/x")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
    
    Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/test/akselerasi/y"), aks_y) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get float... %s\n", Firebase.getFloat(fbdo, F("/test/akselerasi/y")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
    
    Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/test/akselerasi/z"), aks_z) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get float... %s\n", Firebase.getFloat(fbdo, F("/test/akselerasi/z")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/test/gyro/x"), gyro_x) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get float... %s\n", Firebase.getFloat(fbdo, F("/test/gyro/x")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
    
    Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/test/gyro/y"), gyro_y) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get float... %s\n", Firebase.getFloat(fbdo, F("/test/gyro/y")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
    
    Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/test/gyro/z"), gyro_z) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get float... %s\n", Firebase.getFloat(fbdo, F("/test/gyro/z")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    FirebaseJson json;
}
