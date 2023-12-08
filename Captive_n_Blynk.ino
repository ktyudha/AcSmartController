#define BLYNK_TEMPLATE_ID "TMPL6P7V-Xi6i"
#define BLYNK_TEMPLATE_NAME "AC Smart Controller JJ308"
#define BLYNK_AUTH_TOKEN "t6P6lACJZS3owlYsAq6oFKIS-ZuGTOf7"


#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h> //Jika protocol tidak terdeteksi
#include <ir_Panasonic.h> //Protocol Panasonic (lihat library untuk protocol remote lain)
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#define DHTPIN D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);


const char* ssid        = "eepiswlan";      //nama wifi
const char* password    = "7t3aec7x";  

String user_id       = "zakypratamasamdyayuan@te.student.pens.ac.id";    //id wifi_id
String pass_id       = "SuperNichiya333";    //pass wifi_id
String ipgate        = "";                  //ini gak wajib, ini ip yang pernah terknonek ke router ke modem telkom
String induk         = "WAG-D7-BAL";        //ini wajib, ini nama kota box yang ada di luar jalan
String setmac        = "";                  //ini gak wajib, ini mac router kamu yang konek ke bot auto konek
String wlan          = "";                  //ini gak wajib, ini kalau gak salah nama box kota yang ada di tiang
const char* hostName = "www.google.com";    //ini ping buat cek jaringan
bool cekwifi         = true;        


//Pin IRLed TX
const uint16_t kIrLed = 4; //D2 - GPIO4

// #define kIrLed D2

int pushMode = 0;
int pushFan = 0;
int pushSwing = 0;

int togglePower = 0;
int toggleMode = 0;
int toggleFan = 0;
int toggleSwing = 0;
int toggleECO = 0;

int temp = 16;

int notifMode,notifFan,notifSwing,notifECO;

// Set the GPIO used for sending messages.
IRPanasonicAc ac(kIrLed);  
IRsend irsend(kIrLed);

WidgetLCD lcd(V7);

//ECO SMART ON
// uint16_t rawEcoOn[263] = {3414, 1782,  392, 502,  368, 1372,  368, 502,  368, 476,  392, 476,  392, 476,  392, 502,  368, 478,  392, 504,  366, 502,  366, 502,  366, 502,  368, 502,  366, 1372,  366, 504,  366, 502,  368, 478,  392, 502,  366, 502,  366, 478,  392, 476,  392, 1372,  368, 1348,  392, 1372,  366, 504,  366, 502,  368, 1348,  392, 478,  392, 502,  368, 504,  366, 478,  392, 476,  392, 502,  368, 502,  368, 502,  368, 502,  368, 502,  368, 502,  368, 504,  366, 502,  368, 502,  366, 502,  368, 476,  392, 502,  366, 502,  368, 502,  366, 502,  368, 502,  366, 502,  368, 502,  366, 478,  390, 502,  368, 502,  366, 502,  368, 476,  394, 502,  366, 478,  392, 1348,  392, 1372,  368, 478,  392, 502,  368, 502,  368, 502,  368, 502,  366, 10428,  3440, 1782,  392, 476,  392, 1374,  366, 500,  368, 478,  392, 476,  392, 478,  392, 502,  366, 502,  368, 502,  366, 502,  366, 502,  368, 502,  368, 502,  366, 1372,  366, 502,  368, 502,  366, 476,  394, 502,  368, 476,  392, 504,  366, 502,  368, 1372,  366, 1372,  368, 1372,  368, 502,  368, 502,  366, 1348,  392, 504,  366, 478,  392, 502,  366, 502,  368, 502,  368, 502,  366, 504,  366, 502,  368, 502,  368, 478,  392, 502,  366, 502,  366, 1348,  392, 502,  368, 1346,  392, 1372,  366, 1372,  368, 504,  366, 476,  394, 502,  368, 502,  368, 502,  366, 1372,  366, 478,  392, 502,  366, 1346,  392, 1348,  392, 1348,  392, 1372,  368, 502,  366, 1348,  392, 1346,  394, 502,  366, 502,  368, 502,  366, 478,  392, 1372,  366};
//ECO SMART OFF
uint16_t rawEcoOff[263] = {3386, 1782,  392, 502,  366, 1350,  390, 502,  366, 502,  368, 502,  366, 504,  366, 502,  366, 504,  366, 502,  368, 502,  366, 502,  366, 504,  366, 504,  366, 1372,  366, 478,  392, 502,  366, 504,  366, 478,  392, 478,  392, 502,  366, 504,  366, 1350,  390, 1348,  392, 1348,  392, 504,  366, 504,  366, 1372,  366, 502,  368, 476,  392, 476,  394, 502,  368, 502,  366, 502,  366, 502,  368, 476,  392, 502,  366, 478,  392, 502,  366, 502,  368, 502,  368, 502,  368, 502,  366, 504,  366, 502,  366, 502,  366, 476,  394, 502,  366, 478,  390, 476,  394, 502,  368, 502,  368, 502,  366, 502,  368, 502,  366, 478,  392, 478,  392, 504,  366, 1372,  366, 1372,  366, 504,  366, 502,  366, 502,  366, 502,  366, 476,  392, 10430,  3440, 1782,  392, 502,  366, 1374,  366, 502,  368, 476,  392, 476,  394, 476,  392, 502,  366, 502,  366, 504,  366, 502,  368, 502,  366, 502,  366, 478,  392, 1372,  368, 476,  392, 502,  366, 478,  392, 502,  366, 502,  366, 502,  366, 504,  366, 1372,  368, 1348,  390, 1348,  392, 476,  392, 504,  366, 1374,  366, 502,  368, 502,  366, 502,  368, 476,  392, 478,  392, 478,  390, 504,  366, 478,  392, 476,  392, 502,  368, 502,  368, 502,  366, 1348,  390, 478,  392, 1372,  368, 1374,  366, 1372,  368, 502,  366, 502,  368, 502,  366, 502,  368, 504,  366, 1372,  368, 502,  368, 502,  368, 1346,  392, 1374,  366, 1372,  368, 1348,  390, 478,  392, 1346,  392, 1372,  366, 504,  366, 504,  366, 502,  366, 504,  366, 1348,  392};

