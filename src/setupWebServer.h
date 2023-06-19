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
    server.on("/onIPMessage.js", HTTP_GET, [](AsyncWebServerRequest *request)
    { AsyncWebServerResponse *response = request->beginResponse_P(200,"text/javascript", onIPMessage);
               request->send(response); });
    server.on("/onNetMessage.js", HTTP_GET, [](AsyncWebServerRequest *request)
    { AsyncWebServerResponse *response = request->beginResponse_P(200,"text/javascript", onNetMessage);
               request->send(response); });
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


    //     server.on("/netscan", HTTP_GET, [](AsyncWebServerRequest *request)
    //   {
    //     request->send_P(200, "text/html", netScan_html, processor); });


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
            if (!request->authenticate(http_username, http_password)){
                return request->requestAuthentication();
              }
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
