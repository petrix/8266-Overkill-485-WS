import "./scss/style.scss";
// import "./js/404.scss";
import { dbApp } from "./db-script";

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

const glyphArr = ["home.svg", "cog.svg", "wifi.svg", "update.svg", "menu.svg"];
glyphArr.forEach((elem, i) => {
  loadTheFile(glyphArr[i]).then((icon) => {
    let img = document.createElement("img");
    img.src = icon.b64;
    // img.style.top = "15px";
    if (i > 0) {
      // img.style.right = `${20 + (60 * (i - 1))}px`;
    } else {
      // img.style.left = `20px`;
    }
    document.querySelector(`#glyph${i}`).appendChild(img);
  });
});
function loadTheFile(theName) {
  return new Promise((resolve, reject) => {
    console.log(theName);
    dbApp.checkPublication(theName, "bmsesp-db").then((res) => {
      // console.log(res);
      if (res) {
        dbApp.loadPublication(theName, "bmsesp-db").then((x) => {
          // resolve(x)
          // console.log("xxxxxxxxxxxxxxxxx    ",x);
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
              resolve();
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
      resolve(sc);
    });
  });
}
const jsFiles = [
  "ws.js",
  "btr2initBtrJs.js",
  "btr2refrshlvlJs.js",
  "btr2BtrMsg.js",
  "btr2NetMsg.js",
  "btr2IPmsg.js",
];
var websocket;
var gateway = "ws://" + "bmsesp.local" + "/ws";
async function socktRuner() {
  return new Promise((resolve, reject) => {
    websocket = new WebSocket(gateway);
    websocket.onopen = (e) => {
      console.log("WS opened");
      console.log(e);
    //   websocket.send("batteryStatus");
    //   websocket.send("netscan");
    //   websocket.send("ip");
    resolve(websocket);
    };
    websocket.onclose = (e) => {
      console.log("WS closed");
      console.log(e);
      setTimeout(initWebSocket, 2000);
    };
    
    
  });
  return websocket;
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
  document.querySelector("#footer").innerHTML = `
    <p>Network name : <span id="netName">%SSID%</span></p>
    <p>softAP IP : <span id="softIP">%LOCALIP%</span></p>
    <p>public IP : <span id="pubIP">%PUBLICIP%</span></p>
    `;
  function preloadAssets() {
    return new Promise((resolve, reject) => {
      jsLoader(jsFiles[1]).then(() => {
        jsLoader(jsFiles[2]).then(() => {
          initBatteryArray(4);
          jsLoader(jsFiles[3]).then(() => {
            jsLoader(jsFiles[5]).then(() => resolve());
          });
        });
      });
    });
  }

  socktRuner().then((reslt) => {
    preloadAssets().then(()=>{
        websocket.send("ip");
    });
    console.log(reslt);
    reslt.onmessage = (e) => {
      console.log("WS message");
      let identifier = e.data.slice(0, 3);
      switch (identifier) {
        case "BAT":
          onBTRMessage(e);
          break;
        case "IP-":
          onIPMessage(e);
          break;

        default:
          onIPMessage(e);
          break;
      }
    };
    clearInterval(refreshInt);
    refreshInt = setInterval(() => {
      reslt.send("batteryStatus");
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
  function preloadAssets(params) {
    return new Promise((resolve, reject) => {
      jsLoader(jsFiles[4]).then(() => {
        jsLoader(jsFiles[5]).then(resolve());
      });
    });
  }
  
  socktRuner().then((wbsckt) => {
  preloadAssets().then(()=>{
          websocket.send("netscan");
          websocket.send("ip");
  });  
    console.log(wbsckt);
    wbsckt.onmessage = (e) => {
      console.log("WS message");
      console.log(e.data.slice(0, 3));
      let identifier = e.data.slice(0, 3);
      switch (identifier) {
        case "SCN":
          onNetMessage(e);
          break;
        case "IP-":
          onIPMessage(e);
          break;
        default:
          onIPMessage(e);
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
    <hr /><button class="button" id="credits" disabled="disabled">Connect</button>
    </div>
    `;
  // document.querySelector("#netscan").onmousedown = websocket.send('netscan')
  document.querySelector("#netscan").addEventListener("mousedown", (e) => {
    websocket.send("netscan");
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
  function preloadAssets(params) {
    return new Promise((resolve, reject) => {
    //   jsLoader(jsFiles[4]).then(() => {
        jsLoader(jsFiles[5]).then(resolve());
    //   });
    });
  }
  
  socktRuner().then((reslt) => {
   preloadAssets().then(()=>{
    reslt.send("ip");
   }); 
    console.log(reslt);
    reslt.onmessage = (e) => {
      console.log("WS message");
      console.log(e.data);
      let identifier = e.data.slice(0, 3);
      switch (identifier) {
        case "IP-":
          onIPMessage(e);
          break;

        default:
          break;
      }
    };
  });

  document.querySelector("#card").innerHTML = `
    <form method="POST" action="http://bmsesp.local/update" enctype="multipart/form-data">
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
  document.querySelector("#card").innerHTML = `
    <p><button id="readCashed" class="button">Read Cashed Elems</button></p>
    <div id="cashedElems">
    </div>

    `;

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

  function preloadAssets(params) {
    return new Promise((resolve, reject) => {
      jsLoader(jsFiles[5]).then(resolve());
    });
  }
  
  socktRuner().then((reslt) => {
   preloadAssets().then(()=>{
    websocket.send("ip");
   }); 
    console.log(reslt);
    reslt.onmessage = (e) => {
      console.log("WS message");
      // console.log(e.data);
      let identifier = e.data.slice(0, 3);
      switch (identifier) {
        case "IP-":
          onIPMessage(e);
          break;

        default:
          onIPMessage(e);
          break;
      }
    };
  });
} else {
  console.log(window.location.pathname);
  // import "./js/404.scss";
  document.querySelector("#card").innerHTML = `<div title="404">404</div>`;
}
