import "./scss/style.scss";
// import "./js/404.scss";
import { dbApp } from "./db-script";

import { onIPMessage } from "./onIPMessage";
import { onNetMessage } from "./onNetMessage";
let batteryObj;
let refreshInt;
let rndName = "pwa-x192.png";

function initDB(params) {
  return new Promise((resolve, reject) => {
    dbApp.init("bmsesp-db").then((e) => {
      console.log("db", e);
      resolve();
    });
  });
}
initDB();

document.querySelector("h1").innerText =
  window.location.pathname.split("/")[1].charAt(0).toUpperCase() +
  window.location.pathname.split("/")[1].slice(1);

const glyphArr = [
  "home.svg",
  "cog.svg",
  "wifi.svg",
  "update.svg",
  "menu.svg",
  "terminal.svg",
];
glyphArr.forEach((elem, i) => {
  loadTheFile(glyphArr[i]).then((icon) => {
    let img = document.createElement("img");
    img.src = icon.b64;
    let blb = icon.blob;
    let b64 = icon.b64;
    blb.text().then((e) => console.log(b64.length / e.length));

    document.querySelector(`#glyph${i}`).appendChild(img);
  });
});
function loadTheFile(theName) {
  return new Promise((resolve, reject) => {
    console.log(theName);
    dbApp.checkPublication(theName, "bmsesp-db").then((res) => {
      if (res) {
        dbApp.loadPublication(theName, "bmsesp-db").then((x) => {
          resolve(x);
        });
      } else {
        dbApp.loadImage(theName).then((res) => {
          if (res) {
            if (res.blob.type != "" || res.blob.type != "text/html") {
              dbApp
                .addPublication(theName, "bmsesp-db", res.blob, res.b64)
                .then(() => {
                  resolve(res);
                });
            } else {
              //   resolve();
            }
          }
        });
      }
    });

    /*  if(!res){
                  fetch(`./${rndName}`).then(res=>console.log(res));
              }*/
  });
}

