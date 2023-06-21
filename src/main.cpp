/// TESTED ON NODEMCU 8266 (ESP-12E)
//
//              +-----------------+
//              |0               0|
//              |    +-------+    |
//              |    |       |    *
//              |    |       |    *
//              |    |       |    *
//              |    |       |    *
//              |    +-------+    *
//              |                 * - 3v3
//              |   [NODEMCU v3]  * - gnd                 +---------------------------------------+
//              |           14(D5)* ---> [GPIO14]    ---> |*RO                                    |
//              |           12(D6)* ---> [GPIO12]    ---> |*RE              [MAX485]            O | ---> RS485 line B ---> to JBD BMS
//              |           13(D7)* ---> [GPIO13]    ---> |*DE              [board ]            O | ---> RS485 line A ---> to JBD BMS
//              |           15(D8)* ---> [GPIO15]    ---> |*DI                                    |
//              |             (RX)* - gpio3               +---------------------------------------+
//              |             (TX)* - gpio1
//              |                 * - gnd
//              |                 * - 3v3
//              |                 |
//              |O               O|
//              +-----------------+
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <SoftwareSerial.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#endif

#include <NTPClient.h>
#define USE_SERIAL_1602_LCD
#define LCD_I2C_ADDRESS 0x27   // Default LCD address is 0x27 for a 20 chars and 4 line / 2004 display
#include "LiquidCrystal_I2C.h" // Use an up to date library version which has the init method
// #define _LCD_TYPE 1
// #include <LCD_1602_RUS_ALL.h>
LiquidCrystal_I2C myLCD(LCD_I2C_ADDRESS, 16, 2);

byte rightUpAnode[8] = {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11110, 0b11110, 0b11110};
byte rightDownAnode[8] = {0b11110, 0b11110, 0b11110, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000};
byte leftUpKathode[8] = {0b11111, 0b10000, 0b10111, 0b10111, 0b10111, 0b10111, 0b10111, 0b10111};
byte leftDownKathode[8] = {0b10111, 0b10111, 0b10111, 0b10111, 0b10111, 0b10111, 0b10000, 0b11111};

byte upperFull[8] = {0b11111, 0b00000,0b11111,  0b11111, 0b11111, 0b11111, 0b11111, 0b11111};
byte upperClear[8] = {0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};
byte lowerFull[8] = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111,  0b11111,0b00000, 0b11111};
byte lowerClear[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111};
// Выделим два переопределяемых символа
//  LCD_1602_RUS myLCD(0x27, 16, 2, 2);
//                            ^^^ количество пользовательских символов

const long utcOffsetInSeconds = 10800;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>

#include <EEPROM.h>

// char mainSSID[33] = "";
// char mainPASSWD[65] = "";
char mainSSID[30] = "";
char mainPASSWD[34] = "";
char mainHOSTNAME[34] = "";

#include "credentials.h"
#include "home_svg.h"
// #include "WebPages.h"
// // #include "ModbusScanWebP.h"
// #include "ModbusRegWebP.h"
#include "UpdateWebP.h"

#include "exportedWebApp.h"
#include "exportedIndex.h"
#include "exportedSomeShit.h"
#include "exportedFavicon.h"
#include "exported_pwa_x192.h"

#include <bms2.h>

// #define rs485_RX 14
#define rs485_RX 12
#define rs485_DE -1
#define rs485_RE 13
#define rs485_TX 15

SoftwareSerial rs485(rs485_RX, rs485_TX); // RX, TX
OverkillSolarBms2 bms = OverkillSolarBms2();

unsigned long timer = millis();

void preTransmission()
{
    digitalWrite(rs485_RE, 1);
    digitalWrite(rs485_DE, 1);
}

void postTransmission()
{
    digitalWrite(rs485_RE, 0);
    digitalWrite(rs485_DE, 0);
}

#ifndef APSSID
#define APSSID "BMSESP2"
#define APPSK "xxxxxxxx"
#endif

bool idle_state = true;
bool ledState = 0;
bool mbScan = 0;
bool regScan = 0;
const int ledPin = 2;
uint8_t scan_client_id;
uint8_t terminal_client_id;
uint8_t pwd_client_id;
uint8_t bmsStat_client_id;

uint8_t scanMB_client_id;
const char *hostName = "bmsesp2";
const uint32_t modbusSpeed[] = {115200, 57600, 38400, 19200, 9600, 4800};

uint8_t mbi = 0;
uint32_t rebootTimer = millis();
bool shouldReboot = false;
uint8_t RRRclientID = 0;
uint8_t RRRtype = 0;
uint8_t RRRregSpeed = 0;
uint8_t RRRdevID = 0;
uint16_t RRRloReg = 0;
uint16_t RRRhiReg = 0;

const char *softAP_ssid = APSSID;
const char *softAP_password = APPSK;
const char *http_username = "admin";
const char *http_password = "admin";

const byte DNS_PORT = 53;

DNSServer dnsServer;

bool is_setup_done = false;
bool valid_ssid_received = false;
bool valid_password_received = false;
bool wifi_timeout = false;
// Web server
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

/* Soft AP network parameters */
IPAddress apIP(10, 1, 1, 1);
IPAddress netMsk(255, 255, 255, 0);

/** Should I connect to WLAN asap? */
boolean connect;

