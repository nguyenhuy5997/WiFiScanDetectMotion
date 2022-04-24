/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
#include "WiFi.h"
typedef struct
{
  char MAC[20];
  int32_t rssi;
}AP_WiFi;

AP_WiFi WiFi_Buff[20];
int wifi_cnt;
bool DAST = false;
bool DASP = false;
void setup()
{
    Serial.begin(115200);
    pinMode(4, OUTPUT); 
    Serial.println("Setup done");
}

void loop()
{
  
    Serial.println("scan start");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    Serial.println("-----------Current wifi scan------------"); 
    for (int i = 0; i < n; i++) 
    {
      Serial.print(WiFi.BSSIDstr(i).c_str());
      Serial.print("-----------");
      Serial.print(WiFi.RSSI(i));    
      Serial.print("-----------");
      Serial.println(WiFi.SSID(i));      
    }
    Serial.println("-----------Last wifi scan------------");
    for(int i = 0; i < wifi_cnt; i++)
    {
       Serial.print(WiFi_Buff[i].MAC);
       Serial.print("-----------");
       Serial.println(WiFi_Buff[i].rssi);         
    }
    Serial.println("\r\n--------Check 1-------------");
    bool skip = false;
    for(int i = 0; i < wifi_cnt; i++)
    {
      Serial.print("Last wifi["); Serial.print(i); Serial.print("]:"); 
      Serial.print(WiFi_Buff[i].MAC);
      Serial.print("-----------");
      Serial.println(WiFi_Buff[i].rssi);  
      if(skip == true) break;
      if(WiFi_Buff[i].rssi > -80)
      {
        for(int j = 0; j < n; j++)
        {
          Serial.print("Current wifi["); Serial.print(j); Serial.print("]:"); 
          Serial.print(WiFi.BSSIDstr(j).c_str());
          Serial.print("-----------");
          Serial.println(WiFi.RSSI(j)); 
          if(strstr(WiFi_Buff[i].MAC, WiFi.BSSIDstr(j).c_str()))
          {
            if(WiFi_Buff[i].rssi - WiFi.RSSI(j) > 10)
            {
               Serial.println("Check 1 set DAST = True");
//             skip = true;
               DAST = true;
            }
            else
            {
                DAST = false;
            }
            break;
          }
          if(j == n -1)
          {
            Serial.println("Check 1 set DAST = False");
//          skip = true;
            DAST = false;
            break;
          }
        }
      }
    }
//    Serial.println("--------Check 2-------------");
//    skip = false;
//    for(int i = 0; i < n; i++)
//    {
//      if(skip == true) break;
//      Serial.print("Current wifi["); Serial.print(i); Serial.print("]:"); 
//      Serial.print(WiFi.BSSIDstr(i).c_str());
//      Serial.print("-----------");
//      Serial.println(WiFi.RSSI(i));
//      if(WiFi.RSSI(i) > -80)
//      {
//        for(int j = 0; j < wifi_cnt; j++)
//        {
//          Serial.print("Last wifi["); Serial.print(j); Serial.print("]:"); 
//          Serial.print(WiFi_Buff[j].MAC);
//          Serial.print("-----------");
//          Serial.println(WiFi_Buff[j].rssi);
//          if(strstr(WiFi.BSSIDstr(i).c_str(), WiFi_Buff[j].MAC))
//          {
//             break;
//          }
//          if(j == wifi_cnt - 1)
//          {
//             Serial.println("Check 2 set DAST = true");
//             skip = true;
//             DAST = true;
//             break;
//          }
//        }
//      }
//    }
    Serial.println("\r\n--------Check 3-------------");
    int sub_wifi_cnt = wifi_cnt/2; 
    int cur_wifi_dis = 0;
    for(int i = 0; i < wifi_cnt; i++)
    {
      Serial.print("Last wifi["); Serial.print(i); Serial.print("]:"); 
      Serial.print(WiFi_Buff[i].MAC);
      Serial.print("-----------");
      Serial.println(WiFi_Buff[i].rssi);
      for(int j = 0; j < n; j++)
      {
        Serial.print("Current wifi["); Serial.print(j); Serial.print("]:"); 
        Serial.print(WiFi.BSSIDstr(j).c_str());
        Serial.print("-----------");
        Serial.println(WiFi.RSSI(j)); 
        if(strstr(WiFi_Buff[i].MAC, WiFi.BSSIDstr(j).c_str())) break;
        else if( j == n -1)
        {
          cur_wifi_dis++;
        }
      }
    }
    if(cur_wifi_dis >= sub_wifi_cnt)
    {
      Serial.print("cur_wifi_dis:");
      Serial.println(cur_wifi_dis);
      Serial.print("sub_wifi_cnt:");
      Serial.println(sub_wifi_cnt);
      DAST = true;
    }
    else
    {
      Serial.print("cur_wifi_dis:");
      Serial.println(cur_wifi_dis);
      Serial.print("sub_wifi_cnt:");
      Serial.println(sub_wifi_cnt);
    }
    
    for (int i = 0; i < n; i++) {
        wifi_cnt = n;
        strcpy(WiFi_Buff[i].MAC, WiFi.BSSIDstr(i).c_str());
        WiFi_Buff[i].rssi = WiFi.RSSI(i);       
        delay(10);
    }
    // Wait a bit before scanning again
    if(DAST == true)
    {
      DAST = false;
      Serial.println("DAST = true");
      digitalWrite(4, HIGH);
    }
    else
    {
      Serial.println("DAST = false");
      digitalWrite(4, LOW);
    }
    WiFi.scanDelete();
    delay(1000);
}
