
const char *HTML_CONTENT = R"=====(




<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>RC Car Control</title>
  <style>
    .slider-container {
      width: 50%;
      display: flex;
      justify-content: center;
      align-items: center;
      margin-bottom: 0px;
      flex: 1;
    }

    .slider {
      -webkit-appearance: none;
      width: 50%;
      height: 10px;
      border-radius: 10px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
    }

    .slider:hover {	
      opacity: 1;
    }

    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: #4caf50;
      cursor: pointer;
    }

    .slider::-moz-range-thumb {
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: #4caf50;
      cursor: pointer;
    }

    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      color: #444;
      background-color: #f0f0f0;
      margin: 0;
      padding: 0;
      user-select: none;
    }
    p {
      font-family: Arial, Helvetica, sans-serif; /* A classic, clean sans-serif font */
      font-size: 14px; /* Smaller font size (adjust to your preference) */
      line-height: 1.5; /* Adds spacing between lines of text for better readability */
      color: #333; /* A slightly darker gray for better contrast */
      margin-bottom: 5px; /* Add spacing between paragraphs */
	}
    .container {
      display: flex;
      flex-direction: column;
      align-items: center;
      
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
      width: 100px;
      height: 100px;
      margin: 10px;
      font-size: 41px;
      border: none;
      border-radius: 55%;
      background-color: #f0f0f0;
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
    
    #wsStatus {
      margin-top: 5px;
    }
    /* Additional styles for mobile layout */
    .mobile-buttons {
  	position: fixed; /* Position the buttons relative to the viewport */
  	bottom: 0; /* Align the buttons to the bottom of the viewport */
  	left: 0; /* Align the buttons to the left edge of the viewport */
  	width: 100%; /* Make the buttons take the full width of the viewport */
  	display: flex;
  	justify-content: space-around;
  	align-items: center;
  	padding: 20px; /* Add some padding for better spacing */
  	background-color: #fff; /* Add background color to the buttons area */
  	z-index: -1;
}

    .vertical-buttons {
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    .middle-div {
  flex: 1; /* Shorthand for flex-grow: 1, flex-shrink: 1, flex-basis: 0 */
  max-width: 50%; /* Limit the width to 50% of the container */
  background-color: #fff;
}

   
    
      
  </style>
</head>
<body>
  <div class="container">
  <h1>RC Car Control</h1>
  <div id="wsStatus">
      Status: <span id="wsState" style="color: red;">Disconnected</span>
      <button id="wsButton" onclick="toggleWebSocket()">Connect</button>
    </div>
     
  
	
    
  <div class="mobile-buttons">
    <!-- Left and Right buttons -->
    <div class="row">
      <button class="button" id="leftButton"data-command="l" data-stop-command="q" onmousedown="sendCommand('l')" onmouseup="sendCommand('q')">&#8678;</button> <!-- Left button -->
      <button class="button" id="rightButton" data-command="r" data-stop-command="w" onmousedown="sendCommand('r')" onmouseup="sendCommand('w')">&#8680;</button> <!-- Right button -->
    </div>
        <div class="middle-div">
	<div class="slider-container">
    			<input type="range" min="145" max="255" value="255" class="slider" id="speedSlider">
    			<p>Speed: <span id="speedValue">255</span></p>
  	</div>
	
  		<p>Last response: <span id="lastResponse"></span></p>
	</div>
    <!-- Forward, and Backward buttons -->
    <div class="vertical-buttons">
      <button class="button" id="upButton" data-command="f" data-stop-command="e" onmousedown="sendCommand('f')" onmouseup="sendCommand('e')">&#8679;</button> <!-- Forward button -->
      <button class="button" id="downButton"data-command="b" data-stop-command="t" onmousedown="sendCommand('b')" onmouseup="sendCommand('t')">&#8681;</button> <!-- Backward button -->
    </div>
  </div>
  
  </div>

   <script>
    var ws;

    // Function to toggle WebSocket connection
    function toggleWebSocket() {
      if (!ws || ws.readyState === WebSocket.CLOSED) {
        connectWebSocket();
      } else {
        disconnectWebSocket();
      }
    }

    // Function to create a WebSocket connection
    function connectWebSocket() {
      ws = new WebSocket("ws://" + window.location.host + ":81");
      ws.onopen = function() {
        console.log('WebSocket connected');
        document.getElementById('wsState').innerText = 'Connected';
        document.getElementById('wsState').style.color = 'green';
        document.getElementById('wsButton').innerText = 'Disconnect';
      };
      ws.onmessage = function(event) {
        console.log('Message from server: ', event.data);
        var responseTime = new Date().getTime() - startTime;
        document.getElementById('lastResponse').innerText = event.data + "(" + responseTime + ")ms";
      };
      ws.onclose = function() {
        console.log('WebSocket connection closed');
        document.getElementById('wsState').innerText = 'Disconnected';
        document.getElementById('wsState').style.color = 'red';
        document.getElementById('wsButton').innerText = 'Connect';
      };
      ws.onerror = function(error) {
        console.error('WebSocket error: ', error);
        document.getElementById('wsState').innerText = 'Error';
        document.getElementById('wsState').style.color = 'red';
        document.getElementById('wsButton').innerText = 'Connect';
      };
    }

    // Function to disconnect WebSocket
    function disconnectWebSocket() {
      if (ws && ws.readyState === WebSocket.OPEN) {
        ws.close();
      }
    }

    // Function to send commands to the WebSocket server
    function sendCommand(command) {
      if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send(command);
        console.log("command send: " + command);
        startTime = new Date().getTime(); // Record the start time
      }
    }

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
          sendCommand('e');
          break;
        case 'ArrowLeft':
          sendCommand('q');
          break;
        case 'ArrowRight':
          sendCommand('w');
          break;
        case 'ArrowDown':
          sendCommand('t');
          break;
      }
    });
    

    document.querySelectorAll('.button').forEach(function(button) {
  button.addEventListener('touchstart', function(event) {
    event.preventDefault(); // Prevent default touch behavior (e.g., scrolling)
    var command = this.getAttribute('data-command');
    sendCommand(command);
    console.log("Button pressed: " + command);

    // Add touch feedback
    this.style.backgroundColor = "#ddd"; // Change button color
    this.style.boxShadow = "0 0 10px rgba(0, 0, 0, 0.3)"; // Add shadow effect
  });

  button.addEventListener('touchend', function(event) {
    event.preventDefault(); // Prevent default touch behavior (e.g., scrolling)
    var command = this.getAttribute('data-stop-command');
    sendCommand(command);
    console.log("Button released: " + command);

    // Remove touch feedback
    this.style.backgroundColor = "#ccc"; // Restore original button color
    this.style.boxShadow = "none"; // Remove shadow effect
  });
});

    var speedValue = document.getElementById("speedValue");

    // Event listener for slider input change
    document.getElementById('speedSlider').addEventListener('input', function() {
      var speed = this.value; // Get the value of the slider
      speedValue.innerHTML = speed;
      sendCommand('speed:' + speed); // Send the speed value via WebSocket
    });
    function disableArrowScroll(event) {
      // Check if the pressed key is an arrow key
      if (event.key === 'ArrowUp' || event.key === 'ArrowDown' || event.key === 'ArrowLeft' || event.key === 'ArrowRight') {
        // Prevent default scroll behavior
        event.preventDefault();
      }
    }

    // Add event listener to capture arrow keydown events
    document.addEventListener('keydown', disableArrowScroll);
    
   
  </script>
</body>
</html>


)=====";
