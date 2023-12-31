const onBTRMessage = (event) => {
  const data = event.data;
  // console.log(data);
  if (data.slice(0, 3) === "BAT") {
    const payload = data.slice(3);
    const result = JSON.parse(payload);
    /*console.log(result);*/
    batteryObj = result[0];
    console.log(batteryObj);
    if (batteryObj.cellsVoltage) {
      let sum = 0.0;
      batteryObj.cellsVoltage.forEach((el, i) => {
        // console.log(el, i);
        // let xx = el - 3;
        let x = el - 3;
        refreshLevel(i, x / 1.15, el);
        sum += el;
      });
        
        let cells = document.querySelectorAll(".cell");

      batteryObj.cellsBalance.forEach((el, i) => {

        // console.log(el, i);
        // let xx = el - 3;
        // el.querySelector('.battery').classList.add('balancing');
        if(el){
            cells[i].querySelector(".battery").classList.add("balancing")
        }else{
            cells[i].querySelector(".battery").classList.remove("balancing")
        }
      });
      console.log(sum.toFixed(3));
      const btrSt = document.btrSt;
      btrSt.cellSum.value.innerHTML = sum.toFixed(3)+ " v";
      btrSt.balanceCap.value.innerHTML = batteryObj.BalanceCapacity + " ah";
      btrSt.rateCap.value.innerHTML = batteryObj.RateCapacity + " ah";
      btrSt.divVoltage.value.innerHTML = batteryObj.voltage + " v";
      btrSt.divCurrent.value.innerHTML = batteryObj.current + " a";
      if(batteryObj.current<0){
        btrSt.divInputPower.value.innerHTML = "0 w";
        btrSt.divOutputPower.value.innerHTML = (batteryObj.voltage * Math.abs(batteryObj.current)).toFixed(2)+ " w";
      }else if(batteryObj.current>0){
        btrSt.divInputPower.value.innerHTML = (batteryObj.voltage * batteryObj.current).toFixed(2) + " w";
        btrSt.divOutputPower.value.innerHTML = "0 w";
      }else{
        btrSt.divInputPower.value.innerHTML = "0 w";
        btrSt.divOutputPower.value.innerHTML = "0 w";
      }
      btrSt.divVoltDiff.value.innerHTML =
        (
          Math.max.apply(null, batteryObj.cellsVoltage) -
          Math.min.apply(null, batteryObj.cellsVoltage)
        ).toFixed(3) + " v";

      document.querySelector("#chrgMosfetValue").checked =
        batteryObj.chargeMosfet;
      document.querySelector("#dsChrgMosfetValue").checked =
        batteryObj.dischargeMosfet;
      // document.querySelector("#chrgMosfetValue").onclick = e=>e.preventDefault;

      btrSt.stateChrg.value.innerHTML = batteryObj.stateOfCharge + " %";

      document
        .querySelector("body > div.content > div > div.ntcsStats")
        .childNodes.forEach((el, i) => {
          el.innerHTML = batteryObj.ntcs[i] + "&#176;C";
        });
    }
    let d = new Date(parseInt(batteryObj.currentTime + "000"));
    let hrs = d.getHours();
    if (hrs < 10) hrs = "0" + hrs;
    let mnts = d.getMinutes();
    if (mnts < 10) mnts = "0" + mnts;
    let secnds = d.getSeconds();
    if (secnds < 10) secnds = "0" + secnds;
    const btrSt = document.btrSt;
    btrSt.currentTime.value.innerHTML = `${hrs}:${mnts}:${secnds}`;
    if (batteryObj.current > 0) {
      document.querySelector("h1").innerHTML = "CHARGING!!!";
      document.querySelectorAll(".acid-container > .acid").forEach((elem) => {
        elem.style.filter = "hue-rotate(222deg)";
        document.querySelector("h1").classList.add("charging");
        document.querySelector("h1").classList.remove("discharging");
      });
    } else if (batteryObj.current < 0) {
      document.querySelector("h1").innerHTML = "DISCHARGING!!!";
      document.querySelectorAll(".acid-container > .acid").forEach((elem) => {
        elem.style.filter = "hue-rotate(0deg)";
        document.querySelector("h1").classList.remove("charging");
        document.querySelector("h1").classList.add("discharging");
      });
    } else {
      document.querySelector("h1").classList.remove("discharging");
      document.querySelector("h1").classList.remove("charging");
      document.querySelector("h1").innerHTML = "..idle";
      document.querySelectorAll(".acid-container > .acid").forEach((elem) => {
        elem.style.filter = "hue-rotate(120deg)";
      });
    }
  } else {
    console.log(data);
  }
};
