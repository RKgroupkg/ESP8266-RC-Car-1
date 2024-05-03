const char *HTML_CONTENT = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>RC Car Control</title>
  <style>
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      color: #444;
      background-color: #f0f0f0;
      margin: 0;
      padding: 0;
    }
    .container {
      display: flex;
      flex-direction: column;
      align-items: center;
      margin-top: 50px;
    }
    .row {
      display: flex;
      justify-content: center;
    }
    .column {
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    .button {
      width: 80px;
      height: 80px;
      margin: 10px;
      font-size: 24px;
      border: none;
      border-radius: 50%;
      background-color: #eee;
      transition: background-color 0.3s, transform 0.2s;
      cursor: pointer; /* Change cursor to pointer for better touch interaction */
    }
    .button:hover {
      background-color: #ddd;
      transform: scale(1.1);
    }
    .button:active {
      transform: scale(0.9);
    }
    .invisible {
      visibility: hidden;
      pointer-events: none; /* Disable clicking */
    }
  </style>
</head>
<body>
  <div class="container">
    <h1 style="margin-bottom: 20px;">RC Car Control</h1>
    <div class="column">
      <div class="row">
        <button class="button invisible" disabled></button> <!-- Blank button -->
        <button class="button" onmousedown="sendCommand('f')" onmouseup="sendCommand('s')">&#8679;</button> <!-- Forward button -->
        <button class="button invisible" disabled></button> <!-- Blank button -->
      </div>
      <div class="row">
        <button class="button" onmousedown="sendCommand('l')" onmouseup="sendCommand('d')">&#8678;</button> <!-- Left button -->
        <button class="button" onclick="sendCommand('s')">Stop</button> <!-- Stop button -->
        <button class="button" onmousedown="sendCommand('r')" onmouseup="sendCommand('d')">&#8680;</button> <!-- Right button -->
      </div>
      <div class="row">
        <button class="button invisible" disabled></button> <!-- Blank button -->
        <button class="button" onmousedown="sendCommand('b')" onmouseup="sendCommand('s')">&#8681;</button> <!-- Backward button -->
        <button class="button invisible" disabled></button> <!-- Blank button -->
      </div>
    </div>
    <p style="margin-top: 20px;">Last button pressed: <span id="lastPressed"></span></p>
    <p>Last successful response: <span id="lastResponse"></span></p>
  </div>
  <script>
    var lastCommand = ''; // Variable to store the last command

    document.addEventListener('keydown', function(event) {
      switch(event.key) {
        case 'ArrowUp':
          sendCommand('f');
          break;
        case 'ArrowLeft':
          sendCommand('l');
          break;
        case 'ArrowRight':
          sendCommand('r');
          break;
        case 'ArrowDown':
          sendCommand('b');
          break;
      }
    });

    document.addEventListener('keyup', function(event) {
      switch(event.key) {
        case 'ArrowUp':
          sendCommand('s');
          break;
        case 'ArrowLeft':
          sendCommand('d');
          break;
        case 'ArrowRight':
          sendCommand('d');
          break;
        case 'ArrowDown':
          sendCommand('s');
          break;
      }
    });

    function sendCommand(command) {
      // Check if the last command is not the same as the current command
      if (command !== lastCommand && !(lastCommand === 'ds' && command === 's')) {
        var start = new Date().getTime();
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            var end = new Date().getTime();
            var timeTaken = end - start;
            document.getElementById('lastPressed').innerText = command.toUpperCase();
            document.getElementById('lastResponse').innerText = '(' + command.toUpperCase() + ') ' + timeTaken + 'ms';
            lastCommand = command; // Update the last command
          }
        };
        xhttp.open('GET', '/control?action=' + command, true);
        xhttp.send();
      }
    }
  </script>
</body>
</html>
)=====";