/** Last time I tried to connect to WLAN */
unsigned long lastConnectTry = 0;

/** Current WLAN status */
unsigned int status = WL_IDLE_STATUS;

uint32_t maxSketchSpace = 0;
int SketchSize = 0;
bool ws_conn = false;

void WiFiSoftAPSetup()
{
    WiFi.softAPConfig(apIP, apIP, netMsk);
    if (mainHOSTNAME)
    {
        WiFi.softAP(mainHOSTNAME, softAP_password);
        WiFi.hostname(mainHOSTNAME);
    }
    else
    {
        WiFi.softAP(softAP_ssid, softAP_password);
        WiFi.hostname(hostName);
    }

    delay(500); // Without delay I've seen the IP address blank
    Serial.print("softAP IP address: ");
    Serial.println(WiFi.softAPIP());
}

void notifyClients()
{
    Serial.println((String) "ledState - " + ledState);
    ws.textAll(String(ledState));
}

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        // request->addInterestingHeader("ANY");
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        server.rewrite("/", "/netscan").setFilter(ON_AP_FILTER);
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
        request->send(response);
    }
};

//  class CaptiveRequestHandler : public AsyncWebHandler
// {
// public:
//     CaptiveRequestHandler()
//     {
//         /* THIS IS WHERE YOU CAN PLACE THE CALLS */
//         server.onNotFound(notFound);
//         //  CAUTION: I could not get it work the code commented
//         //      //Android captive portal. Maybe not needed. Might be handled by notFound handler.
//         server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request)
//                   { request->send(200, "text/html", index_html); });
//         // Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
//         server.on("/fwlink", HTTP_GET, [](AsyncWebServerRequest *request)
//                   { request->send(200, "text/html", handleRequest); });
//         server.on("/connectivitycheck.gstatic.com", HTTP_GET, [](AsyncWebServerRequest *request)
//                   { request->send(200, "text/html", handleRequest); });
//         server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request)
//                   { request->send(200, "text/html", handleRequest); });
//         server.on("/captive.apple.com", HTTP_GET, [](AsyncWebServerRequest *request)
//                   { request->send(200, "text/html", handleRequest); });
//     }
//     virtual ~CaptiveRequestHandler() {}

//     bool canHandle(AsyncWebServerRequest *request)
//     {
//         return true;
//     }

//     void handleRequest(AsyncWebServerRequest *request)
//     {
//         server.rewrite("/", "/netscan").setFilter(ON_AP_FILTER);
//         AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
//         request->send(response);
//     }
// };

