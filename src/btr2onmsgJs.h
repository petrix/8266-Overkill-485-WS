const char btr2onmsgJs_file[] PROGMEM = R"=====(
function onMessage(event) {
    let data = event.data;
    /* console.log(data);*/
    if(data.slice(0,3) == "BAT"){
      let payload = data.slice(3);
      let result = JSON.parse(payload);
      batteryObj = result[0];
            /* console.log(batteryObj);*/
            if(batteryObj.cellsVoltage){
        batteryObj.cellsVoltage.forEach((el,i)=>{
                        refreshLevel(i,(el/4.2));
        });
        function getQS(name){
            return document.querySelector("."+name);
        }
        getQS("balanceCap").innerHTML = "<span>BalanceCapacity: </span><span>"+batteryObj.BalanceCapacity+" Ah</span>";
        getQS("rateCap").innerHTML = "<span>RateCapacity: </span><span>"+batteryObj.RateCapacity+" Ah</span>";
        getQS("voltage").innerHTML = "<span>Voltage: </span><span>"+batteryObj.voltage+" Volts</span>";
        getQS("current").innerHTML = "<span>Current: </span><span>"+batteryObj.current+" Amps</span>";
        if(batteryObj.chargeMosfet){
            getQS("chrgMosfet").innerHTML = "<span>Charging Mosfet -</span><span>Enabled</span>";
        }else{
            getQS("chrgMosfet").innerHTML = "<span>Charging Mosfet -</span><span>Disabled</span>";
        }
        if(batteryObj.dischargeMosfet){
            getQS("dsChrgMosfet").innerHTML = "<span>Discharging Mosfet -</span><span>Enabled</span>";
        }else{
            getQS("dsChrgMosfet").innerHTML = "<span>Discharging Mosfet -</span><span>Disabled</span>";
        }
        getQS("stateChrg").innerHTML = "<span>stateOfCharge: </span><span>"+batteryObj.stateOfCharge+" </span>";
        document.querySelectorAll("body > div.content > div > div.ntcsStats > span").forEach((el,i)=>{
            /*el.innerHTML = batteryObj.ntcs[i] + "&#176;C";*/
        }); 
        }
        getQS("currentTime").innerHTML = "<span>currentTime -</span><span>"+batteryObj.currentTime+"</span>";


    }
  }
)=====";