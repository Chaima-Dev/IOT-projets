***connect to wifi***

connect to the wifi using the router credentials 


***Call OpenWeather API*** 

create a new data structure called "WeatherData" that has 2 fields that will hold the responce from the API

we used ArduinoJson to handle our API call

Make an HTTP GET request to the OpenWeatherMap API to fetch weather data for a specified city and country




***Handle API server***
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);