void writeRegValue(uint8_t RWWclientID, uint8_t RWWtype, uint8_t RWWregSpeed, uint8_t RWWdevID, uint16_t RWWReg, uint16_t RWWvalue)
{
}
void sendBatteryStatus(int cliId)
{
    // Serial.printf("%d looking Battery\n", cliId);
    String json = "BAT[";

    json += "{";
    json += "\"voltage\":" + String(bms.get_voltage());
    json += ",\"current\":" + String(bms.get_current());
    json += ",\"elemsInArray\":" + String(bms.get_num_cells());
    json += ",\"stateOfCharge\":" + String(bms.get_state_of_charge());
    json += ",\"cellsVoltage\":[";
    for (uint8_t i = 0; i < bms.get_num_cells(); i++)
    {
        // json += "\"value";
        // json += i;
        // json += "\":";
        json += String(bms.get_cell_voltage(i), 3);
        (i < (bms.get_num_cells() - 1)) ? json += "," : json += "";
    }
    json += "]";
    json += ",\"cellsBalance\":[";
    for (uint8_t i = 0; i < bms.get_num_cells(); i++)
    {
        // json += "\"value";
        // json += i;
        // json += "\":";
        json += String(bms.get_balance_status(i));
        (i < (bms.get_num_cells() - 1)) ? json += "," : json += "";
    }
    json += "]";
    json += ",\"ntcs\":[";
    for (uint8_t i = 0; i < bms.get_num_ntcs(); i++)
    {
        // json += "\"ntc";
        // json += i;
        // json += "\":";
        json += String(bms.get_ntc_temperature(i));
        (i < (bms.get_num_ntcs() - 1)) ? json += "," : json += "";
    }
    json += "]";
    json += ",\"BalanceCapacity\":" + String(bms.get_balance_capacity());
    json += ",\"RateCapacity\":" + String(bms.get_rate_capacity());
    json += ",\"chargeMosfet\":" + String(bms.get_charge_mosfet_status());
    json += ",\"dischargeMosfet\":" + String(bms.get_discharge_mosfet_status());
    json += ",\"bmsName\":\"" + String(bms.get_bms_name()) + "\"";
    json += ",\"faultCount\":" + String(bms.get_fault_count());
    json += ",\"protection_status\":" + String(bms.get_protection_status_summary());
    json += ",\"cycleCount\":" + String(bms.get_cycle_count());
    json += ",\"currentTime\":" + String(timeClient.getEpochTime() - utcOffsetInSeconds);

    json += "}";

    json += "]";
    if (cliId)
    {
        ws.text(cliId, (String)json);
    }
    else
    {
        ws.textAll((String)json);
    }

    json = String();
}
void webTerm(String message){
    if(terminal_client_id>0){
        ws.textAll((String)"TRM--"+String(timeClient.getEpochTime() - utcOffsetInSeconds)+"--"+message);
    }
}
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        data[len] = 0;
        os_printf("%s\n", (char *)arg);
        //    String message = (char *)arg;
        os_printf("%s\n", (char *)data);
        String message = (char *)data;
        // Serial.println(String((char *)data));
        Serial.printf("WebSocket client ID: #%u \n", client->id());
        webTerm((String)"WS#"+client->id()+"-:Message:-"+message);
        if (strcmp((char *)data, "toggle") == 0)
        {
            ledState = !ledState;
            notifyClients();
        }
        else if (strcmp((char *)data, "ip") == 0)
        {
            uint8_t ip_cli_id = client->id();
            // uint32_t id = client->id();
            Serial.print("id- ip - ");
            // Serial.println(id);
            String ipMsg = "IP--:local:-";
            ipMsg += WiFi.localIP().toString().c_str();
            ipMsg += "-:softap:-";
            ipMsg += WiFi.softAPIP().toString().c_str();
            ipMsg += "-:hostname:-";
            ipMsg += String(mainHOSTNAME);
            ipMsg += "-:ssid:-";
            ipMsg += String(mainSSID);
            ipMsg += "-:passwd:-";
            ipMsg += String(mainPASSWD);
            ws.text(ip_cli_id, ipMsg);
            //   String reply = (String)bms.get_voltage()+" - "+bms.get_current();
            // ws.text(sendBatteryStatus());
            // sendBatteryStatus(id);
        }
        else if (strcmp((char *)data, "batteryStatus") == 0)
        {
            bmsStat_client_id = client->id();
            // uint32_t id = client->id();
            Serial.print("id- getBatteryStatus - ");
            // Serial.println(id);
            //   String reply = (String)bms.get_voltage()+" - "+bms.get_current();
            // ws.text(sendBatteryStatus());
            // sendBatteryStatus(id);
        }
        else if (strcmp((char *)data, "reset") == 0)
        {
            ESP.reset();
            // ESP.updateSketch();
        }
        else if (strcmp((char *)data, "config") == 0)
        {
            ESP.eraseConfig();
            // ESP.updateSketch();
        }
        else if (strcmp((char *)data, "restart") == 0)
        {
            ESP.restart();
        }
        else if (strcmp((char *)data, "terminal") == 0)
        {
            terminal_client_id = client->id();
        }
        else if (strcmp((char *)data, "netscan") == 0)
        {
            scan_client_id = client->id();
        }
        else if (strcmp((char *)data, "scanmodbus") == 0)
        {
            scanMB_client_id = client->id();
            mbScan = true;
        }
        else if ((message.substring(0, message.indexOf("--")) == "MSFT"))
        {
            Serial.println("MSFT");
            String msft1 = message.substring(message.indexOf("--") + 2, message.length());
            Serial.println(msft1);
            String cmd = msft1.substring(0, msft1.indexOf("-"));
            Serial.println(cmd);
            String value = msft1.substring(msft1.indexOf("-") + 1, msft1.length());
            Serial.println(value);
            if (cmd == "dsc")
            {
                if (value == "false")
                {
                    bms.set_0xE1_mosfet_control_discharge(0);
                }
                else
                {
                    bms.set_0xE1_mosfet_control_discharge(1);
                }
            }
            else if (cmd == "chr")
            {
                if (value == "false")
                {
                    bms.set_0xE1_mosfet_control_charge(0);
                }
                else
                {
                    bms.set_0xE1_mosfet_control_charge(1);
                }
            }
        }
        else if ((message.substring(0, message.indexOf("--")) == "PWD"))
        {
            pwd_client_id = client->id();
            Serial.println("PWD");
            String tmp1 = message.substring(message.indexOf("--") + 2, message.length());
            Serial.println(tmp1);
            String tmpSSID = tmp1.substring(0, tmp1.indexOf("::"));
            Serial.println(tmpSSID);
            String tmp2 = tmp1.substring(tmp1.indexOf("::") + 2, tmp1.length());
            Serial.println(tmp2);
            String tmpPASSWD = tmp2.substring(0, tmp2.indexOf("::"));
            Serial.println(tmpPASSWD);
            String tmpHOSTNAME = tmp2.substring(tmp2.indexOf("::") + 2, tmp2.length());
            Serial.println(tmpHOSTNAME);

            tmpSSID.toCharArray(mainSSID, tmpSSID.length() + 1);
            tmpPASSWD.toCharArray(mainPASSWD, tmpPASSWD.length() + 1);
            tmpHOSTNAME.toCharArray(mainHOSTNAME, tmpHOSTNAME.length() + 1);
            Serial.print("xSSID: ");
            Serial.println(mainSSID);
            Serial.print("And xPASSWD : ");
            Serial.println(mainPASSWD);
            Serial.print("And xHOSTNAME : ");
            Serial.println(mainHOSTNAME);

            // char xSSID[33] = "";
            // char xPASSWD[65] = "";

            //      WiFiStationSetup(tmpSSID, tmpPASSWD);
            //      connectWifi();
            //      if (connectWifi(xSSID, xPASSWD)){
            //        Serial.println("Connection to Wifi successful.");
            saveCredentials();
            //        }else{
            //          Serial.println("Wrong SSID or password");
            //          }
        }

        uint8_t cientID = client->id();
        String reg0lbel = message.substring(0, message.indexOf(":"));
        String reg1tmp1 = message.substring(message.indexOf(":") + 1, message.length());
        uint8_t reg1type = reg1tmp1.substring(0, reg1tmp1.indexOf(":")).toInt();
        String reg1tmp2 = reg1tmp1.substring(reg1tmp1.indexOf(":") + 1, reg1tmp1.length());
        uint8_t reg2speed = reg1tmp2.substring(0, reg1tmp2.indexOf(":")).toInt();
        String reg1tmp3 = reg1tmp2.substring(reg1tmp2.indexOf(":") + 1, reg1tmp2.length());

        uint8_t reg3id = reg1tmp3.substring(0, reg1tmp3.indexOf(":")).toInt();
        String reg1tmp4 = reg1tmp3.substring(reg1tmp3.indexOf(":") + 1, reg1tmp3.length());
        uint16_t reg4loReg = reg1tmp4.substring(0, reg1tmp4.indexOf(":")).toInt();
        uint16_t reg4hiReg = reg1tmp4.substring(reg1tmp4.indexOf(":") + 1, reg1tmp4.length()).toInt();
        String mbr = "MBR";
        String mbw = "MBW";
        //      if(mbr ==reg0lbel)Serial.println("MBR equal MBR");
        if (mbr == reg0lbel)
        {
            RRRhiReg = reg4hiReg;
            RRRloReg = reg4loReg;
            RRRdevID = reg3id;
            RRRregSpeed = reg2speed;
            RRRtype = reg1type;
            RRRclientID = cientID;
            regScan = true;
        }
        else if (mbw == reg0lbel)
        {
            writeRegValue(cientID, reg1type, reg2speed, reg3id, reg4loReg, reg4hiReg);
        }
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        webTerm((String)"WS#"+ client->id()+" connected from "+client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        webTerm((String)"WebSocket client #"+ client->id()+" disconnected ");

        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len, client);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

