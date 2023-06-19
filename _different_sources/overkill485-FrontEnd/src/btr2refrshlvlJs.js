let batteryIsCharging = false;
function refreshLevel(elemNumber, level, value) {
    document.querySelector("#batteryCell"+elemNumber+" > div > div.acid-container > div > div").style.height = (level * document.querySelector("#batteryCell"+elemNumber+" > div").clientHeight) + "px";
    document.querySelector("#batteryCell"+elemNumber+" > div > div.acid-container > div > div").style.setProperty("--acid-height", (level * document.querySelector("#batteryCell0 > div").clientHeight) + "px");
    document.querySelector("#batteryCell"+elemNumber+" > div > div.percentage > span").innerText = value;
}
