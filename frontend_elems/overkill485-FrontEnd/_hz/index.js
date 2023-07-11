import "./js/style.scss";
// import "./js/404.scss";
let batteryObj;
let refreshInt;
// import "./js/styleBattery.scss";
// import "./js/checkbox.scss";

// import {  } from "./js/btr2initBtrJs";
// import {  } from "./js/btr2refrshlvlJs";

// import { onMessage } from "./js/btr2onmsgJs";


async function jsLoader(name){
    return new Promise(function (resolve, reject) {
        fetch("/js/"+name+'.js'/*, {
          headers: {
            "Content-Type": "application/octet-stream",
          },
          credentials: 'include'
            }*/).then(res=>{
        if(res.ok){
           return res.text(); 
        }
        }).then(txt=>{
        /*var objectURL = URL.createObjectURL(blob); */
            var sc = document.createElement("script");
           /* sc.setAttribute("src", objectURL);*/ 
           sc.innerHTML = txt;
            sc.async = false;
            sc.setAttribute("type", "text/javascript"); 
            // sc.setAttribute("defer", "defer");
            document.head.appendChild(sc);
            resolve(sc);
        });
    });
}
const jsFiles = ["ws","btr2initBtrJs","btr2refrshlvlJs","btr2BtrMsg","btr2NetMsg","btr2IPmsg"];

async function scktRuner (){
return new Promise((resolve, reject) => {
    let sckt = jsLoader(jsFiles[0]).then(()=>{
    console.log(sckt);
    websocket.onopen = (e)=>{
    console.log("WS opened");
    console.log(e);
    websocket.send('batteryStatus');
    websocket.send('netscan');
    websocket.send('ip');
    };
    websocket.onclose = (e)=>{
    console.log("WS closed");
    console.log(e);
    setTimeout(initWebSocket, 2000);
    };
return websocket;
    }).then(res => resolve(res));

})

}



if(window.location.pathname == "/"){
    console.log(window.location.pathname);

    document.querySelector("#card").innerHTML = `
    <div id="batteryArray" class="batteryArray"></div>
        <div class="ntcsStats"></div>
        <div class="batteryStats"></div> `
    let initBtr = jsLoader(jsFiles[1]).then(e=>{

        console.log(initBtr)
let btrArray = initBatteryArray(4);
console.log(btrArray);
}).then(e=>{
       let z = jsLoader(jsFiles[2]);
}).then(e=>{
       let btrmsg = jsLoader(jsFiles[3]);
}).then(e=>{
       let ipmsg = jsLoader(jsFiles[5]);
});

     scktRuner().then(reslt => {
        console.log(reslt)
        reslt.onmessage = (e)=>{
            console.log("WS message");
            let identifier = e.data.slice(0,3);
            switch (identifier) {
                case "BAT":
                    onBTRMessage(e);
                break;
                case "IP-":
                    onIPMessage(e);
                 break;
            
                default:
                    break;
            }
        };
        clearInterval(refreshInt);
        refreshInt = setInterval(() => {
            reslt.send('batteryStatus');
        }, 1300);
    });
    

 

    // let sckt = await jsLoader(jsFiles[0]);
    // console.log(sckt);






    // document.querySelector("#chrgMosfetValue").addEventListener("change",(e)=>{

    //     console.log(e.target.checked);
    //                     console.log(e);
    //     websocket.send('MSFT--chr-'+e.target.checked);
    // });


}else if(window.location.pathname == "/netscan"){
    scktRuner().then(wbsckt => {
        console.log(wbsckt)
        wbsckt.onmessage = (e)=>{
            console.log("WS message");
            console.log(e.data.slice(0,3));
            let identifier = e.data.slice(0,3);
            switch (identifier) {
                case "SCN":
                    onNetMessage(e);
                    break;
                    case "IP-":
                        onIPMessage(e);
                        break;
            
                default:
                    break;
            }
            
        };
            }).then(()=>{
                    let netmsg = jsLoader(jsFiles[4]);
    console.log(netmsg);
            }).then(()=>{
                    let ipmsg = jsLoader(jsFiles[5]);
    console.log(ipmsg);
            });


    document.querySelector("#card").innerHTML = `
    <p><button id="button" class="button">Scan</button></p>
    <h2>Scaned Stations</h2>
    <div id="scanStations" class="scanStations"></div>
    <h4>Connect to network:</h4>
    <div class="credentials">
    <input id="ssidInput" type='text' placeholder='network' name='n'/>
    <hr /><input id="ssidPassword" type='password' placeholder='password' name='p'/>
    <hr /><button class="button" id="credits" disabled="disabled">Connect</button>
    </div>
    `

}else if(window.location.pathname == "/update"){
    scktRuner().then(reslt => {
        console.log(reslt)
        reslt.onmessage = (e)=>{
        console.log("WS message");
        console.log(e.data);
        let identifier = e.data.slice(0,3);
        switch (identifier) {
                case "IP-":
                    onIPMessage(e);
                    break;
        
            default:
                break;
        }
    };
    
    });
    let ipmsg = jsLoader(jsFiles[5]);
    console.log(ipmsg);

    document.querySelector("#card").innerHTML = `
    <form method="POST" action="/update" enctype="multipart/form-data">
    <input class="button" type="file" name="update">
    <input class="button" type="submit" value="Update"></form>`



}else if(window.location.pathname == "/settings"){
    scktRuner().then(reslt => {
        console.log(reslt)
        reslt.onmessage = (e)=>{
        console.log("WS message");
        console.log(e.data);
        let identifier = e.data.slice(0,3);
        switch (identifier) {
            case "IP-":
                onIPMessage(e);
             break;
        
            default:
                break;
        }
    };
    
    });
    let ipmsg = jsLoader(jsFiles[5]);
    console.log(ipmsg);

}else{
    console.log(window.location.pathname);
    // import "./js/404.scss";
    document.querySelector("#card").innerHTML =`<div title="404">404</div>`
}
//     let btrSt = {};
// if(window.location.pathname =="/"){
//     console.log(window.location);
//         loader(jsFiles[0]).then(loader(jsFiles[1]).then(loader(jsFiles[2]).then(loader(jsFiles[3]).then(e=>console.log(jsFiles[3],e)))));
// }else if (window.location.pathname == "/netscan.html") {
    
//     loader(jsFiles[2]).then(loader(jsFiles[3]).then(e=>console.log(e)));
// }