String processor(const String &var)
{
    Serial.print(var);
    if (var == "STATE")
    {
        if (ledState)
        {
            Serial.println(" - ON");
            return "ON";
        }
        else
        {
            Serial.println(" - OFF");
            return "OFF";
        }
    }
    else if (var == "PUBLICIP")
    {
        return WiFi.softAPIP().toString().c_str();
        // ;"-local-"WiFi.localIP().toString().c_str();
    }
    else if (var == "LOCALIP")
    {
        // return WiFi.softAPIP().toString();
        return WiFi.localIP().toString().c_str();
    }

    return String();
}

void initWebSocket()
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);
};

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void setupWebServer()
{

    server.onNotFound(notFound);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
            request->send(response); });
    server.on("/netscan", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
            request->send(response); });
    server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
            request->send(response); });
    server.on("/battery", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
            request->send(response); });
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
            request->send(response); });
    server.on("/terminal", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
            request->send(response); });

    server.on("/initBatteryArray.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"text/javascript", initBatteryArray);
            request->send(response); });
    server.on("/onBTRMessage.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"text/javascript", onBTRMessage);
               request->send(response); });
    server.on("/btr2refrshlvlJs.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"text/javascript", btr2refrshlvlJs);
               request->send(response); });
    // server.on("/onIPMessage.js", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { AsyncWebServerResponse *response = request->beginResponse_P(200,"text/javascript", onIPMessage);
    //            request->send(response); });
    // server.on("/onNetMessage.js", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { AsyncWebServerResponse *response = request->beginResponse_P(200,"text/javascript", onNetMessage);
    //            request->send(response); });
    //    request->send(200, "text/javascript", wsJs_file); });
    server.on("/home.svg", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/svg+xml", home_svg);
               request->send(response); });
    server.on("/wifi.svg", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/svg+xml", wifi_svg);
               request->send(response); });
    server.on("/update.svg", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/svg+xml", update_svg);
               request->send(response); });
    server.on("/cog.svg", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/svg+xml", cog_svg);
               request->send(response); });
    server.on("/menu.svg", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/svg+xml", menu_svg);
               request->send(response); });
    server.on("/close.svg", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/svg+xml", close_svg);
               request->send(response); });
    server.on("/terminal.svg", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/svg+xml", terminal_svg);
               request->send(response); });

    server.on("/manifest.json", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"application/json", manifest_json);
               request->send(response); });

    server.on("/someshit/webapp.html", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"text/html", webapp_html);
               request->send(response); });
    server.on("/someshit/index-css.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"text/css", index_css);
               request->send(response); });
    server.on("/someshit/index-js.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"text/javascript", index_js);
               request->send(response); });

    server.on("/someshit/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/x-icon", favicon,favicon_len);
               request->send(response); });
    server.on("/someshit/favicon16x16.png", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/png", favicon16x16, favicon16x16_len);
               request->send(response); });
    server.on("/someshit/favicon32x32.png", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/png", favicon32x32, favicon32x32_len);
               request->send(response); });
    server.on("/someshit/favicon48x48.png", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/png", favicon48x48, favicon48x48_len);
               request->send(response); });

    server.on("/pwa-x192.png", HTTP_GET, [](AsyncWebServerRequest *request)
              { AsyncWebServerResponse *response = request->beginResponse_P(200,"image/png", pwa_x192, pwa_x192_len);
               request->send(response); });

    server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->addHeader("Server", "ESP Async Web Server");
    response->printf("<!DOCTYPE html><html><head><title>Webpage at %s</title></head><body>", request->url().c_str());
    response->print("<h2>Hello ");
    response->print(request->client()->remoteIP());
    response->print("</h2>");
    response->print("<h3>General</h3>");
    response->print("<ul>");
    response->printf("<li>Version: HTTP/1.%u</li>", request->version());
    response->printf("<li>Method: %s</li>", request->methodToString());
    response->printf("<li>URL: %s</li>", request->url().c_str());
    response->printf("<li>Host: %s</li>", request->host().c_str());
    response->printf("<li>ContentType: %s</li>", request->contentType().c_str());
    response->printf("<li>ContentLength: %u</li>", request->contentLength());
    response->printf("<li>Multipart: %s</li>", request->multipart() ? "true" : "false");
    response->print("</ul>");
    response->print("<h3>Headers</h3>");
    response->print("<ul>");
    int headers = request->headers();
    for (int i = 0; i < headers; i++) {
      AsyncWebHeader *h = request->getHeader(i);
      response->printf("<li>%s: %s</li>", h->name().c_str(), h->value().c_str());
    }
    response->print("</ul>");
    response->print("<h3>Parameters</h3>");
    response->print("<ul>");
    int params = request->params();
    for (int i = 0; i < params; i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->isFile()) {
        response->printf("<li>FILE[%s]: %s, size: %u</li>", p->name().c_str(), p->value().c_str(), p->size());
      } else if (p->isPost()) {
        response->printf("<li>POST[%s]: %s</li>", p->name().c_str(), p->value().c_str());
      } else {
        response->printf("<li>GET[%s]: %s</li>", p->name().c_str(), p->value().c_str());
      }
    }
    response->print("</ul>");
    response->print("</body></html>");
    //send the response last
    request->send(response); });

    server.on("/ip", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->addHeader("Server", "ESP Async Web Server");
    response->printf("<!DOCTYPE html><html><head><title>Webpage at %s</title></head><body>", request->url().c_str());
    response->print("<h2>Hello ");
    response->print(request->client()->remoteIP());
    response->print("</h2>");
            Serial.print("id- ip - ");

            String ipMsg = "IP--:local:-";
            ipMsg+=WiFi.localIP().toString().c_str();
            ipMsg+="-:softap:-";
            ipMsg+=WiFi.softAPIP().toString().c_str();
            ipMsg+="-:hostname:-";
            ipMsg+=WiFi.getHostname();
            ipMsg+="-:ssid:-";
            ipMsg+=String(mainSSID);
            ipMsg+="-:passwd:-";
            ipMsg+=String(mainPASSWD);

            response->print(ipMsg);
            // ws.text(ip_cli_id, ipMsg);


    response->print("</body></html>");
    //send the response last
    request->send(response); });
    server.on("/btr", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->addHeader("Server", "ESP Async Web Server");
    response->printf("<!DOCTYPE html><html><head><title>Webpage at %s</title></head><body>", request->url().c_str());
    response->print("<h2>Hello ");
    response->print(request->client()->remoteIP());
    response->print("</h2>");
         

    String json = "BAT[";

    json += "{";
    json += "\"voltage\":" + String(bms.get_voltage());
    json += ",\"current\":" + String(bms.get_current());
    json += ",\"elemsInArray\":" + String(bms.get_num_cells());
    json += ",\"stateOfCharge\":" + String(bms.get_state_of_charge());
    json += ",\"cellsVoltage\":[";
    for (uint8_t i = 0; i < bms.get_num_cells(); i++)
    {
        // json += "\"value";
        // json += i;
        // json += "\":";
        json += String(bms.get_cell_voltage(i), 3);
        (i < (bms.get_num_cells() - 1)) ? json += "," : json += "";
    }
    json += "]";
    json += ",\"cellsBalance\":[";
    for (uint8_t i = 0; i < bms.get_num_cells(); i++)
    {
        // json += "\"value";
        // json += i;
        // json += "\":";
        json += String(bms.get_balance_status(i));
        (i < (bms.get_num_cells() - 1)) ? json += "," : json += "";
    }
    json += "]";
    json += ",\"ntcs\":[";
    for (uint8_t i = 0; i < bms.get_num_ntcs(); i++)
    {
        // json += "\"ntc";
        // json += i;
        // json += "\":";
        json += String(bms.get_ntc_temperature(i));
        (i < (bms.get_num_ntcs() - 1)) ? json += "," : json += "";
    }
    json += "]";
    json += ",\"BalanceCapacity\":" + String(bms.get_balance_capacity());
    json += ",\"RateCapacity\":" + String(bms.get_rate_capacity());
    json += ",\"chargeMosfet\":" + String(bms.get_charge_mosfet_status());
    json += ",\"dischargeMosfet\":" + String(bms.get_discharge_mosfet_status());
    json += ",\"bmsName\":\"" + String(bms.get_bms_name()) + "\"";
    json += ",\"faultCount\":" + String(bms.get_fault_count());
    json += ",\"protection_status\":" + String(bms.get_protection_status_summary());
    json += ",\"cycleCount\":" + String(bms.get_cycle_count());
    json += ",\"currentTime\":" + String(timeClient.getEpochTime());

    json += "}";

    json += "]";

            response->print(json);
            // ws.text(ip_cli_id, ipMsg);


    response->print("</body></html>");
    //send the response last
    request->send(response); });

    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //           {
    // AsyncWebServerResponse *response = request->beginResponse_P(200,"text/html", battery_html,processor);
    //            request->send(response); });

    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //         { request->send(LittleFS, "/index.html", "text/html"); });

    //   { request->send_P(200, "text/html", battery_html, processor); });
    server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", String(ESP.getFreeHeap())); });

    // server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { if (!request->authenticate(http_username, http_password)){
    //             return request->requestAuthentication();
    //           } request->send_P(200, "text/html", settings_html, processor); });
    // server.on("/btn", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send_P(200, "text/html", btn_html, processor); });

    // server.on("/modbus", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send_P(200, "text/html", modbus_cfg_html, processor); });
    // server.on("/modbus-reg", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send_P(200, "text/html", modbus_reg_scanner_html, processor); });

    server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    if (!request->authenticate(http_username, http_password)){
                return request->requestAuthentication();
              }
    request->send(200, "text/plain", "Login Success!"); });

    // Simple Firmware Update Form
    // server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
    //           {
    //             if (!request->authenticate(http_username, http_password)){
    //             return request->requestAuthentication();
    //           }
    //             request->send_P(200, "text/html", webUpdate_html, processor); });
    server.on(
        "/update", HTTP_POST, [](AsyncWebServerRequest *request)
        {
            // if (!request->authenticate(http_username, http_password)){
            //     return request->requestAuthentication();
            //   }
      shouldReboot = !Update.hasError();
      shouldReboot ? rebootTimer = millis() : false;
      AsyncWebServerResponse *response = request->beginResponse(200, "text/html", shouldReboot ? webUpdate_ok_html : webUpdate_fail_html);
      
      Serial.println(shouldReboot);
      response->addHeader("Connection", "close");
      request->send(response); },
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
        {
            if (!index)
            {
                Serial.printf("Update Start: %s\n", filename.c_str());
                Update.runAsync(true);
                if (!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000))
                {
                    Update.printError(Serial);
                }
            }
            if (!Update.hasError())
            {
                if (Update.write(data, len) != len)
                {
                    Update.printError(Serial);
                }
            }
            if (final)
            {
                if (Update.end(true))
                {
                    Serial.printf("Update Success: %uB\n", index + len);
                }
                else
                {
                    Update.printError(Serial);
                }
            }
        });
}

