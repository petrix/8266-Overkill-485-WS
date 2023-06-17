
// var gateway="ws://" + window.location.hostname + "/ws"; 
var gateway="ws://" + "bmsesp.local" + "/ws"; 
var websocket; 

let int;
function initWebSocket(){
    console.log('Trying to open a WebSocket connection...'); 
    websocket=new WebSocket(gateway);
    // websocket.onopen=onOpen; 
    // websocket.onclose=onClose;
    }
// function onOpen(event){
//     console.log('Connection opened');
//         // websocket.onmessage = onMessage;
//         // websocket.send('batteryStatus');
//         // initBatteryArray(4)
        
//         // clearInterval(int);
//         // int = setInterval(()=>{
//         //     websocket.send('batteryStatus');
//         // }, 2000);
//     }
// function onClose(event){
//     console.log('Connection closed'); 
//     // clearInterval(int);
//     // setTimeout(initWebSocket, 2000);
//     }
initWebSocket();

