char index[] PROGMEM = R"=="==(
<html>
    <link rel="stylesheet" href="https://bootswatch.com/4/darkly/bootstrap.css" media="screen">
    <link rel="stylesheet" href="https://bootswatch.com/_assets/css/custom.min.css"> 
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>

    <body>
    <div class="container">
    <div class="col-lg-6">
        <form>
        <fieldset>
            <legend>WiFi</legend>
            <div class="form-group">
            <label for="SSID">SSID</label>
            <input type="text" class="form-control" id="SSID" aria-describedby="SSID" placeholder="">
            <small id="SSIDHelp" class="form-text text-muted">Name of wifi network to be connected to.</small>
            </div>
            <div class="form-group">
            <label for="exampleInputPassword1">Password</label>
            <input type="password" class="form-control" id="WiFiPwd" placeholder="">
            <small id="WiFiPwdHelp" class="form-text text-muted">WiFi password.</small>
            </div>
        </fieldset>
        </form>

        <form>
        <fieldset>
            <legend>Database</legend>
            <div class="form-group">
                <label for="exampleInputEmail1">Local IP</label>
                <input type="text" class="form-control" id="DBIP" aria-describedby="emailHelp" placeholder="">
                <small id="DBIPHelp" class="form-text text-muted">IP adress of db server.</small>
            </div>

            <div class="form-group">
                <label for="exampleInputEmail1">Username</label>
                <input type="text" class="form-control" id="DBUsername" aria-describedby="emailHelp" placeholder="">
                <small id="DBUserHelp" class="form-text text-muted">DB username.</small>
            </div>

            <div class="form-group">
                <label for="exampleInputEmail1">Password</label>
                <input type="password" class="form-control" id="DBPwd" aria-describedby="DBPwd" placeholder="">
                <small id="DBPwdHelp" class="form-text text-muted">DB username password.</small>
            </div>

        </fieldset>
        </form>

        <button type="button" class="btn btn-primary" id="submitBtn">Submit</button>
    </div>                
    </div>
    </body>

    <script>
        // Insert placeholders based on current configuration
        $(document).ready(function() 
        {
            var xhr = new XMLHttpRequest();
            var url = "/config";

            var data = {};

            // function to be applied after request is ready
            xhr.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) 
                {
                    var response = this.responseText;  // get response text
                    response = response.split("&");  // split response parameters
                    console.log(response);

                    for(var i=0; i < response.length; i++)
                    {
                        var key, value;
                        var keyValuePair = response[i].split("=");   // extract key value pairs;

                        key = keyValuePair[0];
                        value = keyValuePair[1];
                        
                        console.log(key);
                        console.log(value);

                        data[key] = value;
                    }
                }

                $('#SSID').attr('placeholder', data['ssid']);
                $('#DBIP').attr('placeholder', data['dbIp']); 
                $('#DBUsername').attr('placeholder', data['dbUsername']);
            };

            xhr.open("GET", url, true);
            xhr.send();            
        });  

        $("#submitBtn").click(function() {
            var ssid = $('#SSID').val();           
            var dbIp = $('#DBIP').val();
            var dbUsername = $('#DBUsername').val();

            var wifiPwd = $("#WiFiPwd").val();
            var dbPwd = $("#DBPwd").val();

            var postData = {
                    ssid: ssid,
                    wifiPwd: wifiPwd,

                    dbIp: dbIp,
                    dbUsername: dbUsername,
                    dbPwd: dbPwd
                };

            for (var key in postData)
            {   
                // check if value is not empty
                if(postData[key] == '')
                {
                    delete postData[key]; 
                }
            }

            $.post("/config", postData);
        })

    </script>
</html>  
)=="==";