var address = location.hostname;
var urlBase = "";

$(document).ready(function() {

  $.get(urlBase + "remote.json", function(data) {
    $("#status").html("Getting, endpoint...");

    remote = data;
    if (remote.ip != undefined)
    address = remote.ip;
    urlBase = "http://" + address + "/";
  })
  .fail(function(errorThrown) {
  });

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
