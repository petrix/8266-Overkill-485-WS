 const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="ru">
<head>
  <title>Battery Status</title>

  <meta charset="UTF-8">
  <meta name="author" content=".p3xx" />
<meta name="viewport" content="width=device-width, initial-scale=1" >
<meta http-equiv="X-UA-Compatible" content="ie=edge">
    <link rel="icon" type="image/png" sizes="192x192" href="data:image/png;base64,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">
    <link rel="shortcut icon" href="data:image/png;base64,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">
    <meta name="theme-color" content="#311">
    <meta name="msapplication-config" content="./favicons/browserconfig.xml">

    <meta name="apple-mobile-web-app-status-bar" content="#311">

    <meta name="apple-mobile-web-app-title" content="BMSESP">
    <meta name="application-name" content="BMSESP">
    <meta name="theme-color" content="#311">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="format-detection" content="telephone=yes">

    <meta property="og:type" content="p3xx.bmsesp.production">
    <meta property="og:title" content="BMSESP">
    <meta property="og:url" content="bmsesp.local">
    
  <script type="module" crossorigin src="./someshit/index-js.js"></script>
  <link rel="stylesheet" href="./someshit/index-css.css">
  <style>
  body{
    background-color: #333;
  }
  </style>
</head>
<body>
   <div class="topnav">
    <a id="glyph0" href="/">
    </a>
    <h1>...</h1>
    <a id="glyph3" href="/update">
    </a>
    <a id="glyph2" href="/netscan">
    </a>
    <a id="glyph1" href="/settings">
    </a>
    <script>
/*
    window.onload=()=>{
        const glyphArr = ["home","cog","wifi","update"];
        function imgPreload(imgName, num) {
            return new Promise((resolve, reject) => {
                        fetch(`./${imgName}.svg`).then(svg=>{
                           if(svg.ok){return svg.blob()}
                           else{reject(imgName)}
                        }).then(e=>{
            let url = URL.createObjectURL(e);
            let img = document.createElement("img");
            img.src = url;
            img.style.top = "15px";
            if(num>0){
                img.style.right = `${20 + (60*(num-1))}px`;
            }else{
                img.style.left = `20px`;
            }
            document.querySelector(`#glyph${num}`).appendChild(img);
        }).then(resolve);
            })
        }
        for (let i = 0; i<glyphArr.length;i++) {
            imgPreload(glyphArr[i],i).then(e=>console.log(e));
        }

        document.querySelector('h1').innerText = window.location.pathname.split('/')[1].charAt(0).toUpperCase() + window.location.pathname.split('/')[1].slice(1);
        // document.querySelector("body > div.topnav > script").remove();
    }
*/    
    </script>
  </div>
  <div class="content">
    <div id="card">
    </div>
  </div>
  <div id="footer">
    <p>Network name : <span id="netName">%SSID%</span></p>
    <p>softAP IP : <span id="softIP">%LOCALIP%</span></p>
    <p>public IP : <span id="pubIP">%PUBLICIP%</span></p>
    </div>

</body>
</html>

  )rawliteral";