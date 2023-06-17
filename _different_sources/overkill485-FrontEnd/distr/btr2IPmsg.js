
const onIPMessage = (event) => {
    const data = event.data;
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

            // document.querySelector("body > div.footer > p:nth-child(2) > span").innerHTML = ip[1];
            // ipObj.setAttribute(obj.split(":-")[0], obj.split(":-")[1]);
        }
        //    let ipObj = 
        //    let ip=payload.split("--local:-")[1].split("-softap:-");
        //    document.querySelector("body > div.footer > p:nth-child(1) > span").innerHTML = ip[1];
        //    document.querySelector("body > div.footer > p:nth-child(2) > span").innerHTML = ip[0];
    } else {
        console.log(data);
    }

}

