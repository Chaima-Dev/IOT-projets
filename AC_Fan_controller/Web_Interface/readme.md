***Web Interface :*** 

a simple styling using HTML and Bootstrap 

![interface_web](https://github.com/shy-shai/IOT-projets/assets/165284030/43f195c1-ec2b-487a-97ee-c4c109da1d34)


The interface code includes functions to fetch the system (AC/FAN/Automatic_mode..) state and automatically update it at regular intervals

* **Enable/Disable Automatic Mode :** Sends a request to the server to execute the setAUTOenabled mutation with enable: true/false

* **Fetch and Display AC State :** Sends a request to the server to execute the getACState query and displays the result in an HTML element with the ID acStateDisplay

* **Turn AC On/OFF :** Sends a request to the server to execute the setACState mutation with state: true/false

* **Fetch and Display Fan State :** Sends a request to the server to execute the getFanState query and displays the result in an HTML element with the ID fanStateDisplay
  
* **Turn Fan On/OFF :** Sends a request to the server to execute the setFanState mutation with state: true/false
  

