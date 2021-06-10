// used when hosting the site on the ESP8266
var address = location.hostname;
var urlBase = "";

// used when hosting the site somewhere other than the ESP8266 (handy for testing without waiting forever to upload to SPIFFS)
// var address = "192.168.30.34";
// var urlBase = "http://" + address + "/";

$(document).ready(function() {

    $("#btnResetWifi").click(function() {
      postValue("wifi", "reset");

    });
  
    $("#btnResetEEPROM").click(function() {
        postValue("eeprom", "reset");
  
      });
      
    $("#status").html("Ready");
  });
  
  function postValue(name, value) {
    $("#status").html("Setting " + name + ": " + value + ", please wait...");
  
    var body = { name: name, value: value };
  
    $.post(urlBase + "configure?name=" + name + "&value=" + value, body, function (data) {
      if (data.name != null) {
        $("#status").html("Configure " + name + ": " + data.name);
      } else {
        $("#status").html("Configure " + name + ": " + data);
      }
    });
  }