/*         server.rewrite("/", "/netscan").setFilter(ON_AP_FILTER);
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
            request->send(response); */

void StartCaptivePortal()
{
    Serial.println("Setting up AP Mode");
    WiFiSoftAPSetup();
    Serial.println("Setting up Async WebServer");

    setupWebServer();
    initWebSocket();
    Serial.println("Starting DNS Server");
    dnsServer.start(53, "*", WiFi.softAPIP());
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP

    server.rewrite("/", "/netscan").setFilter(ON_AP_FILTER);

    server.begin();
    dnsServer.processNextRequest();
}

void connectWifi()
{

    if (String(mainSSID) != "")
    {

        Serial.println("Connecting as wifi client...");
        Serial.print(mainSSID);
        Serial.print(" - ");
        Serial.print(mainPASSWD);
        Serial.println();
        WiFi.disconnect();
        WiFi.begin(mainSSID, mainPASSWD);

        int connRes = WiFi.waitForConnectResult();
        // WiFi.hostname(hostName);
        WiFi.hostname(mainHOSTNAME);
        Serial.print("connRes: ");
        Serial.println(connRes);
        String sendMsg = "NCSucessful:-";
        sendMsg += mainSSID;
        sendMsg += "-:pwd:-";
        sendMsg += mainPASSWD;
        sendMsg += "-:hostname:-";
        sendMsg += mainHOSTNAME;
        ws.text(pwd_client_id, sendMsg);
        //      return connRes;
    }
    else
    {
        Serial.println("Had no wifi credentials...");
    }
}

