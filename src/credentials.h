/** Load WLAN credentials from EEPROM */
// char mainSSID[33] = "";
// char mainPASSWD[65] = "";
// #include <Arduino.h>
// #include <cstring>
// #include <EEPROM.h>

void loadCredentials() {
  EEPROM.begin(512);
  EEPROM.get(0, mainSSID);
  EEPROM.get(0 + sizeof(mainSSID), mainPASSWD);
  char ok[2 + 1];
  EEPROM.get(0 + sizeof(mainSSID) + sizeof(mainPASSWD), ok);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    mainSSID[0] = 0;
    mainPASSWD[0] = 0;
  }
  Serial.println("Recovered credentials:");
  Serial.println(mainSSID);
   Serial.println(mainPASSWD);
  Serial.println(strlen(mainPASSWD) > 0 ? "********" : "<no password>");
}

/** Store WLAN credentials to EEPROM */
void saveCredentials() {

  EEPROM.begin(512);

//  EEPROM.put(0, ssid);
//  EEPROM.put(0 + sizeof(ssid), passwd);
//  char ok[2 + 1] = "OK";
//  EEPROM.put(0 + sizeof(ssid) + sizeof(passwd), ok);

  EEPROM.put(0, mainSSID);
  EEPROM.put(0 + sizeof(mainSSID), mainPASSWD);
  char ok[2 + 1] = "OK";
  EEPROM.put(0 + sizeof(mainSSID) + sizeof(mainPASSWD), ok);

  EEPROM.commit();
  EEPROM.end();
}
