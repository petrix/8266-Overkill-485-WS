!function(){const e=document.createElement("link").relList;if(!(e&&e.supports&&e.supports("modulepreload"))){for(const e of document.querySelectorAll('link[rel="modulepreload"]'))t(e);new MutationObserver((e=>{for(const n of e)if("childList"===n.type)for(const e of n.addedNodes)"LINK"===e.tagName&&"modulepreload"===e.rel&&t(e)})).observe(document,{childList:!0,subtree:!0})}function t(e){if(e.ep)return;e.ep=!0;const t=function(e){const t={};return e.integrity&&(t.integrity=e.integrity),e.referrerPolicy&&(t.referrerPolicy=e.referrerPolicy),"use-credentials"===e.crossOrigin?t.credentials="include":"anonymous"===e.crossOrigin?t.credentials="omit":t.credentials="same-origin",t}(e);fetch(e.href,t)}}();const e=(e,t)=>t.some((t=>e instanceof t));let t,n;const o=new WeakMap,r=new WeakMap,s=new WeakMap,a=new WeakMap,i=new WeakMap;let c={get(e,t,n){if(e instanceof IDBTransaction){if("done"===t)return r.get(e);if("objectStoreNames"===t)return e.objectStoreNames||s.get(e);if("store"===t)return n.objectStoreNames[1]?void 0:n.objectStore(n.objectStoreNames[0])}return p(e[t])},set:(e,t,n)=>(e[t]=n,!0),has:(e,t)=>e instanceof IDBTransaction&&("done"===t||"store"===t)||t in e};function l(e){c=e(c)}function d(e){return e!==IDBDatabase.prototype.transaction||"objectStoreNames"in IDBTransaction.prototype?(n||(n=[IDBCursor.prototype.advance,IDBCursor.prototype.continue,IDBCursor.prototype.continuePrimaryKey])).includes(e)?function(...t){return e.apply(b(this),t),p(o.get(this))}:function(...t){return p(e.apply(b(this),t))}:function(t,...n){const o=e.call(b(this),t,...n);return s.set(o,t.sort?t.sort():[t]),p(o)}}function u(n){return"function"==typeof n?d(n):(n instanceof IDBTransaction&&function(e){if(r.has(e))return;const t=new Promise(((t,n)=>{const o=()=>{e.removeEventListener("complete",r),e.removeEventListener("error",s),e.removeEventListener("abort",s)},r=()=>{t(),o()},s=()=>{n(e.error||new DOMException("AbortError","AbortError")),o()};e.addEventListener("complete",r),e.addEventListener("error",s),e.addEventListener("abort",s)}));r.set(e,t)}(n),e(n,t||(t=[IDBDatabase,IDBObjectStore,IDBIndex,IDBCursor,IDBTransaction]))?new Proxy(n,c):n)}function p(e){if(e instanceof IDBRequest)return function(e){const t=new Promise(((t,n)=>{const o=()=>{e.removeEventListener("success",r),e.removeEventListener("error",s)},r=()=>{t(p(e.result)),o()},s=()=>{n(e.error),o()};e.addEventListener("success",r),e.addEventListener("error",s)}));return t.then((t=>{t instanceof IDBCursor&&o.set(t,e)})).catch((()=>{})),i.set(t,e),t}(e);if(a.has(e))return a.get(e);const t=u(e);return t!==e&&(a.set(e,t),i.set(t,e)),t}const b=e=>i.get(e);function m(e,t,{blocked:n,upgrade:o,blocking:r,terminated:s}={}){const a=indexedDB.open(e,t),i=p(a);return o&&a.addEventListener("upgradeneeded",(e=>{o(p(a.result),e.oldVersion,e.newVersion,p(a.transaction),e)})),n&&a.addEventListener("blocked",(e=>n(e.oldVersion,e.newVersion,e))),i.then((e=>{s&&e.addEventListener("close",(()=>s())),r&&e.addEventListener("versionchange",(e=>r(e.oldVersion,e.newVersion,e)))})).catch((()=>{})),i}const g=["get","getKey","getAll","getAllKeys","count"],h=["put","add","delete","clear"],w=new Map;function f(e,t){if(!(e instanceof IDBDatabase)||t in e||"string"!=typeof t)return;if(w.get(t))return w.get(t);const n=t.replace(/FromIndex$/,""),o=t!==n,r=h.includes(n);if(!(n in(o?IDBIndex:IDBObjectStore).prototype)||!r&&!g.includes(n))return;const s=async function(e,...t){const s=this.transaction(e,r?"readwrite":"readonly");let a=s.store;return o&&(a=a.index(t.shift())),(await Promise.all([a[n](...t),r&&s.done]))[0]};return w.set(t,s),s}l((e=>({...e,get:(t,n,o)=>f(t,n)||e.get(t,n,o),has:(t,n)=>!!f(t,n)||e.has(t,n)})));const y=["continue","continuePrimaryKey","advance"],v={},I=new WeakMap,S=new WeakMap,D={get(e,t){if(!y.includes(t))return e[t];let n=v[t];return n||(n=v[t]=function(...e){I.set(this,S.get(this)[t](...e))}),n}};async function*B(...e){let t=this;if(t instanceof IDBCursor||(t=await t.openCursor(...e)),!t)return;const n=new Proxy(t,D);for(S.set(n,t),i.set(n,b(t));t;)yield n,t=await(I.get(n)||t.continue()),I.delete(n)}function P(t,n){return n===Symbol.asyncIterator&&e(t,[IDBIndex,IDBObjectStore,IDBCursor])||"iterate"===n&&e(t,[IDBIndex,IDBObjectStore])}l((e=>({...e,get:(t,n,o)=>P(t,n)?B:e.get(t,n,o),has:(t,n)=>P(t,n)||e.has(t,n)})));const L={};let E=window.indexedDB||window.mozIndexedDB||window.webkitIndexedDB||window.msIndexedDB||self.indexedDB;var M;let x;L.init=async e=>{console.log("DB-init",e),"indexedDB"in window?await M(e):console.log("This browser doesn't support IndexedDB")},M=async e=>await m(e,1,{upgrade(t){const n=t.createObjectStore(e,{keyPath:"title",autoIncrement:!0});n.createIndex("title","title",{unique:!0}),n.createIndex("b64","b64",{unique:!0}),n.createIndex("dateModified","dateModified",{unique:!1})}}),L.addPublication=async(e,t,n,o)=>{console.log(e,t,n);let r=await m(t,1);return await r.get(t,e)?(await r.put(t,{title:e,blob:n,b64:o,dateModified:(new Date).getTime()}),console.log("updated"),"updated"):(await r.add(t,{title:e,blob:n,b64:o,dateModified:(new Date).getTime()}),console.log("added"),"added")},L.loadPublication=async(e,t)=>{const n=await m(t,1);return await n.get(t,e)},L.checkPublication=async(e,t)=>{const n=await m(t,1);let o,r=n.transaction(t,"readonly");try{o=await r.objectStore(t).get(e)}catch{}return n.close(),o},L.loadImage=async e=>{const t=await async function(e){return console.log("function -loader - db "+e),new Promise(((t,n)=>{let o;try{o=fetch(e,{mode:"cors"}).then((e=>e.blob())),t(o)}catch(r){console.log(r)}finally{console.log("We do cleanup here")}}))}(`${e}`);console.log("recValue ",t);let n=t;return console.log(n),{title:e,blob:n,b64:await L.convToB64(n),dateModified:(new Date).getTime()}},L.readAllDB=async e=>{const t=await m(e,1),n=await t.getAll(e);return await t.getAllKeys(e),t.close(),n},L.deletePublication=async(e,t)=>{const n=await m(e,1);let o=n.transaction(e,"readwrite");try{await o.objectStore(e).get(t);o.objectStore(e).delete(t)}catch(r){}n.close()},L.deleteDatabase=e=>{E.open(e,1);var t=E.deleteDatabase(e);t.onsuccess=function(t){console.log("Database "+e+" deleted")},console.warn(t)},L.deleteObjStore=e=>{var t=L.getObjectStore(e,"readwrite");try{t.clear()}catch(n){}},L.bmpTOblob=e=>new Promise((t=>{const n=document.createElement("canvas");n.width=e.width,n.height=e.height,n.getContext("bitmaprenderer").transferFromImageBitmap(e),n.toBlob((e=>t(e)))})),L.convToB64=function(e){return new Promise((t=>{let n=new FileReader;n.onload=function(){t(this.result)},n.readAsDataURL(e)}))},new Promise(((e,t)=>{L.init("bmsesp-db").then((t=>{console.log("db",t),e()}))})),document.querySelector("h1").innerText=window.location.pathname.split("/")[1].charAt(0).toUpperCase()+window.location.pathname.split("/")[1].slice(1);const T=["home.svg","cog.svg","wifi.svg","update.svg"];function C(e){return new Promise(((t,n)=>{console.log(e),L.checkPublication(e,"bmsesp-db").then((n=>{n?L.loadPublication(e,"bmsesp-db").then((e=>{t(e)})):L.loadImage(e).then((n=>{n&&(""!=n.blob.type||"text/html"!=n.blob.type?L.addPublication(e,"bmsesp-db",n.blob,n.b64).then((()=>{t(n)})):t())}))}))}))}async function k(e){return new Promise((function(t,n){console.log("======================= : ",e),C(e).then((e=>{var n=document.createElement("script");e.blob.text().then((e=>{n.innerHTML=e})),n.async=!1,console.log(n),document.head.appendChild(n),t(n)}))}))}T.forEach(((e,t)=>{C(T[t]).then((e=>{let n=document.createElement("img");n.src=e.b64,n.style.top="15px",t>0?n.style.right=20+60*(t-1)+"px":n.style.left="20px",document.querySelector(`#glyph${t}`).appendChild(n)}))}));const j=["ws.js","btr2initBtrJs.js","btr2refrshlvlJs.js","btr2BtrMsg.js","btr2NetMsg.js","btr2IPmsg.js"];async function A(){return new Promise(((e,t)=>{let n=C(j[0]).then((e=>(console.log(e),e.blob.text()))).then((e=>{console.log(e);let t=document.createElement("script");return t.innerHTML=e,document.head.appendChild(t),console.log(n),websocket.onopen=e=>{console.log("WS opened"),console.log(e),websocket.send("batteryStatus"),websocket.send("netscan"),websocket.send("ip")},websocket.onclose=e=>{console.log("WS closed"),console.log(e),setTimeout(initWebSocket,2e3)},websocket})).then((t=>e(t)))}))}if("/"==window.location.pathname){let e=function(){return new Promise(((e,t)=>{k(j[1]).then((()=>{k(j[2]).then((()=>{initBatteryArray(4),k(j[3]).then((()=>{k(j[5]).then((()=>e()))}))}))}))}))};console.log(window.location.pathname),document.querySelector("#card").innerHTML='\n    <div id="batteryArray" class="batteryArray"></div>\n        <div class="ntcsStats"></div>\n        <div class="batteryStats"></div> \n    ',document.querySelector("#footer").innerHTML='\n    <p>Network name : <span id="netName">%SSID%</span></p>\n    <p>softAP IP : <span id="softIP">%LOCALIP%</span></p>\n    <p>public IP : <span id="pubIP">%PUBLICIP%</span></p>\n    ',e(),A().then((e=>{console.log(e),e.onmessage=e=>{if(console.log("WS message"),"BAT"===e.data.slice(0,3))onBTRMessage(e);else onIPMessage(e)},clearInterval(x),x=setInterval((()=>{e.send("batteryStatus")}),1300)}))}else if("/netscan"==window.location.pathname){let e=function(e){return new Promise(((e,t)=>{k(j[4]).then((()=>{k(j[5]).then(e())}))}))};e(),A().then((e=>{console.log(e),e.onmessage=e=>{if(console.log("WS message"),console.log(e.data.slice(0,3)),"SCN"===e.data.slice(0,3))onNetMessage(e);else onIPMessage(e)}})),document.querySelector("#card").innerHTML='\n    <p><button id="button" class="button">Scan</button></p>\n    <h2>Scaned Stations</h2>\n    <div id="scanStations" class="scanStations"></div>\n    <h4>Connect to network:</h4>\n    <div class="credentials">\n    <input id="ssidInput" type=\'text\' placeholder=\'network\' name=\'n\'/>\n    <hr /><input id="ssidPassword" type=\'password\' placeholder=\'password\' name=\'p\'/>\n    <hr /><button class="button" id="credits" disabled="disabled">Connect</button>\n    </div>\n    '}else if("/update"==window.location.pathname){let e=function(e){return new Promise(((e,t)=>{k(j[5]).then(e())}))};e(),A().then((e=>{console.log(e),e.onmessage=e=>{console.log("WS message"),console.log(e.data);e.data.slice(0,3);onIPMessage(e)}})),document.querySelector("#card").innerHTML='\n    <form method="POST" action="/update" enctype="multipart/form-data">\n    <input class="button" type="file" name="update">\n    <input class="button" type="submit" value="Update"></form>'}else if("/settings"==window.location.pathname){let e=function(e){console.log(e,e.length),console.log(e[0]);for(let t=0;t<e.length;t++){const o=e[t],r=["jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"];let s=document.createElement("div");s.setAttribute("class","row");let a=document.createElement("div");a.innerText=o.title,a.setAttribute("class","row__title");let i=document.createElement("div");i.innerText=o.b64.length,i.setAttribute("class","row__b64");let c=document.createElement("div");c.setAttribute("class","row__blob");let l=document.createElement("div");l.innerHTML=o.blob.type.split("/")[0]+"<br>"+o.blob.type.split("/")[1],l.setAttribute("class","row__blob__type");let d=document.createElement("div");d.innerText=o.blob.size,d.setAttribute("class","row__blob__size");let u=document.createElement("div"),p=new Date(o.dateModified);u.innerHTML=`${p.getDate()} ${r[p.getMonth()]} ${p.getFullYear()}<br>${p.getHours()}:${p.getMinutes()}:${p.getSeconds()}`,u.setAttribute("class","row__date"),s.appendChild(a),s.appendChild(i),c.appendChild(d),c.appendChild(l),s.appendChild(c),s.appendChild(u),s.childNodes.forEach((e=>e.childNodes.forEach((e=>e.ariaLabel="row-"+(t+1))))),s.childNodes.forEach((e=>e.ariaLabel="row-"+(t+1))),s.ariaLabel="row-"+(t+1),s.onmousedown=e=>{console.log(e.target.ariaLabel);let t=e.target.ariaLabel,n=document.querySelector(`#cashedElems > [aria-label=${t}]`),o=confirm("shure?");console.log(o),o&&(L.deletePublication("bmsesp-db",n.firstChild.innerText),console.log(n.firstChild.innerText),n.remove())},n.appendChild(s)}},t=function(e){return new Promise(((e,t)=>{k(j[5]).then(e())}))};document.querySelector("#card").innerHTML='\n    <p><button id="readCashed" class="button">Read Cashed Elems</button></p>\n    <div id="cashedElems">\n    </div>\n\n    ';const n=document.querySelector("#cashedElems");document.querySelector("#readCashed").addEventListener("mousedown",(t=>{n.innerHTML="",L.readAllDB("bmsesp-db").then((t=>{e(t)}))})),t(),A().then((e=>{console.log(e),e.onmessage=e=>{console.log("WS message");e.data.slice(0,3);onIPMessage(e)}}))}else console.log(window.location.pathname),document.querySelector("#card").innerHTML='<div title="404">404</div>';
