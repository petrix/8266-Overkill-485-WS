export const onIPMessage = {};
onIPMessage.exec = (event) => {
  const data = event.data;
  console.log("IP Messages================================================");
  // console.log(data);
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
        case "hostname":
          document.querySelector(
            "#hostname"
          ).innerHTML = `<a href="http://${value}.local">http://${value}.local</a>`;
          break;

        default:
          break;
      }
    }
  } else if (data.slice(0, 3) === "BAT") {
    console.log("BAT");
    let partOne = data.split(`current":`)[1];
    let partTwo = partOne.split(",")[0];
    let current = JSON.parse(partTwo);
    console.log(current);
    if (current > 0.2) {
      document.querySelector("h1").classList.add("charging");
    } else if (current < 0) {
      document.querySelector("h1").classList.add("discharging");
    } else if (current == 0) {
      document.querySelector("h1").classList.remove("charging");
      document.querySelector("h1").classList.remove("discharging");
    }
  } else if (data.slice(0, 3) === "CON") {
    console.log("CON");
    let crd = data.split("--")[1];
    let ssid = crd.split("-passwd-")[0];
    let ssidPwd = crd.split("-passwd-")[1];
    document.querySelector("#scanStations").innerHTML =
      "Connected to " + ssid + "<br>" + "with password : " + ssidPwd;
  } else if (data.slice(0, 3) === "NCS") {
    console.log("NCS");
    let arr1 = data.split("-:");
    let ssid = arr1[0].split(":-")[1];
    let ssidPwd = arr1[1].split(":-")[1];
    let hostName = `http://${arr1[2].split(":-")[1]}.local`;
    document.querySelector(
      "#scanStations"
    ).innerHTML = `Connected to ${ssid} with password : ${ssidPwd}<br> hostname : ${hostName}`;
  } else {
    console.log(data);
  }
};