async function jsLoader(name) {
  return new Promise(function (resolve, reject) {
    //    dbApp.checkPublication(name+".js");
    console.log("======================= : ", name);
    // fetch(name+'.js').then(res=>{
    loadTheFile(name).then((res2) => {
      // console.log("+++++++++++++++++",res2.b64);
      /*var objectURL = URL.createObjectURL(blob); */
      var sc = document.createElement("script");
      /* sc.setAttribute("src", objectURL);*/

      let blb = res2.blob;
      blb.text().then((e) => {
        sc.innerHTML = e;
      });

      sc.async = false;
      // sc.setAttribute("type", "text/javascript");
      // sc.setAttribute("defer", "defer");
      console.log(sc);
      document.head.appendChild(sc);
      resolve(blb.text());
      //   resolve(sc);
    });
  });
}
const jsFiles = [
  "onIPMessage.js",
  "initBatteryArray.js",
  "btr2refrshlvlJs.js",
  "onBTRMessage.js",
  "onNetMessage.js",
];
var wSock = document.wSock;
// var gateway = "ws://" + "async-x.local" + "/ws";
// var gateway = "ws://" + "esp-async.local" + "/ws";
// var gateway = "ws://" + window.location.hostname + "/ws";
// var gateway1 = "ws://" + window.location.hostname + "/ws";
var gateway2 = "ws://" + "bmsesp2.local" + "/ws";
async function socktRuner() {
  return new Promise((resolve, reject) => {
    wSock = new WebSocket(gateway2);
    wSock.onopen = (e) => {
      console.log("WS opened");
      console.log(e);
      resolve(wSock);
    };
    wSock.onclose = (e) => {
      console.log("WS closed");
      console.log(e);
      setTimeout(socktRuner, 2000);
    };
  });
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////                          //////////////////////
////////////////////        INDEX PAGE        //////////////////////
////////////////////                          //////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
if (window.location.pathname == "/") {
  console.log(window.location.pathname);

  document.querySelector("#card").innerHTML = `
    <div id="batteryArray" class="batteryArray"></div>
        <div class="ntcsStats"></div>
        <div class="batteryStats"></div> 
    `;

  function preloadAssets() {
    return new Promise((resolve, reject) => {
      jsLoader(jsFiles[1]).then(() => {
        jsLoader(jsFiles[2]).then(() => {
          initBatteryArray(4);
          jsLoader(jsFiles[3]).then((e) => resolve(e));
        });
      });
    });
  }

  socktRuner().then((sockt) => {
    preloadAssets().then(() => {
      wSock.send("ip");
      wSock.send("batteryStatus");
      let chrMSFT = document.querySelector("#chrgMosfetValue");
      let dscMSFT = document.querySelector("#dsChrgMosfetValue");
      chrMSFT.onclick = (e) => {
        console.log(e.target);
        if (!e.target.checked) {
          let conf = confirm("are you shure?");
          if (conf) {
            wSock.send("MSFT--chr-false");
          }
        } else {
          wSock.send("MSFT--chr-true");
        }
      };
      dscMSFT.onclick = (e) => {
        console.log(e.target);
        if (!e.target.checked) {
          let conf = confirm("are you shure?");
          if (conf) {
            wSock.send("MSFT--dsc-false");
          }
        } else {
          wSock.send("MSFT--dsc-true");
        }
      };
    });
    console.log(sockt);
    sockt.onmessage = (e) => {
      console.log("WS message");
      let identifier = e.data.slice(0, 3);
      switch (identifier) {
        case "BAT":
          onBTRMessage(e);
          break;
        case "IP-":
          onIPMessage.exec(e);
          break;

        default:
          onIPMessage.exec(e);
          break;
      }
    };
    clearInterval(refreshInt);
    refreshInt = setInterval(() => {
      sockt.send("batteryStatus");
    }, 1300);
  });

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////                          //////////////////////
  ////////////////////        NETSCAN           //////////////////////
  ////////////////////                          //////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
} else if (window.location.pathname == "/netscan") {
  socktRuner().then((sockt) => {
    wSock.send("netscan");
    wSock.send("ip");
    wSock.send("batteryStatus");

    console.log(sockt);
    sockt.onmessage = (e) => {
      console.log("WS message");
      console.log(e.data.slice(0, 3));
      let identifier = e.data.slice(0, 3);
      switch (identifier) {
        case "SCN":
          onNetMessage.exec(e);
          break;
        case "IP-":
          onIPMessage.exec(e);
          break;
        default:
          onIPMessage.exec(e);
          break;
      }
    };
    //   websocket.send("batteryStatus");
    //   websocket.send("netscan");
    //   websocket.send("ip");
  });

  document.querySelector("#card").innerHTML = `
    <p><button id="netscan" class="button">Scan</button></p>
    <h2>Scaned Stations</h2>
    <div id="scanStations" class="scanStations">
    <span class="loader-89"></span>
    </div>
    <h4>Connect to network:</h4>
    <div class="credentials">
    <input id="ssidInput" type='text' placeholder='network' name='n'/>
    <hr /><input id="ssidPassword" type='password' placeholder='password' name='p'/>
    <hr /><button class="button" id="credits">Connect</button>
    </div>
    `;
  // document.querySelector("#netscan").onmousedown = websocket.send('netscan')
  document.querySelector("#netscan").addEventListener("mousedown", (e) => {
    wSock.send("netscan");
    document.querySelector("#scanStations").innerHTML = "";
    let preloader = document.createElement("span");
    preloader.classList.add("loader-89");
    document.querySelector("#scanStations").appendChild(preloader);
  });
  document.querySelector("#credits").addEventListener("mousedown", (e) => {
    let ssid = document.querySelector("#ssidInput").value;
    let ssidPasswd = document.querySelector("#ssidPassword").value;
    wSock.send("PWD--" + ssid + "-passwd-" + ssidPasswd);
    document.querySelector("#scanStations").innerHTML = "";
    let preloader = document.createElement("span");
    preloader.classList.add("loader-89");
    document.querySelector("#scanStations").appendChild(preloader);
  });

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////                          //////////////////////
  ////////////////////        UPDATE            //////////////////////
  ////////////////////                          //////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
} else if (window.location.pathname == "/update") {
  socktRuner().then((reslt) => {
    // console.log(e)
    reslt.send("ip");
    reslt.send("batteryStatus");

    console.log(reslt);
    reslt.onmessage = (e) => {
      console.log("WS message");
      console.log(e.data);
      let identifier = e.data.slice(0, 3);
      switch (identifier) {
        case "IP-":
          onIPMessage.exec(e);
          break;

        default:
          onIPMessage.exec(e);
          break;
      }
    };
  });

  document.querySelector("#card").innerHTML = `

    <form method="POST" action="/update" enctype="multipart/form-data">
    <input class="button" type="file" accept=".bin" name="update">
    <input class="button" type="submit" value="Update"></form>`;

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
} else if (window.location.pathname == "/terminal") {
  socktRuner().then((reslt) => {
    // console.log(e)
    reslt.send("ip");
    reslt.send("batteryStatus");

    console.log(reslt);
    reslt.onmessage = (e) => {
      console.log("WS message");
      console.log(e.data);
      let identifier = e.data.slice(0, 3);
      switch (identifier) {
        case "IP-":
          onIPMessage.exec(e);
          break;

        default:
          onIPMessage.exec(e);
          break;
      }
    };
  });

  document.querySelector("#card").innerHTML = `
   <form method="POST" action="/update" enctype="multipart/form-data">
    <input class="button" type="file" accept=".bin" name="update">
    <input class="button" type="submit" value="Update"></form>`;

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
} else if (window.location.pathname == "/settings") {
  socktRuner().then((reslt) => {
    // console.log(e);
    wSock.send("ip");
    wSock.send("batteryStatus");

    console.log(reslt);
    reslt.onmessage = (e) => {
      console.log("WS message");
      // console.log(e.data);
      let identifier = e.data.slice(0, 3);
      switch (identifier) {
        case "IP-":
          onIPMessage.exec(e);
          break;

        default:
          onIPMessage.exec(e);
          break;
      }
    };
  });

  document.querySelector("#card").innerHTML = `
    <p><button id="readCashed" class="button">Read Cashed Elems</button></p>
    <div id="cashedElems">
    </div>`;

  const cashedElems = document.querySelector("#cashedElems");
  document.querySelector("#readCashed").addEventListener("mousedown", (e) => {
    cashedElems.innerHTML = "";
    dbApp.readAllDB("bmsesp-db").then((e) => {
      printResult(e);
    });
  });
  function printResult(elem) {
    console.log(elem, elem.length);
    for (let index = 0; index < elem.length; index++) {
      const element = elem[index];

      const monthArr = [
        "jan",
        "feb",
        "mar",
        "apr",
        "may",
        "jun",
        "jul",
        "aug",
        "sep",
        "oct",
        "nov",
        "dec",
      ];
      let row = document.createElement("div");
      row.setAttribute("class", "row");

      let title = document.createElement("div");
      title.innerText = element.title;
      title.setAttribute("class", "row__title");
      let b64 = document.createElement("div");
      b64.innerText = element.b64.length;
      b64.setAttribute("class", "row__b64");
      let blob = document.createElement("div");
      blob.setAttribute("class", "row__blob");
      let blobType = document.createElement("div");
      blobType.innerHTML =
        element.blob.type.split("/")[0] +
        "<br>" +
        element.blob.type.split("/")[1];
      blobType.setAttribute("class", "row__blob__type");
      let blobSize = document.createElement("div");
      blobSize.innerText = element.blob.size;
      blobSize.setAttribute("class", "row__blob__size");
      let date = document.createElement("div");
      let d = new Date(element.dateModified);
      date.innerHTML = `${d.getDate()} ${
        monthArr[d.getMonth()]
      } ${d.getFullYear()}<br>${d.getHours()}:${d.getMinutes()}:${d.getSeconds()}`;
      date.setAttribute("class", "row__date");
      row.appendChild(title);
      row.appendChild(b64);
      blob.appendChild(blobSize);
      blob.appendChild(blobType);
      row.appendChild(blob);
      row.appendChild(date);
      row.childNodes.forEach((el) =>
        el.childNodes.forEach((ch) => (ch.ariaLabel = "row-" + (index + 1)))
      );
      row.childNodes.forEach((el) => (el.ariaLabel = "row-" + (index + 1)));
      row.ariaLabel = "row-" + (index + 1);
      row.onmousedown = (e) => {
        // console.log(e.parentElement.parentElement.ariaLabel)
        // console.log(e.parentElement.ariaLabel)
        console.log(e.target.ariaLabel);
        let lbl = e.target.ariaLabel;
        let selected = document.querySelector(
          `#cashedElems > [aria-label=${lbl}]`
        );
        let a = confirm("shure?");
        console.log(a);
        if (a) {
          dbApp.deletePublication("bmsesp-db", selected.firstChild.innerText);
          console.log(selected.firstChild.innerText);
          selected.remove();
        }
      };
      cashedElems.appendChild(row);
    }
  }
} else {
  console.log(window.location.pathname);
  // import "./js/404.scss";
  document.querySelector("#card").innerHTML = `<div title="404">404</div>`;
}