void sendWifiScanResult(int n)
{
    Serial.printf("%d network(s) found\n", n);
    String json = "SCN[";
    if (n)
    {
        for (int i = 0; i < n; ++i)
        {
            if (i)
                json += ",";
            json += "{";
            json += "\"rssi\":" + String(WiFi.RSSI(i));
            json += ",\"ssid\":\"" + WiFi.SSID(i) + "\"";
            json += ",\"bssid\":\"" + WiFi.BSSIDstr(i) + "\"";
            json += ",\"channel\":" + String(WiFi.channel(i));
            json += ",\"secure\":" + String(WiFi.encryptionType(i));
            json += ",\"hidden\":" + String(WiFi.isHidden(i) ? "true" : "false");
            json += "}";
        }
        WiFi.scanDelete();
    }
    json += "]";
    ws.text(scan_client_id, (String)json);
    // ws.textAll((String)json);
    json = String();
    scan_client_id = 0;
}

void setup()
{
    pinMode(rs485_RE, OUTPUT);
    pinMode(rs485_DE, OUTPUT);
    preTransmission();
    postTransmission();
    loadCredentials();
    myLCD.init();
    myLCD.clear();
    myLCD.backlight();
    myLCD.setCursor(1, 0);
    myLCD.print("ЖОПА POWERBANK");
    myLCD.setCursor(2, 1);
    myLCD.print(__DATE__);

    myLCD.createChar(0, leftUpKathode);
    myLCD.createChar(1, leftDownKathode);
    myLCD.createChar(2, upperClear);
    myLCD.createChar(3, lowerClear);
    myLCD.createChar(4, upperFull);
    myLCD.createChar(5, lowerFull);
    myLCD.createChar(6, rightUpAnode);
    myLCD.createChar(7, rightDownAnode);
    // bigNumberLCD.disableGapBetweenNumbers();

    // bigNumberLCD.setBigNumberCursor(0);
    // bigNumberLCD.print(F("--" ONE_COLUMN_SPACE_STRING "47.11"));
    // bigNumberLCD.writeAt(':',19); // Because numbers have by default a trailing but no leading gap.

    Serial.begin(115200);

    rs485.begin(9600);
    while (!rs485)
    { // Wait for the BMS serial port to initialize
    }
    bms.begin(&rs485);
    bms.preTransmission(preTransmission);
    bms.postTransmission(postTransmission);

    bms.main_task(true);

    StartCaptivePortal();
    delay(500); // Without delay I've seen the IP address blank
                // Serial.print("softAP IP address: ");
                // Serial.println(WiFi.softAPIP());
                // Begin LittleFS
    if (!LittleFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }
    /* Setup the DNS server redirecting all the domains to the apIP */
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", apIP);

    connectWifi();
    Serial.println("");
    Serial.print("local IP Address: ");
    Serial.println(WiFi.localIP());
    // server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);  //only when requested from AP
    //  server.rewrite("/", "/scan").setFilter(ON_AP_FILTER);
    //  server.rewrite("/", "/heap").setFilter(ON_STA_FILTER);
    setupWebServer();
    initWebSocket();
    server.begin();

    //   Serial.println( "Certs validation" );
    //   server.beginSecure("/Cert.pem", "/Key.pem", NULL);
    //   Serial.println( "HTTPs server has started" );

    timeClient.begin();
}
uint32_t timer2 = millis();
uint32_t timer3 = millis();
bool changedDisplay = false;
void dispLayout1()
{
    String time = timeClient.getFormattedTime();

    uint8_t day = timeClient.getDay();

    myLCD.setCursor(0, 0);
    myLCD.print(time);
    myLCD.setCursor(9, 0);
    myLCD.print("Ёмк.");
    myLCD.print((String)bms.get_state_of_charge() + "%");

    myLCD.setCursor(0, 1);
    myLCD.print((String)bms.get_voltage());
    myLCD.print("V");

    myLCD.setCursor(8, 1);
    myLCD.print((String)bms.get_current());
    myLCD.print("A");
}

