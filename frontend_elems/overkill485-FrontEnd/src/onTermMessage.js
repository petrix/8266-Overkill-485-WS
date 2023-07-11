export const onTermMessage = {};

onTermMessage.exec = (event) => {
  const data = event.data;

  if (data.slice(0, 3) === "TRM") {
    // console.table(data.slice(0, 3),"============received message");
    let payload = data.slice(5);
    let msg = "";
    let timeStamp = payload.split("--")[0]+"000";
    let pl1 = payload.split("--")[1];
    let d = new Date(JSON.parse(timeStamp));
    console.log(d);
    let hh = d.getHours();
    let mm = d.getMinutes();
    let ss = d.getSeconds();
    if(hh<10)msg+="0";
    msg+=hh;
    msg+=":";
    if(mm<10)msg+="0";
    msg+=mm;
    msg+=":";
    if(ss<10)msg+="0";
    msg+=ss;

    // msg += `${hh}:${mm}:${ss}`;

    msg+=pl1;

   const termFrame =  document.querySelector("#termFrame");
    let div = document.createElement("div");
    div.innerText = msg;
    termFrame.appendChild(div);
    div.scrollIntoView({ behavior: "smooth", block: "end", inline: "nearest" });
// console.log();

  } else {
    console.log(data);
  }
};
