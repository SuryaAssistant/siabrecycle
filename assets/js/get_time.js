function runClock(){
    var dt = new Date();
    var dt_now = dt.toLocaleTimeString();
    document.getElementById("datetime").innerHTML = dt_now;
    display_c();
}

function display_c(){
    var refresh=1000; // Refresh rate in milli seconds
    mytime=setTimeout('runClock()',refresh)
    }