void dispLayout2()
{
    uint8_t stateOfCharge = bms.get_state_of_charge();
            myLCD.setCursor(0, 0);
            myLCD.write(0);
            myLCD.setCursor(0, 1);
            myLCD.write(1);
    for (size_t i = 1; i < 9; i++)
    {
        if (i * 13 < stateOfCharge)
        {
            myLCD.setCursor(i, 0);
            myLCD.write(4);
            myLCD.setCursor(i, 1);
            myLCD.write(5);
        }
        else
        {
            myLCD.setCursor(i, 0);
            myLCD.write(2);
            myLCD.setCursor(i, 1);
            myLCD.write(3);
        }
    }
            myLCD.setCursor(9, 0);
            myLCD.write(6);
            myLCD.setCursor(9, 1);
            myLCD.write(7);



            myLCD.setCursor(10, 0);
            myLCD.print((String)bms.get_voltage()+"V   ");
            myLCD.setCursor(10, 1);
            myLCD.print((String)bms.get_current()+"A   ");
}

void loop()
{

    if (idle_state)
    {

        // bigNumberLCD.setBigNumberCursor(0);
        // dispLayout1();
        if (!changedDisplay)
        {
            myLCD.clear();

            changedDisplay = true;
        }
        dispLayout2();
        idle_state = !idle_state;
        timer3 = millis();
    }

    if (millis() - timer3 > 1000)
    {
        idle_state = true;
        timer3 = millis();
    }
    if (millis() - timer2 > 15000)
    {

        timer2 = millis();
        timeClient.update();

        // idle_state = false;
        // timer3 = millis();
        // myLCD.clear();
        // // myLCD.home();

        // bigNumberLCD.print(bms.get_voltage());

        // bigNumberLCD.print(bms.get_current());

        // Serial.println((String)"Vcc\t\t\t\t\t\t"+ESP.getVcc());
        // Serial.println((String)"BootVersion\t\t"+ESP.getBootVersion());
        // Serial.println((String)"CpuFreqMHz\t\t"+ESP.getCpuFreqMHz());
        // Serial.println((String)"CycleCount\t\t"+ESP.getCycleCount());
        // Serial.println((String)"FlashChipId\t\t"+ESP.getFlashChipId());
        // Serial.println((String)"FlashChipMode\t\t"+ESP.getFlashChipMode());
        // Serial.println((String)"FlashChipRealSize\t\t"+ESP.getFlashChipRealSize());
        // Serial.println((String)"FlashChipSize\t\t"+ESP.getFlashChipSize());
        // Serial.println((String)"FlashChipSizeByChipIdn\t\t"+ESP.getFlashChipSizeByChipId());
        // Serial.println((String)"FlashChipSpeed\t\t"+ESP.getFlashChipSpeed());
        // Serial.println("--------------------------");
        // Serial.println((String)"FreeContStack\t\t"+ESP.getFreeContStack());
        // Serial.println((String)"FreeHeap\t\t"+ESP.getFreeHeap());
        // Serial.println((String)"FreeSketchSpace\t\t"+ESP.getFreeSketchSpace());
        // Serial.println((String)"FullVersion\t\t"+ESP.getFullVersion());
        // Serial.println((String)"HeapFragmentation\t\t"+ESP.getHeapFragmentation());
        // Serial.println((String)"ResetInfo\t\t"+ESP.getResetInfo());
        // Serial.println((String)"ResetReason\t\t"+ESP.getResetReason());
        // Serial.println((String)"SdkVersion\t\t"+ESP.getSdkVersion());
        // Serial.println((String)"SketchMD5\t\t"+ESP.getSketchMD5());
        // Serial.println((String)"SketchSize\t\t"+ESP.getSketchSize());
        // Serial.println("--------------------------");
        // Serial.print(daysOfTheWeek[timeClient.getDay()]);
        // Serial.print(", ");
        // Serial.print(timeClient.getHours());
        // Serial.print(":");
        // Serial.print(timeClient.getMinutes());
        // Serial.print(":");
        // Serial.println(timeClient.getSeconds());
        // Serial.println(timeClient.getFormattedTime());
        // unsigned long epochTime = timeClient.getEpochTime();
        // Serial.print("Epoch Time: ");
        // Serial.println(epochTime);
    }

    if (millis() - timer > 1000)
    {
        timer = millis();
        bms.main_task(true);

        // sendBatteryStatus(NULL);

        //
        //    bms.set_0xE1_mosfet_control(1,1);
        //    bms.set_0xE1_mosfet_control_charge(1);
        //    bms.set_0xE1_mosfet_control_discharge(1);
    }

    //  rebootTimer = millis();
    if (shouldReboot)
    {
        Serial.println(3000 - (millis() - rebootTimer));
        if (millis() - rebootTimer > 3000)
        {
            ESP.restart();
        }
    }

    if (bmsStat_client_id)
    {
        sendBatteryStatus(bmsStat_client_id);
        bmsStat_client_id = 0;
    }
    if (scan_client_id)
    {
        WiFi.scanNetworksAsync(sendWifiScanResult, true);
    }

    if (connect)
    {
        connect = false;
        Serial.println("Connect requested");

        connectWifi();
        lastConnectTry = millis();
    }
    else
    {
        int s = WiFi.status();
        if (s == 0 && millis() > (lastConnectTry + 3000))
        {
            /* If WLAN disconnected and idle try to connect */
            /* Don't set retry time too low as retry interfere the softAP operation */
            connect = true;
        }
        if (status != s)
        { // WLAN status change
            Serial.print("Status: ");
            Serial.println(s);
            status = s;
            if (s == WL_CONNECTED)
            {
                idle_state = false;
                timer3 = millis();
                /* Just connected to WLAN */
                Serial.println("");
                Serial.print("Connected to ");
                Serial.println(mainSSID);
                Serial.print("IP address: ");
                Serial.println(WiFi.localIP());
                myLCD.clear();
                myLCD.home();
                myLCD.print("SSID:");
                myLCD.print(mainSSID);
                myLCD.setCursor(0, 1);
                myLCD.print("IP:");
                myLCD.print(WiFi.localIP().toString().c_str());
                // Setup MDNS responder
                if (!MDNS.begin(mainHOSTNAME))
                {
                    Serial.println("Error setting up MDNS responder!");
                }
                else
                {
                    Serial.println("mDNS responder started");
                    myLCD.home();
                    myLCD.print(" mDNS started ");
                    myLCD.clear();

                    // Add service to MDNS-SD
                    MDNS.addService("http", "tcp", 80);
                }
            }
            else if (s == WL_NO_SSID_AVAIL)
            {
                Serial.println("WL_NO_SSID_AVAIL");
                WiFi.disconnect();
            }
            else if (s == WL_CONNECT_FAILED)
            {
                Serial.println("WL_CONNECT_FAILED");
            }
            else if (s == WL_IDLE_STATUS)
            {
                Serial.println("WL_IDLE_STATUS");
            }
            else if (s == WL_DISCONNECTED)
            {
                Serial.println("WL_DISCONNECTED");
            }
            else if (s == -1)
            {
                Serial.println("WL_TIMEOUT");
            }
        }
        if (s == WL_CONNECTED)
        {
            MDNS.update();
        }
    }
    // Do work:
    // DNS
    dnsServer.processNextRequest();
    // HTTP

    if (Serial.available() > 0)
    {
        char c[] = {(char)Serial.read()};
    }

    ws.cleanupClients();
    digitalWrite(ledPin, !ledState);
}
