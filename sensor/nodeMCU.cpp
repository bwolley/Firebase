#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include <addons/RTDBHelper.h>

#define DATABASE_URL "test-5e8b6-default-rtdb.firebaseio.com/" 
#define DATABASE_SECRET "I2eogD1QkCGbZFCLCkKTQ9yGwZu441ErljWEiH0B"   
char chr_recv;
bool recv_flag = false, flag = false;
String str_recv = "";

unsigned long prev_time;
String str_test;
String SensorState[] = {"", "", "", "", "", "", "", "", "", "", ""};
int status = WL_IDLE_STATUS;
FirebaseData fbdo; 
FirebaseAuth auth;  
FirebaseConfig config;

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  //wm.resetSettings();  
  if (!wm.autoConnect("Firebase")) {
    
  }
  InitWiFi();
  config.database_url = DATABASE_URL;                  
  config.signer.tokens.legacy_token = DATABASE_SECRET; 
  Firebase.reconnectWiFi(true);                       
  Firebase.begin(&config, &auth);
  Serial.println(WiFi.localIP());    
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION); 
}

void loop() { 
  
  if(!flag) {
    delay(5000);
    flag = true;
  }
  if (Serial.available() > 0 ) {
    chr_recv = Serial.read();
    if (chr_recv == '@') {
      recv_flag = false;
      Split(str_recv, ',');
      str_recv = "";
      
    } else if (chr_recv == '!') {
      recv_flag = true;
    } else if (recv_flag) {
      str_recv += chr_recv;
    }
  }
}

void Split(String sData, char cSeparator) {  
  int nCount = 0, nGetIndex = 0;  
  String sTemp = "", sCopy = sData;
  while(true){
    nGetIndex = sCopy.indexOf(cSeparator); 
    if(-1 != nGetIndex){    
      sTemp = sCopy.substring(0, nGetIndex);
      sCopy = sCopy.substring(nGetIndex + 1);
      if (nCount == 1) { 
      SensorState[0] = sTemp;
      } else if(nCount == 2){ 
        SensorState[1] = sTemp;
      } else if(nCount == 3){
        SensorState[2] = sTemp;
      } else if(nCount == 4){
        SensorState[3] = sTemp;
      } else if(nCount == 5){  
        SensorState[4] = sTemp;
      } else if(nCount == 6){
        SensorState[5] = sTemp;
      } 
    } else {
      if(Firebase.RTDB.pushString(&fbdo, "/Temp", SensorState[0] ) == true );
      if(Firebase.RTDB.pushString(&fbdo, "/Humi", SensorState[1] ) == true );
      if(Firebase.RTDB.pushString(&fbdo, "/CDS", SensorState[2] ) == true);
      if(Firebase.RTDB.pushString(&fbdo, "/Volt", SensorState[3] ) == true);
      if(Firebase.RTDB.pushString(&fbdo, "/mA", SensorState[4] ) == true);
      if(Firebase.RTDB.pushString(&fbdo, "/Dust", SensorState[5] ) == true);
      break;
    }    
    ++nCount;
  }
}

void InitWiFi() {
  Serial.println("Connecting to AP ...");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println("Connected to AP");
}
