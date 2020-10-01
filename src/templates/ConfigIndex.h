#ifndef ConfigIndex__h
#define ConfigIndex__h

static char config_index[] PROGMEM = R"=="==(
<html>
	<head>
		<link rel="stylesheet" href="https://bootswatch.com/4/darkly/bootstrap.css" media="screen">
		<link rel="stylesheet" href="https://bootswatch.com/_assets/css/custom.min.css"> 

		<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
		<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js"></script>

		<script src="https://bootswatch.com/_vendor/jquery/dist/jquery.min.js"></script>
		<script src="https://bootswatch.com/_vendor/bootstrap/dist/js/bootstrap.min.js"></script>
		<script src="https://bootswatch.com/_assets/js/custom.js"></script>		
	</head>
	
	</body>
	<title>Config</title>
		
	<div class="container">

	<div class="col-lg-6">
		<ul class="nav nav-tabs">
		  <li class="nav-item">
			<a class="nav-link active" data-toggle="tab" href="#WiFi">WiFi</a>
		  </li>
		  <li class="nav-item">
			<a class="nav-link" data-toggle="tab" href="#API">API</a>
		  </li>
		</ul>

		<div id="myTabContent" class="tab-content">
		
			<!-- WiFI Config page -->		
			<div class="tab-pane fade active show" id="WiFi">
		  
				<!-- SSID -->
				<div class="form-group">
					<label for="SSID">SSID</label>
					<input type="text" class="form-control" id="SSID" aria-describedby="SSID" placeholder="">
					<small class="form-text text-muted">Name of wifi network to be connected to.</small>
				</div>

				<!-- PWD -->
				<div class="form-group">
					<label for="exampleInputPassword1">Password</label>
					<input type="password" class="form-control" id="WiFiPwd" placeholder="">
					<small class="form-text text-muted">WiFi password.</small>
				</div>
				
				<!-- AP Mode -->
				<div class="form-group">
					<label for="exampleInputPassword1">AP mode</label>
					<select class="custom-select">
					  <option value="1">AP+Client</option>
					  <option value="2">Client</option>
					</select>
					<small class="form-text text-muted">Device working mode.</small>
				</div>
				<button type="button" class="btn btn-primary" id="WiFiSubmitBtn">Submit</button>
			</div>

			  <!-- API Config page -->  
			  <div class="tab-pane fade" id="API">
				<div class="form-group">
					<label for="SecretKey" disabled="">Sekret key</label>
					<input type="text" class="form-control" id="Secret" aria-describedby="SSID" placeholder="" disabled="">
					<small id="SSIDHelp" class="form-text text-muted" disabled="">Secret key to be used when connecting to API.</small>
					
				</div>

				<div class="form-group">
					<label for="HostIP">Host IP</label>
					<input type="text" class="form-control" id="IP" placeholder="">
					<small id="IPHelp" class="form-text text-muted">IP of local API server.</small>

					<label for="HostPort">Port</label>
					<input type="number" class="form-control" id="Port" placeholder=80>
					<small id="PortHelp" class="form-text text-muted">API server port (default 80).</small>
				</div>

				<div class="form-group">
					<label for="DeviceAPIID">Device ID</label>
					<input type="number" class="form-control" id="DeviceAppId" placeholder="">
					<small id="DeviceAPIIDHelp" class="form-text text-muted">Device ID in SmartHome app.</small>
				</div>

				<div class="form-group">
					<label for="DeviceAPIID">Device API name</label>
					<input type="text" class="form-control" id="DeviceAppName" placeholder="">
					<small id="DeviceAPIIDHelp" class="form-text text-muted">Device name in SmartHome app.</small>
				</div>
				
				<button type="button" class="btn btn-primary" id="APISubmitBtn">Submit</button>
				</div> 		  		  
			  </div>
		</div>
	</div>
	</div>
	</body>
	
	<script>
		document.addEventListener('DOMContentLoaded', function () {
			var xhr = new XMLHttpRequest();
			xhr.open("GET", "/config", false);
			xhr.send(null);

			var obj = JSON.parse(xhr.responseText);
			console.log(xhr.responseText);
			document.getElementById("SSID").value = obj["wifi"]["ssid"];

			document.getElementById("IP").value = obj["api"]["apiHostIP"];
			document.getElementById("Port").value = obj["api"]["apiHostPort"];

			document.getElementById("DeviceAppId").value = obj["api"]["apiDeviceId"];
			document.getElementById("DeviceAppName").value = obj["api"]["apiDeviceName"];
		});

        document.getElementById("WiFiSubmitBtn").addEventListener("click", function() {
			var obj = new Object();
			
            obj.ssid = document.getElementById("SSID").value;
            obj.pwd = document.getElementById("WiFiPwd").value;

			var jsonString = JSON.stringify(obj);
			console.log(jsonString);            

			var xhr = new XMLHttpRequest();
			xhr.open("POST", "/config?wifi", true);
			xhr.setRequestHeader('Content-Type', 'text/plain');
			xhr.send(jsonString);
        });

        document.getElementById("APISubmitBtn").addEventListener("click", function() {
			var obj = new Object();
			
            obj.appHostIp = document.getElementById("IP").value;
            obj.appHostPort = document.getElementById("Port").value;
      
	        obj.appDeviceId = document.getElementById("DeviceAppId").value;
			obj.appDeviceName = document.getElementById("DeviceAppName").value;

			var jsonString = JSON.stringify(obj);
			console.log(jsonString);            

			var xhr = new XMLHttpRequest();
			xhr.open("POST", "/config?api", true);
			xhr.setRequestHeader('Content-Type', 'text/plain');
			xhr.send(jsonString);
        });
		
	</script>
</html>
)=="==";

#endif