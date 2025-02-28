function fetchData() {
    //juls
    // Fetch fingerprint data 
    fetch('http://192.168.141.12/data')
        .then(response => response.text())
        .then(data => {
            let fingerprintBox = document.getElementById('fingerprintStatus');
            fingerprintBox.innerText = data;
            
            // Change color based on fingerprint status
            fingerprintBox.className = 'data-box'; 
            if (data === "PLACE FINGER" || data === "ENROLLING" || data === "PLACE FINGER AGAIN" || data === "REMOVE FINGER") {
                fingerprintBox.classList.add("orange");
            } else if (data === "MATCH/Door Opened" || data === "SUCCESS: Fingerprint stored" || data === "SUCCESS") {
                fingerprintBox.classList.add("green");
            } else if (data === "NO MATCH/Door Closed" || data === "ERROR: Fingerprints did not match" || data === "ERROR") {
                fingerprintBox.classList.add("red");
            }
        })
        .catch(error => console.error('Error fetching fingerprint data:', error));

    // Fetch sensor object detection data
    fetch('http://192.168.141.12/object')
    .then(response => response.text())
    .then(data => {
        let objectBox = document.getElementById('objectStatus');
        objectBox.innerText = data; 

            // Reset classes before applying new ones
            objectBox.className = "sen1"; 
            // Change color based on object detection status
            if (data === "No object detected.") {
                objectBox.classList.add("no-object"); 
            } else if (data === "Sensor trigger sending Message!") {
                objectBox.classList.add("object-near");  // Now this should work correctly!
            }
    })
    .catch(error => console.error('Error fetching object data:', error));


    fetch('http://192.168.141.12/gsm')
    .then(response => response.text())
    .then(data => {
        let logbox = document.getElementById('logbox');
        logbox.innerText = data;

            // Reset classes before applying new ones
            objectBox.className = "logbox"; 
            if (data === "Sensor trigger sending Message sending message to: +639763709707") {
                logbox.innerText = "asdasd";
            }else if (data === "standby to send message"){
                logbox.innerText = "Standby to send message"
            }
    })
    .catch(error => console.error('Error fetching gsm data:', error));
}





function sendCommand() {
    let command = document.getElementById("commandInput").value;
    if (command.toUpperCase() === "E") {
        fetch('http://192.168.141.12/register', { method: 'POST' })
            .then(response => response.text())
            .then(data => console.log("Command sent: " + data))
            .catch(error => console.error('Error sending command:', error));
    }
}

setInterval(fetchData, 2000); // Fetch data every 2 seconds
