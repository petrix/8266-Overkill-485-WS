const char batteryJs_file[] PROGMEM = R"=====(
let batteryObj = {};

  function onMessage(event) {
    let data = event.data;
    console.log(data);
    if(data.slice(0,3) == "BAT"){
      let payload = data.slice(3);
      batteryObj = JSON.parse(payload);
            console.log(batteryObj[0]);
            if(batteryObj[0].cellsVoltage){
        batteryObj[0].cellsVoltage.forEach((el,i)=>{
                        refreshLevel(i,(el/4.2));
        });
        document.querySelector(".balanceCap").innerHTML = "<span>BalanceCapacity: </span><span>"+batteryObj[0].BalanceCapacity+" Ah</span>";
        document.querySelector(".rateCap").innerHTML = "<span>RateCapacity: </span><span>"+batteryObj[0].RateCapacity+" Ah</span>";
        document.querySelector(".voltage").innerHTML = "<span>Voltage: </span><span>"+batteryObj[0].voltage+" Volts</span>";
        document.querySelector(".current").innerHTML = "<span>Current: </span><span>"+batteryObj[0].current+" Amps</span>";
        if(batteryObj[0].chargeMosfet){
            document.querySelector(".chrgMosfet").innerHTML = "<span>Charging Mosfet -</span><span>Enabled</span>";
        }else{
            document.querySelector(".chrgMosfet").innerHTML = "<span>Charging Mosfet -</span><span>Disabled</span>";
        }
        if(batteryObj[0].dischargeMosfet){
            document.querySelector(".dsChrgMosfet").innerHTML = "<span>Discharging Mosfet -</span><span>Enabled</span>";
        }else{
            document.querySelector(".dsChrgMosfet").innerHTML = "<span>Discharging Mosfet -</span><span>Disabled</span>";
        }
        document.querySelector(".stateChrg").innerHTML = "<span>stateOfCharge: </span><span>"+batteryObj[0].stateOfCharge+" %</span>";

        document.querySelectorAll("body > div.content > div > div.ntcsStats > span").forEach((el,i)=>{
            el.innerHTML = batteryObj[0].ntcs[el.id.split("ntc")[1]] + "&#176;C";
        }); 
        }
    }
  }
function on2Load(event) {
    initBatteryArray(4);

    initWebSocket();

    getBatteryStatus();
    
    initButtons();
    websocket.onmessage = onMessage;
  }
    on2Load();
  function initButtons() {
  }
  function getBatteryStatus(){
    websocket.send('batteryStatus');
  }

  
  function initBatteryArray(elemNum){
    document.querySelector("#batteryArray").style.width = "100%";
    // console.log(elemNum);
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
  `
for(let i = 0; i< elemNum; i++){
    let div1 = document.createElement("div");
    div1.setAttribute("id",`batteryCell${i}`);
    div1.classList.add("cell");
    div1.innerHTML = elem;
    document.querySelector("#batteryArray").appendChild(div1);
}
const batteryStats = document.querySelector("body > div.content > div > div.batteryStats");

let balanceCap = document.createElement("div");
balanceCap.classList.add("balanceCap");
batteryStats.appendChild(balanceCap);
let rateCap = document.createElement("div");
rateCap.classList.add("rateCap");
batteryStats.appendChild(rateCap);

let divVoltage = document.createElement("div");
divVoltage.classList.add("voltage");
batteryStats.appendChild(divVoltage);
let divCurrent = document.createElement("div");
divCurrent.classList.add("current");
batteryStats.appendChild(divCurrent);
let chrgMosfet = document.createElement("div");
chrgMosfet.classList.add("chrgMosfet");
batteryStats.appendChild(chrgMosfet);
let dsChrgMosfet = document.createElement("div");
dsChrgMosfet.classList.add("dsChrgMosfet");
batteryStats.appendChild(dsChrgMosfet);
let stateChrg = document.createElement("div");
stateChrg.classList.add("stateChrg");
batteryStats.appendChild(stateChrg);

for(let i = 0; i< 3; i++){
    let span2 = document.createElement("span");
    span2.setAttribute("id",`ntc${i}`);
    span2.innerHTML = "tmp";
    document.querySelector(".ntcsStats").appendChild(span2);
}
  }
  let batteryIsCharging = false;
function refreshLevel(elemNumber, level) {
  document.querySelector(`#batteryCell${elemNumber} > div > div.acid-container > div > div`).style.height = (level * document.querySelector(`#batteryCell${elemNumber} > div`).clientHeight) + "px";
 document.querySelector(`#batteryCell${elemNumber} > div > div.acid-container > div > div`).style.setProperty('--acid-height', (level * document.querySelector("#batteryCell0 > div").clientHeight) + "px");

  if (level < .25) {
    document.body.style.setProperty('--background-color', "#c0392b");
  } else if (level < .5) {
    document.body.style.setProperty('--background-color', "#f39c12");
  } else {
    document.body.style.setProperty('--background-color', "#27ae60");
  }
    document.querySelector(`#batteryCell${elemNumber} > div > div.percentage > span`).innerText = (level*4.2).toFixed(3);
}
    )=====";