
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
  <div class="percentage"><span class="percentagexxx">0</span> v</div>
  </div>
  `


    for(let i = 0; i< elemNum; i++){
        let div1 = document.createElement("div");
        div1.setAttribute("id",`batteryCell${i}`);
        div1.classList.add("cell");
        div1.innerHTML = elem;
        document.querySelector("#batteryArray").appendChild(div1);
    }

    btrSt.cellSum = document.createElement("div");
    btrSt.cellSum.classList.add("cellSum");
    btrSt.appendChild(btrSt.cellSum);
    btrSt.cellSum.name = document.createElement("span");
    btrSt.cellSum.name.classList.add("cellSumName");
    btrSt.cellSum.name.innerHTML = "cellSum:"
    btrSt.cellSum.appendChild(btrSt.cellSum.name);
    btrSt.cellSum.value = document.createElement("span");
    btrSt.cellSum.value.classList.add("cellSumValue");
    btrSt.cellSum.appendChild(btrSt.cellSum.value);

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
