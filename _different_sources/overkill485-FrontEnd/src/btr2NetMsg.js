
const onNetMessage = (event) => {
    const data = event.data;
    
    if (data.slice(0, 3) === "SCN") {
        // console.table(data.slice(0, 3),"============received message");
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
            ssid.classList.add("ssid")
            p.appendChild(ssid);

            let params = document.createElement('div');
            params.classList.add("params");


            let rssi = document.createElement("span");
            rssi.innerText = stations[i].rssi + "dBm";
            params.appendChild(rssi);
            //      let bssid = document.createElement("span");
            //      bssid.innerText = stations[i].bssid;
            //      p.appendChild(bssid);
            let channel = document.createElement("span");
            channel.innerText = "ch:" + stations[i].channel;
            params.appendChild(channel);
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
            params.appendChild(secure);
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
            params.appendChild(bssid);
            p.appendChild(params);

            p.onclick = (e) => {
                let ssid_name =
                  e.target.parentElement.parentElement.getAttribute("label");

                if (!ssid_name || ssid_name == null) {
                    ssid_name = e.target.parentElement.getAttribute("label");
                }
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

