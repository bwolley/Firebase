#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include <addons/RTDBHelper.h>

#define DATABASE_URL "@YOUR_RTDB_URL@" 
#define DATABASE_SECRET "@YOUR_RTDB_SECRET@"   
char chr_recv;
bool recv_flag = false;
String payload = "";
String str_recv = "";
FirebaseData fbdo; 
FirebaseAuth auth;  
FirebaseConfig config; 

String str_test;
unsigned long lastSend, prev_time, prev_time2, prev_time3, prev_time4, prev_time5;
String SensorState[] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  wm.resetSettings();
  bool res;
  res = wm.autoConnect("Firebase");
  if(!res) {
    Serial.println("Failed to connect");
  } else {
    Serial.println("connected...yeey :)");
  }
  InitWiFi();

  Serial.println(WiFi.localIP());    
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION); 
  config.database_url = DATABASE_URL;                  
  config.signer.tokens.legacy_token = DATABASE_SECRET; 
  Firebase.reconnectWiFi(true);                       
  Firebase.begin(&config, &auth);                     
}

void loop() {
  if (Serial.available() > 0 ) {
    chr_recv = Serial.read();
    if (chr_recv == '@') {
      recv_flag = false;
      Split(str_recv, ',');
    } else if (chr_recv == '!') {
      payload = "{";
      recv_flag = true;
    } else if (recv_flag) {
      str_recv += chr_recv;
    }
  }
}

void Split(String sData, char cSeparator){  
  int nCount = 0, nGetIndex = 0;  
  String sTemp = "", sCopy = sData;
  while(true){
    nGetIndex = sCopy.indexOf(cSeparator); 
    if(-1 != nGetIndex){    
      sTemp = sCopy.substring(0, nGetIndex);
      sCopy = sCopy.substring(nGetIndex + 1);

      if (nCount == 1) {
      payload += sTemp;
      SensorState[0] = sTemp;
      } else if(nCount == 2){
        payload += sTemp;
        SensorState[1] = sTemp;
      } else if(nCount == 3){
        payload += ",";
        payload += sTemp;
        SensorState[2] = sTemp;
      } else if(nCount == 4){
        payload += sTemp;
        SensorState[3] = sTemp;
      } else if(nCount == 5){  
        payload += ",";
        payload += sTemp;
        SensorState[4] = sTemp;
      } else if(nCount == 6){
        payload += sTemp;
        SensorState[5] = sTemp;
      } else if(nCount == 7){
        payload += sTemp;
        SensorState[6] = sTemp;
      } else if(nCount == 8){
        payload += sTemp;
        SensorState[7] = sTemp;
      } else if(nCount == 9){
        payload += sTemp;
        SensorState[8] = sTemp;
      } else if(nCount == 10){
        payload += sTemp;
        SensorState[9] = sTemp;
      } else if(nCount == 11){
        payload += sTemp;
        SensorState[10] = sTemp;
      } else if(nCount == 12){
        payload += sTemp;
        SensorState[11] = sTemp;
      } 
    } else {
      payload += "}";

      if(Firebase.RTDB.setString(&fbdo,"/Temperature", SensorState[1] ) == true);
      else  Serial.println(fbdo.errorReason().c_str());
      if(Firebase.RTDB.setString(&fbdo,"/Humidity", SensorState[3] ) == true);
      else  Serial.println(fbdo.errorReason().c_str());
      if(Firebase.RTDB.setString(&fbdo,"/CDS", SensorState[5] ) == true);
      else  Serial.println(fbdo.errorReason().c_str());
      if(Firebase.RTDB.setString(&fbdo,"/Volt", SensorState[7] ) == true);
      else  Serial.println(fbdo.errorReason().c_str());
      if(Firebase.RTDB.setString(&fbdo,"/mA", SensorState[9] ) == true);
      else  Serial.println(fbdo.errorReason().c_str());
      if(Firebase.RTDB.setString(&fbdo,"/Dust", SensorState[11] ) == true);
      else  Serial.println(fbdo.errorReason().c_str());

      str_recv = "";
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
