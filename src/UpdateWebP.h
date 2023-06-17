
const char webUpdate_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <title>webUpdate</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="style.css" />
    <link rel="icon" href="data:,">
  </head>
  <body>
    %HEAD%
    <div class="content">
      <div class="card">
        <form method="POST" action="/update" enctype="multipart/form-data">
          <input class="button" type="file" name="update">
          <input class="button" type="submit" value="Update"></form>
      </div>
    </div>
    %FOOTER%
  <script>
  var gateway = `ws://${window.location.hostname}/ws`;
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




  const char webUpdate_ok_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
  <title>webUpdate</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
 <link rel="stylesheet" href="style.css" />
  <link rel="icon" href="data:,">
  </head>
  <body>
  <div class="topnav">
  <h1>webUpdate</h1>
  </div>
  <div class="content">
  <div class="card">

      <div class="button">SUCCESS</div>

  </div>
  </div>
  <script>
  var gateway = `ws://${window.location.hostname}/ws`;
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
    setTimeout(()=>{window.location.href = "/";}, 4000);
  }
  </script>
  </body>
  </html>
  )rawliteral";






    const char webUpdate_fail_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
  <title>webUpdate</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
 <link rel="stylesheet" href="style.css" />
  <link rel="icon" href="data:,">
  </head>
  <body>
  <div class="topnav">
  <h1>webUpdate</h1>
  </div>
  <div class="content">
  <div class="card">

      <div class="button">FAIL</div>

  </div>
  </div>
  <script>
  var gateway = `ws://${window.location.hostname}/ws`;
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
    setTimeout(()=>{window.location.href = "/";}, 4000);
  }
  </script>
  </body>
  </html>
  )rawliteral";
