$(function(){
  showOff();
  noInput = setTimeout(function(){
    // Nudge the user with a hint
    $("#hint").show();
  },8000);
});
var mouseTimer,showingOff,noInput, charge = default_charge = 63;
$("#battery").click(function(){
  clearInterval(noInput);
  showOff();
});
$("div","#buttons").mousedown(function(){
  clearInterval(noInput);
  var charging = $(this).hasClass("more");
  mouseTimer = setInterval(function(){
    // Continuously increase/decrease battery while clicking/holding
    //console.log("Holding mouse down!");
    if(charge>12 && !charging){
      charge--;
    }else if(charge<89 && charging){
      charge++;
    }else{
      return false;
    }
    batUpdate();
  },250);
}).click(function(){
  clearInterval(noInput);
  clearInterval(mouseTimer);
  if(charge>12 && !$(this).hasClass("more")){
    charge--;
  }else if(charge<89 && $(this).hasClass("more")){
    charge++;
  }else{
    return false;
  }
  batUpdate();
});
$(document).mouseup(function(){
  clearInterval(mouseTimer);
  return false;
});
function batUpdate(){
  //console.log("Charge: ",charge);
  if(charge<20){
    // Red - Danger!
    col = ["#750900","#c6462b", "#b74424", "#df0a00", "#590700"];
  }else if(charge<40){
    // Yellow - Might wanna charge soon...
    col = ["#754f00","#f2bb00", "#dbb300", "#df8f00", "#593c00"];
  }else{
    // Green - All good!
    col = ["#316d08","#60b939", "#51aa31", "#64ce11", "#255405"];
  }
  $("#battery").css("background-image","linear-gradient(to right, transparent 5%, "+col[0]+" 5%, "+col[0]+" 7%, "+col[1]+" 8%, "+col[1]+" 10%, "+col[2]+" 11%, "+col[2]+" "+ (charge-3) +"%, "+col[3]+" "+ (charge-2) +"%, "+col[3]+" "+ charge +"%, "+col[4]+" "+ charge +"%, black "+ (charge+5) +"%, black 95%, transparent 95%), linear-gradient(to bottom, rgba(255,255,255,0.5) 0%, rgba(255,255,255,0.4) 4%, rgba(255,255,255,0.2) 7%, rgba(255,255,255,0.2) 14%, rgba(255,255,255,0.8) 14%, rgba(255,255,255,0.2) 40%, rgba(255,255,255,0) 41%, rgba(255,255,255,0) 80%, rgba(255,255,255,0.2) 80%, rgba(255,255,255,0.4) 86%, rgba(255,255,255,0.6) 90%, rgba(255,255,255,0.1) 92%, rgba(255,255,255,0.1) 95%, rgba(255,255,255,0.5) 98%)");
}
function showOff(){
  clearInterval(showingOff);
  var chgInt = 8;
  if(charge < 40){
    // Charge it up!
    showingOff = setInterval(function(){
      if(charge+chgInt<63){
        // Last update
        charge = charge+chgInt;
        batUpdate();
      }else{
        charge = 63;
        batUpdate();
        clearInterval(showingOff);
      }
    },350);
  }else{
    showingOff = setInterval(function(){
      if(charge-chgInt>11){
        // Last update
        charge = charge-chgInt;
        batUpdate();
      }else{
        charge = 63;
        batUpdate();
        clearInterval(showingOff);
      }
    },350);
  }
}