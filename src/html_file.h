// HTML page with a file upload form and progress bar
const char *htmlPage = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP32 File Upload</title>
  <script>
    function uploadFile() {
      var file = document.getElementById("file").files[0];
      var formData = new FormData();
      formData.append("file", file);

      var xhr = new XMLHttpRequest();
      xhr.open("POST", "/upload", true);

      // Update progress bar
      xhr.upload.onprogress = function(event) {
        if (event.lengthComputable) {
          var percentComplete = (event.loaded / event.total) * 100;
          document.getElementById("progress").value = percentComplete;
          document.getElementById("status").innerText = Math.round(percentComplete) + "% uploaded";
        }
      };

      // On completion
      xhr.onload = function() {
        if (xhr.status == 200) {
          document.getElementById("status").innerText = "File uploaded successfully!";
        } else {
          document.getElementById("status").innerText = "Failed to upload file.";
        }
      };

      xhr.send(formData);
    }
  </script>
</head>
<body>
  <h1>ESP32 Gif File Upload</h1>
  <input type="file" id="file"><br><br>
  <input type="button" value="Upload File" onclick="uploadFile()"><br><br>
  <progress id="progress" value="0" max="100"></progress>
  <p id="status"></p>
</body>
</html>
)rawliteral";