void setup() 
{
  ac.begin();
  irsend.begin();

  Serial.begin(115200);
  toCaptive();
  lcdState();
  Blynk.virtualWrite(V8, temp);
}

void getDHT() {
float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal membaca dari sensor DHT");
    return;
  }

  Serial.print("Kelembaban: ");
  Serial.print(h);
  Serial.print("%\t");
  Serial.print("Suhu: ");
  Serial.print(t);
  Serial.println("°C");
 Blynk.virtualWrite(V9, t);
  delay(2000);
}

void toCaptive(){
   Serial.println("Bot mencoba masuk ke ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Konek ke wifi stabil dengan detail: ");
  Serial.println("IP: " + String(WiFi.localIP().toString()) + " | IP GW: " + String(WiFi.gatewayIP().toString()) + " | MAC GW: " + WiFi.BSSIDstr() + " | RSSI: " + String(WiFi.RSSI()) + " | MAC: " + String(WiFi.macAddress().c_str()));
  if (cekwifi) {
      Serial.println("Mulai cek login!!!");
      String cek = UrlGet("https://iac2.pens.ac.id:8003/index.php?zone=eepiswlan&redirurl=http%3A%2F%2Fwww.gstatic.com%2Fgenerate_204","auth_user=" + user_id + "&auth_pass=" + pass_id + "&accept=Login" );
      if (cek == "-1") {
        Serial.println("Salah Config Coba Tanya Mbak Google");
        delay(60000);
      } else {
        // Serial.println("Indihomonya lagi jelek");
        Serial.println(cek);
        Serial.println("ini hasilnya");
       Serial.println(UrlGet("https://iac2.pens.ac.id:8003/index.php?zone=eepiswlan", ""));
       Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
       delay(5000);
      }
    } else {
      Serial.println("Lapor pak RT Internetnya mati pak");      
      delay(20000);
    }
}

void loop() 
{
 
  getDHT();
  delay(1000);
  Blynk.run();
}

String UrlGet(String hostGet, String data) {
  int httpCode = 0;
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
  if (https.begin(*client, hostGet)) {
    https.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36");
    Serial.println("Curl....");
    if (data == "") {
      httpCode = https.GET();
    } else {
      https.addHeader("Content-Type", "application/x-www-form-urlencoded");
      httpCode = https.POST(data);
    }
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = https.getString();
        https.end();
        return payload;
      }
    } else {
      Serial.printf("Curl failed, error: %s\n\r", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.printf("Curl unable to connect\n\r");
  }
  return String(httpCode);
}

//Widget Power Button(Switch)
BLYNK_WRITE(V0)
{
  Serial.println("Power");
  togglePower = param.asInt();
  if(togglePower==1)
  {
   
    ac.on();
    ac.setSwingVertical(true);
    ac.setSwingHorizontal(false);
    ac.setTemp(temp);
    Blynk.virtualWrite(V8, temp);
    
    // Now send the IR signal.
    #if SEND_PANASONIC_AC
      ac.send();
    #endif 
    delay(2000);   
  }
  else
  {
    ac.off();
 
    // Now send the IR signal.
    #if SEND_PANASONIC_AC
      ac.send();
    #endif 
    delay(2000);   
  }
  
}

//Widget Button ECO SMART(Switch)
// BLYNK_WRITE(V1)
// {
//   toggleECO = param.asInt();
//   if(toggleECO == 1)
//   {
//     #if SEND_RAW
//      irsend.sendRaw(rawEcoOn, 263, 38);  // Send a raw data capture at 38kHz.
//     #endif  //SEND_RAW
//     lcd.print(11,0, "E-ON ");
//     delay(2000);  
//   }
//   else
//   {
//     #if SEND_RAW
//      irsend.sendRaw(rawEcoOff, 263, 38);  // Send a raw data capture at 38kHz.
//     #endif  // SEND_RAW
//     lcd.print(11,0, "E-OFF");
//     delay(2000);     
//   }
// }

