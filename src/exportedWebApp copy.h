 const char manifest_webmanifest[] PROGMEM = R"rawliteral(
{"name":"BMSESP","short_name":"BMSESP","start_url":".","display":"standalone","background_color":"#000000","lang":"ru","scope":"./","description":"BMSESP - the best webApplication!!!","theme_color":"#000","orientation":"portrait","icons":[{"src":"pwa-x192.png","sizes":"192x192","type":"image/png","purpose":"any"},{"src":"pwa-x512.png","sizes":"512x512","type":"image/png","purpose":"any"},{"src":"pwa-x512.png","sizes":"512x512","type":"image/png","purpose":"maskable"}]}
  )rawliteral";
 const char registerSW[] PROGMEM = R"rawliteral(
if('serviceWorker' in navigator) {window.addEventListener('load', () => {navigator.serviceWorker.register('/sw-front.js', { scope: '/' })})}
  )rawliteral";
 const char sw_front[] PROGMEM = R"rawliteral(
if(!self.define){let e,n={};const s=(s,i)=>(s=new URL(s+".js",i).href,n[s]||new Promise((n=>{if("document"in self){const e=document.createElement("script");e.src=s,e.onload=n,document.head.appendChild(e)}else e=s,importScripts(s),n()})).then((()=>{let e=n[s];if(!e)throw new Error(`Module ${s} didn’t register its module`);return e})));self.define=(i,t)=>{const r=e||("document"in self?document.currentScript.src:"")||location.href;if(n[r])return;let o={};const c=e=>s(e,r),m={module:{uri:r},exports:o,require:c};n[r]=Promise.all(i.map((e=>m[e]||c(e)))).then((e=>(t(...e),o)))}}define(["./workbox-c987d892"],(function(e){"use strict";self.skipWaiting(),e.clientsClaim(),e.precacheAndRoute([{url:"index.html",revision:"6dbfede28bf5ea0e6365b47fe032f63b"},{url:"registerSW.js",revision:"488b9e6ed86e8369b4d8dce8f00ccd59"},{url:"someshit/index-9d805b0c.js",revision:null},{url:"someshit/index-a6f162e7.css",revision:null},{url:"someshit/webapp.html",revision:"57b0886933ea447ae11040142861502b"},{url:"manifest.webmanifest",revision:"ed4e244c2f34b07ece0c68aa8a958f5f"}],{}),e.cleanupOutdatedCaches(),e.registerRoute(new e.NavigationRoute(e.createHandlerBoundToURL("index.html"))),e.registerRoute(/\.png/i,new e.CacheFirst({cacheName:"interfaceImages",plugins:[new e.ExpirationPlugin({maxEntries:10,maxAgeSeconds:31536e3}),new e.CacheableResponsePlugin({statuses:[0,200]})]}),"GET"),e.registerRoute(/\.jpg/i,new e.CacheFirst({cacheName:"beautyImages",plugins:[new e.ExpirationPlugin({maxEntries:2e3,maxAgeSeconds:31536e3}),new e.CacheableResponsePlugin({statuses:[0,200]})]}),"GET"),e.registerRoute(/\/api\/.*\/*.json/,new e.NetworkOnly({plugins:[new e.BackgroundSyncPlugin("myQueueName",{maxRetentionTime:1440})]}),"POST")}));
  )rawliteral";
 const char manifest_json[] PROGMEM = R"rawliteral(
{
  "index.css": {
    "file": "someshit/index-ssc.css",
    "src": "index.css"
  },
  "index.html": {
    "css": [
      "someshit/index-dc.css"
    ],
    "file": "someshit/index-sj.js",
    "isEntry": true,
    "src": "index.html"
  }
}
)rawliteral";

 const char ws_js[] PROGMEM = R"rawliteral(
var gateway="ws://" + window.location.hostname + "/ws"; 
var websocket; 

let int;
function initWebSocket(){
    console.log('Trying to open a WebSocket connection...'); 
    websocket=new WebSocket(gateway);

    }
initWebSocket();
  )rawliteral";

 const char btr2refrshlvlJs[] PROGMEM = R"rawliteral(
let batteryIsCharging = false;
function refreshLevel(elemNumber, level) {
    document.querySelector("#batteryCell"+elemNumber+" > div > div.acid-container > div > div").style.height = (level * document.querySelector("#batteryCell"+elemNumber+" > div").clientHeight) + "px";
    document.querySelector("#batteryCell"+elemNumber+" > div > div.acid-container > div > div").style.setProperty("--acid-height", (level * document.querySelector("#batteryCell0 > div").clientHeight) + "px");
    document.querySelector("#batteryCell"+elemNumber+" > div > div.percentage > span").innerText = (level*4.2).toFixed(3);
}
  )rawliteral";

 const char btr2NetMsg[] PROGMEM = R"rawliteral(

const onNetMessage = (event) => {
    const data = event.data;
    
    if (data.slice(0, 3) === "SCN") {
        console.log(data.slice(0, 3),"============received message");
        let payload = data.slice(3);
        document.getElementById('scanStations').innerText = "";
        console.log(JSON.parse(payload));
        let stations = JSON.parse(payload);
        for (let i = 0; i < stations.length; i++) {
            let p = document.createElement("div");
            p.setAttribute("label", stations[i].ssid);

            let ssid = document.createElement("span");
            if(!stations[i].hidden){
                ssid.innerText = stations[i].ssid;
            }else{
                ssid.innerText = "*(hidden)*";
            }
            
            p.appendChild(ssid);
            let rssi = document.createElement("span");
            rssi.innerText = stations[i].rssi + "dBm";
            p.appendChild(rssi);
            //      let bssid = document.createElement("span");
            //      bssid.innerText = stations[i].bssid;
            //      p.appendChild(bssid);
            let channel = document.createElement("span");
            channel.innerText = "ch:" + stations[i].channel;
            p.appendChild(channel);
            let secure = document.createElement("span");
            let securityDescription = "";
            switch (stations[i].secure) {
                case 2:
                    securityDescription = "TKIP";
                    break;
                case 4:
                    securityDescription = "CCMP";
                    break;
                case 5:
                    securityDescription = "WEP";
                    break;
                case 7:
                    securityDescription = "NONE";
                    break;
                case 8:
                    securityDescription = "AUTO";
                    break;
                default:
                    securityDescription = "NO INFO";
                    break;
            }
            secure.innerText = securityDescription;
            p.appendChild(secure);
            // let hidden = document.createElement("span");
            // if (!stations[i].hidden) {
            //     hidden.innerText = "visible";
            // } else {
            //     hidden.innerText = "hidden";
            // }

            // p.appendChild(hidden);

            document.getElementById('scanStations').appendChild(p);
            let bssid = document.createElement("span");
            bssid.innerText = stations[i].bssid;
            p.appendChild(bssid);

            p.onclick = (e) => {
                let ssid_name = e.target.parentElement.getAttribute("label");
                if (!ssid_name || ssid_name == null) {
                    ssid_name = e.target.getAttribute("label");
                }
                document.querySelector("#ssidInput").value = ssid_name;
                document.querySelector("#ssidPassword").focus();
                console.log(ssid_name);
            }
        }
    } else {
        console.log(data);
    }

}
  )rawliteral";

 const char btr2IPmsg[] PROGMEM = R"rawliteral(

const onIPMessage = (event) => {
    const data = event.data;
    console.log("IP Messages================================================");
    console.log(data);
    if (data.slice(0, 2) === "IP") {

        let payload = data.split("--:")[1];
        console.log(payload.split("-:"));
        let ipData = payload.split("-:");
        for (const obj of ipData) {
            console.log(obj.split(":-"));
            let item = obj.split(":-")[0];
            let value = obj.split(":-")[1];
            switch (item) {
                case "softap":
                    document.querySelector("#softIP").innerHTML = value;
                    break;

                case "local":

                    document.querySelector("#pubIP").innerHTML = value;
                    break;
                case "ssid":

                    document.querySelector("#netName").innerHTML = value;
                    break;

                default:
                    break;
            }
        }
    }else if (data.slice(0, 3) === "BAT") {
        console.log("BAT")
        let partOne = data.split(`current":`)[1];
        let partTwo = partOne.split(",")[0];
        let current = JSON.parse(partTwo);
        console.log(current);
        if(current > 0.2){
            document.querySelector('h1').classList.add('charging')
        }else if(current<0){
            document.querySelector('h1').classList.add('discharging')
        }else if(current==0){
            document.querySelector('h1').classList.remove('charging')
            document.querySelector('h1').classList.remove('discharging')
        }
    } else {
        console.log(data);
    }
}


  )rawliteral";

   const char btr2initBtrJs[] PROGMEM = R"rawliteral(
  function initBatteryArray(elemNum){
    const btrSt = document.btrSt = document.querySelector("body > div.content > div > div.batteryStats");
    btrSt.innerHTML = "";
    document.querySelector("#batteryArray").innerHTML = "";
    document.querySelector(".ntcsStats").innerHTML = "";
    let elem = `
    <div class="battery">
    <div class="acid-container">
      <div class="acid">
        <div class="level" class="fill"></div>
        <svg class="waves"
             xmlns="http://www.w3.org/2000/svg"
             xmlns:xlink="http://www.w3.org/1999/xlink"
             viewBox="0 24 150 28"
             preserveAspectRatio="none">
          <defs>
            <path id="gentle-wave"
                  d="M-160 44c30 0 
                     58-18 88-18s
                     58 18 88 18 
                     58-18 88-18 
                     58 18 88 18
                     v44h-352z" />
          </defs>
          <g class="parallax">
            <use xlink:href="#gentle-wave" x="50" y="0" fill="#000"/>
            <use xlink:href="#gentle-wave" x="50" y="3" fill="#fff"/>
            <use xlink:href="#gentle-wave" x="50" y="6" fill="#0ff"/>  
          </g>
        </svg>
      </div>
    </div>
  <div class="percentage"><i class="icon" class="fas fa-plug"></i><span class="percentagexxx">0</span> V</div>
  </div>
  `

    for(let i = 0; i< elemNum; i++){
        let div1 = document.createElement("div");
        div1.setAttribute("id",`batteryCell${i}`);
        div1.classList.add("cell");
        div1.innerHTML = elem;
        document.querySelector("#batteryArray").appendChild(div1);
    }

    btrSt.divVoltage = document.createElement("div");
    btrSt.divVoltage.classList.add("voltage");
    btrSt.appendChild(btrSt.divVoltage);
    btrSt.divVoltage.name = document.createElement("span");
    btrSt.divVoltage.name.classList.add("voltageName");
    btrSt.divVoltage.name.innerHTML = "Voltage:"
    btrSt.divVoltage.appendChild(btrSt.divVoltage.name);
    btrSt.divVoltage.value = document.createElement("span");
    btrSt.divVoltage.value.classList.add("voltageValue");
    btrSt.divVoltage.appendChild(btrSt.divVoltage.value);

    btrSt.divVoltDiff = document.createElement("div");
    btrSt.divVoltDiff.classList.add("voltage");
    btrSt.appendChild(btrSt.divVoltDiff);
    btrSt.divVoltDiff.name = document.createElement("span");
    btrSt.divVoltDiff.name.classList.add("divVoltDiffName");
    btrSt.divVoltDiff.name.innerHTML = "Voltage diff:"
    btrSt.divVoltDiff.appendChild(btrSt.divVoltDiff.name);
    btrSt.divVoltDiff.value = document.createElement("span");
    btrSt.divVoltDiff.value.classList.add("divVoltDiffValue");
    btrSt.divVoltDiff.appendChild(btrSt.divVoltDiff.value);
    
    btrSt.divCurrent = document.createElement("div");
    btrSt.divCurrent.classList.add("current");
    btrSt.appendChild(btrSt.divCurrent);
    btrSt.divCurrent.name = document.createElement("span");
    btrSt.divCurrent.name.classList.add("currentName");
    btrSt.divCurrent.name.innerHTML = "Current:"
    btrSt.divCurrent.appendChild(btrSt.divCurrent.name);
    btrSt.divCurrent.value = document.createElement("span");
    btrSt.divCurrent.value.classList.add("currentValue");
    btrSt.divCurrent.appendChild(btrSt.divCurrent.value);

    btrSt.balanceCap = document.createElement("div");
    btrSt.balanceCap.classList.add("balanceCap");
    btrSt.appendChild(btrSt.balanceCap);
    btrSt.balanceCap.name = document.createElement("span");
    btrSt.balanceCap.name.classList.add("balanceCapName");
    btrSt.balanceCap.name.innerHTML = "BalanceCapacity:"
    btrSt.balanceCap.appendChild(btrSt.balanceCap.name);
    btrSt.balanceCap.value = document.createElement("span");
    btrSt.balanceCap.value.classList.add("balanceCapValue");
    btrSt.balanceCap.appendChild(btrSt.balanceCap.value);
    
    btrSt.rateCap = document.createElement("div");
    btrSt.rateCap.classList.add("rateCap");
    btrSt.appendChild(btrSt.rateCap);
    btrSt.rateCap.name = document.createElement("span");
    btrSt.rateCap.name.classList.add("rateCapName");
    btrSt.rateCap.name.innerHTML = "RateCapacity:"
    btrSt.rateCap.appendChild(btrSt.rateCap.name);
    btrSt.rateCap.value = document.createElement("span");
    btrSt.rateCap.value.classList.add("rateCapValue");
    btrSt.rateCap.appendChild(btrSt.rateCap.value);
    
    btrSt.chrgMosfet = document.createElement("div");
    btrSt.chrgMosfet.classList.add("chrgMosfet");
    btrSt.appendChild(btrSt.chrgMosfet);

    btrSt.chrgMosfet.name = document.createElement("span");
    btrSt.chrgMosfet.name.classList.add("chrgMosfetName");
    btrSt.chrgMosfet.name.innerHTML = "Charging mosfet:"
    btrSt.chrgMosfet.appendChild(btrSt.chrgMosfet.name);

    btrSt.chrgMosfet.value = document.createElement("input");
    btrSt.chrgMosfet.value.setAttribute("type", "checkbox");
    btrSt.chrgMosfet.value.setAttribute("id", "chrgMosfetValue");
    btrSt.chrgMosfet.value.classList.add("chrgMosfetValue");
    btrSt.chrgMosfet.appendChild(btrSt.chrgMosfet.value);
    btrSt.chrgMosfet.label = document.createElement("label");
    btrSt.chrgMosfet.label.setAttribute("for", "chrgMosfetValue");
    btrSt.chrgMosfet.appendChild(btrSt.chrgMosfet.label);

    btrSt.dsChrgMosfet = document.createElement("div");
    btrSt.dsChrgMosfet.classList.add("dsChrgMosfet");
    btrSt.appendChild(btrSt.dsChrgMosfet);

    btrSt.dsChrgMosfet.name = document.createElement("span");
    btrSt.dsChrgMosfet.name.classList.add("dsChrgMosfetName");
    btrSt.dsChrgMosfet.name.innerHTML = "Discharging mosfet:"
    btrSt.dsChrgMosfet.appendChild(btrSt.dsChrgMosfet.name);

    btrSt.dsChrgMosfet.value = document.createElement("input");
    btrSt.dsChrgMosfet.value.setAttribute("type", "checkbox");
    btrSt.dsChrgMosfet.value.setAttribute("id", "dsChrgMosfetValue");
    btrSt.dsChrgMosfet.value.classList.add("dsChrgMosfetValue");
    btrSt.dsChrgMosfet.appendChild(btrSt.dsChrgMosfet.value);
    btrSt.dsChrgMosfet.label = document.createElement("label");
    btrSt.dsChrgMosfet.label.setAttribute("for", "dsChrgMosfetValue");
    btrSt.dsChrgMosfet.appendChild(btrSt.dsChrgMosfet.label);

    if(btrSt.chrgMosfet.value){
        clearEventListener(btrSt.chrgMosfet.value);
    }
    if(btrSt.dsChrgMosfet.value){
        clearEventListener(btrSt.dsChrgMosfet.value);
    }
    
    btrSt.stateChrg = document.createElement("div");
    btrSt.stateChrg.classList.add("stateChrg");
    btrSt.appendChild(btrSt.stateChrg);
    btrSt.stateChrg.name = document.createElement("span");
    btrSt.stateChrg.name.classList.add("stateChrgName");
    btrSt.stateChrg.name.innerHTML = "StateOfCharge:"
    btrSt.stateChrg.appendChild(btrSt.stateChrg.name);
    btrSt.stateChrg.value = document.createElement("span");
    btrSt.stateChrg.value.classList.add("stateChrgValue");
    btrSt.stateChrg.appendChild(btrSt.stateChrg.value);

    btrSt.currentTime = document.createElement("div");
    btrSt.currentTime.classList.add("currentTime");
    btrSt.appendChild(btrSt.currentTime);
    btrSt.currentTime.name = document.createElement("span");
    btrSt.currentTime.name.classList.add("currentTimeName");
    btrSt.currentTime.name.innerHTML = "CurrentTime:"
    btrSt.currentTime.appendChild(btrSt.currentTime.name);
    btrSt.currentTime.value = document.createElement("span");
    btrSt.currentTime.value.classList.add("currentTimeValue");
    btrSt.currentTime.appendChild(btrSt.currentTime.value);
      for(let x=0; x<3;x++){
        let span2 = document.createElement("span");
        span2.setAttribute("id",`ntc${x}`);
        span2.innerHTML = "tmp";
        document.querySelector(".ntcsStats").appendChild(span2);
      }  
      return btrSt;
}
function clearEventListener(element) {
 const clonedElement = element.cloneNode(true);
element.replaceWith(clonedElement);
return clonedElement;
}
  )rawliteral";

   const char btr2BtrMsg[] PROGMEM = R"rawliteral(
const onBTRMessage = (event )=> {
    const data = event.data;
    console.log(data);
    if (data.slice(0, 3) === "BAT") {
        const payload = data.slice(3);
        const result = JSON.parse(payload);
        /*console.log(result);*/
        batteryObj = result[0];
        console.log(batteryObj);
        if (batteryObj.cellsVoltage) {
            batteryObj.cellsVoltage.forEach((el, i) => {
                    refreshLevel(i, (el / 4.2));
                
            });
            const btrSt = document.btrSt;
            btrSt.balanceCap.value.innerHTML = batteryObj.BalanceCapacity + " Ah";
            btrSt.rateCap.value.innerHTML = batteryObj.RateCapacity + " Ah";
            btrSt.divVoltage.value.innerHTML = batteryObj.voltage + " Volts";
            btrSt.divCurrent.value.innerHTML = batteryObj.current + " Ampers";
            btrSt.divVoltDiff.value.innerHTML = (Math.max.apply(null, batteryObj.cellsVoltage) - Math.min.apply(null, batteryObj.cellsVoltage)).toFixed(3) + " V";

            document.querySelector("#chrgMosfetValue").checked = batteryObj.chargeMosfet;
            document.querySelector("#dsChrgMosfetValue").checked = batteryObj.dischargeMosfet;
            btrSt.stateChrg.value.innerHTML = batteryObj.stateOfCharge;

            document.querySelector("body > div.content > div > div.ntcsStats").childNodes.forEach((el, i) => {
                el.innerHTML = batteryObj.ntcs[i] + "&#176;C";
            });
        }
        let d = new Date(parseInt(batteryObj.currentTime + '000'));
        let hrs = d.getHours();
        if (hrs < 10) hrs = "0" + hrs;
        let mnts = d.getMinutes();
        if (mnts < 10) mnts = "0" + mnts;
        let secnds = d.getSeconds();
        if (secnds < 10) secnds = "0" + secnds;
        const btrSt = document.btrSt;
        btrSt.currentTime.value.innerHTML = `${hrs}:${mnts}:${secnds}`;
        if (batteryObj.current > 0) {
            document.querySelector('h1').innerHTML = "CHARGING!!!"
            document.querySelectorAll('.acid-container > .acid').forEach(elem => {
                elem.style.filter = "hue-rotate(222deg)"
                document.querySelector('h1').classList.add('charging');
                document.querySelector('h1').classList.remove('discharging');
            });
        } else if (batteryObj.current < 0) {
            document.querySelector('h1').innerHTML = "DISCHARGING!!!"
            document.querySelectorAll('.acid-container > .acid').forEach(elem => {
                elem.style.filter = "hue-rotate(0deg)"
                document.querySelector('h1').classList.remove('charging');
                document.querySelector('h1').classList.add('discharging');
            });
        } else {
            document.querySelector('h1').classList.remove('discharging');
            document.querySelector('h1').classList.remove('charging')
            document.querySelector('h1').innerHTML = "..idle"
            document.querySelectorAll('.acid-container > .acid').forEach(elem => {
                elem.style.filter = "hue-rotate(120deg)"
            });
        }
    }else{
       console.log(data); 
    }
}
  )rawliteral";