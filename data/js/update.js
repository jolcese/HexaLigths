var address = location.hostname;
var urlBase = "";

$(document).ready(function() {

  $.get(urlBase + "remote.json", function(data) {
    $("#status").html("Getting, endpoint...");

    remote = data;
    if (remote.ip != undefined)
    address = remote.ip;
    urlBase = "http://" + address;
    $("#status").html("Endpoint: " + urlBase);
  })
  .fail(function(errorThrown) {
    $("#status").html("Endpoint: local");
  });

  $('form').submit(function(e){
    e.preventDefault();
    var form = $('#upload_form')[0];
    console.log(form);
    var data = new FormData(form);
    console.log(data);
    $.ajax({
        url: urlBase  + '/updateFirmware',
        type: 'POST',
        data: data,
        contentType: false,
        processData:false,
        xhr: function() {
            var xhr = new window.XMLHttpRequest();
            xhr.upload.addEventListener('progress', function(evt) {
                if (evt.lengthComputable) {
                    var per = evt.loaded / evt.total;
                    $("#status").html('Progress: ' + Math.round(per*100) + '%');
                }
            }, false);
            return xhr;
        },
        success:function(d, s) {
            console.log('Success!')
        },
        error: function (a, b, c) {
        }
    });
});
  
  $("#status").html("Ready");
});

