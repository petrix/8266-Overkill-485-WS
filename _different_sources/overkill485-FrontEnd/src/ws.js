
// var gateway="ws://" + window.location.hostname + "/ws"; 
var gateway="ws://" + "bmsesp.local" + "/ws"; 
var websocket; 
let int;
function initWebSocket(){
    console.log('Trying to open a WebSocket connection...'); 
    websocket=new WebSocket(gateway);
    }
initWebSocket();

