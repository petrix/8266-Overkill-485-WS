
const char default_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP</title>
  <meta name="viewport" content="width=device-width, initial-scale=1" >
  <link rel="stylesheet" href="style.css" />
<link rel="icon" href="data:,">
<style>
.card > a {
  width: 50%;
  }
</style>
</head>
<body>
  <div class="topnav">
    <h1>ESP</h1>
  </div>
  <div class="content">
    <div class="card">
      <a class="button" href="/battery">Battery Status</a>
      <hr>
      <a class="button" href="/update">Update</a>
      <hr>
      <a class="button" href="/scan">Network Settings</a>
      <hr>
    </div>
  </div>
  %FOOTER%
<script>
  var gateway = "ws://" + window.location.hostname+ "/ws";
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {


  }
  function onLoad(event) {
    initWebSocket();
  }

</script>
</body>
</html>
)rawliteral";