//Widget Button MODE(Push)
// BLYNK_WRITE(V2)
// {
//   toggleMode = param.asInt();
//   if(toggleMode == 1)
//   {
//     pushMode++;
//     delay(200);
//     if (pushMode>2)
//     {
//       pushMode=0;
//     }
//     if(pushMode==1)
//     {
//       notifMode=0; //Mode Auto
//       lcd.print(5, 0, "AUTO");
//       //ac.setMode(0);
//     }
//     else if(pushMode==2)
//     {
//       notifMode=2; //Mode Dry
//       lcd.print(5, 0, "DRY ");
//       //ac.setMode(2);
//     }
//     else
//     {
//       notifMode=3; //Mode Cool
//       lcd.print(5, 0, "COOL");
//       //ac.setMode(3);
//     }
//     ac.setMode(notifMode);
   
//     // Now send the IR signal.
//     #if SEND_PANASONIC_AC
//       ac.send();
//     #endif  
//   }
// }

//Widget Button Fan Speed(Push)
// BLYNK_WRITE(V3)
// {
//   toggleFan = param.asInt();
//   if(toggleFan == 1)
//   {
//     pushFan++;
//     delay(200);
//     if (pushFan>3)
//     {
//       pushFan=0;
//     }
//     if(pushFan==1)
//     {
//       notifFan=0; //Quiet
//       lcd.print(4, 1, "-   ");
//       //ac.setFan(0);
//     }
//     else if(pushFan==2)
//     {
//       notifFan=2; //Medium
//       lcd.print(4, 1, "--  ");
//       //ac.setFan(2);
//     }
//     else if(pushFan==3)
//     {
//       notifFan=3; //Max
//       lcd.print(4, 1, "--- ");
//       //ac.setFan(3);
//     }
//     else
//     {
//       notifFan=7; //Auto
//       lcd.print(4, 1, "AUTO");
//       //ac.setFan(7);
//     }
//     ac.setFan(notifFan);
 
//     // Now send the IR signal.
//     #if SEND_PANASONIC_AC
//       ac.send();
//     #endif  
//   }
// }

//Widget Button AirSwingV(Push)
// BLYNK_WRITE(V4)
// {
//   toggleSwing = param.asInt();
//   if(toggleSwing == 1)
//   {
//     pushSwing++;
//     delay(200);
//     if (pushSwing>5)
//     {
//       pushSwing=0;
//     }
//     if(pushSwing==1)
//     {
//       notifSwing=1; //Highest
//       lcd.print(9,1, "Highest");
//     }
//     else if(pushSwing==2)
//     {
//       notifSwing=2; //High
//       lcd.print(9,1, "   High");
//     }
//     else if(pushSwing==3)
//     {
//       notifSwing=3; //Midle
//       lcd.print(9,1, "  Midle");
//     }
//     else if(pushSwing==4)
//     {
//       notifSwing=4; //Low
//       lcd.print(9,1, "    Low");
//     }
//     else if(pushSwing==5)
//     {
//       notifSwing=5; //Lowest
//       lcd.print(9,1, " Lowest");
//     }
//     else
//     {
//       notifSwing=15; //Auto
//       lcd.print(9,1, "  AUTO ");
//     }
//     ac.setSwingVertical(notifSwing);
 
//     // Now send the IR signal.
//     #if SEND_PANASONIC_AC
//       ac.send();
//     #endif  
//   }
// }

//Widget Button TempUp(Push)
BLYNK_WRITE(V5)
{
  int tempUp = param.asInt();
  Serial.println("Up");
  if(tempUp==1)
  {
    temp++;
    delay(200);
    if(temp>30)
    {
      temp=30;
    }
    
    // Now send the IR signal.
    #if SEND_PANASONIC_AC
      ac.send();
    #endif 
    Blynk.virtualWrite(V8, temp);
  }
  
}

//Widget Button TempDown(Push)
BLYNK_WRITE(V6)
{
  int tempDown = param.asInt();
  Serial.println("Down");
  if(tempDown==1)
  {
    temp--;
    delay(200);
    if(temp<16)
    {
      temp=16;
    }
    
    // Now send the IR signal.
    #if SEND_PANASONIC_AC
      ac.send();
    #endif 
    Blynk.virtualWrite(V8, temp);
  }  
}

//Widget LCD Display
void lcdState() 
{
  lcd.clear();
  lcd.print(0, 0, "Mode:");
  lcd.print(5, 0, "COOL");
  lcd.print(11,0, "E-OFF");
  lcd.print(0, 1, "Fan:");
  lcd.print(4, 1, "AUTO");
  lcd.print(9, 1, "AUTO  ");  
}
