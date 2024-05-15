***connect to wifi***

connect to the wifi using the router credentials 


***Call OpenWeather API*** 

*create a new data structure called "WeatherData" that has 2 fields that will hold the responce from the API

we used ArduinoJson to handle our API call

Make an HTTP GET request to the OpenWeatherMap API to fetch weather data for a specified city and country




***Handle API server***


**handle_rest_POST_request_for_AC()** : defines an endpoint to control the state of the AC. It listens for POST requests at the /AC URL. The state of the AC is toggled based on the state parameter provided in the URL-encoded body of the request.

**The function handle_rest_GET_request_for_AC()** : defines an endpoint to retrieve the current state of the AC. It listens for GET requests at the /AC URL.

**The function handle_rest_POST_request_for_Fan()** : defines an endpoint to control the state of the Fan. It listens for POST requests at the /Fan URL. The state of the Fan is toggled based on the state parameter provided in the URL-encoded body of the request.

**The function handle_rest_GET_request_for_Fan()**: defines an endpoint to retrieve the current state of the Fan. It listens for GET requests at the /Fan URL.



***Communicate with GraphQL server***



