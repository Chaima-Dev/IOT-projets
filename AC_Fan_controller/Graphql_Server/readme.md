
***Prerequisites:***

 * Make sure you have Node.js and npm installed on your machine


***Configurations :*** 
 * npm init -y
 * npm install express  ( For creating the GraphQL server )
 * npm install express-graphql  ( For creating the GraphQL server )
 * npm install graphql-tools   ( For schema management )
 * npm install axios    ( For making HTTP requests to the ESP32 )




***This code includes :*** 

* a GraphQL server using Express, express-graphql, and graphql-tools to interact with an ESP32 microcontroller via HTTP requests.

**This server manages :**
  
*  querying and mutation of the states of an air conditioner (AC) and a fan throught the web Interface
* fetching the state of the air conditioner (AC) and a fan from the ESP32
*  it handles the IP address of the ESP32 and an automatic mode
  

**Functions**
* **getStateFromESP32(device, IP) :** Sends a GET request to the ESP32 to fetch the state of a device (AC or fan)

* **sendStateToESP32(device, state, IP) :** Sends a POST request to the ESP32 to set the state of a device (AC or fan)
