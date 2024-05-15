
***Install libraries :***

* ArduinoJson.h

![L1](https://github.com/shy-shai/IOT-projets/assets/165284030/6d325ef0-46fa-40ca-bbc4-fa8463a7e842)

* WiFi.h

* HTTPClient.h

* ESPAsyncWebServer.h

![L3](https://github.com/shy-shai/IOT-projets/assets/165284030/9580a55d-9e54-4ee3-893a-db7366c74be3)

* DHT.h

 ![L2](https://github.com/shy-shai/IOT-projets/assets/165284030/06d41c7e-1540-45a7-96d8-639e5d9b9f6d)




****The Code include :****



***Connect to WIFI***

connect to the wifi using the router credentials 


***Call OpenWeather API*** 

* create a new data structure called "WeatherData" that has 2 fields that will hold the responce from the API

* we used ArduinoJson to handle our API call

* Make an HTTP GET request to the OpenWeatherMap API to fetch weather data for a specified city and country




***Handle API server***


**handle_rest_POST_request_for_AC()** : defines an endpoint to control the state of the AC. It listens for POST requests at the /AC URL. The state of the AC is toggled based on the state parameter provided in the URL-encoded body of the request.

**The function handle_rest_GET_request_for_AC()** : defines an endpoint to retrieve the current state of the AC. It listens for GET requests at the /AC URL.

**The function handle_rest_POST_request_for_Fan()** : defines an endpoint to control the state of the Fan. It listens for POST requests at the /Fan URL. The state of the Fan is toggled based on the state parameter provided in the URL-encoded body of the request.

**The function handle_rest_GET_request_for_Fan()**: defines an endpoint to retrieve the current state of the Fan. It listens for GET requests at the /Fan URL.



***Communicate with GraphQL server***

* querying and setting the state of automatic mode : by sending a query to get the state of AUTOenabled that will be used in a condition to enable/disable the automatic mode
  
* sending status messages : by sending a mutation to set a state message (this message is a confirmation that the mode is properly set , it only triggers after we enter the automatic mode in the code )

* sending the device's IP address to the server :by sending a mutation to set the IP address (because the ESP32 can change ip address so often , it will send its address to the graphql server , to be able to receive traffic later)



***The main Code***


* monitor the current temperature/humidity of the room using a DHT sensor

* fetch weather data from an API at regular intervals 

* checks whether automatic mode is enabled and sends state messages accordingly
  
*control an air conditioner/fan in the automatic mode based on the sensor and weather data

