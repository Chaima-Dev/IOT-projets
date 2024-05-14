***Project Idea***
My project is a Controller for the AC/Fan system in an intelligent cafe (simulated by 2 leds led )
Instead of owner of the café having to manually turning on/off the air-conditioning  or the ventilation in the room , I implemented an intelligent controller that automatically adjusts temperature settings based on real-time conditions and a web interface that can still give the owner some control over the system by monitoring and adjusting settings






***Features***
Control AC and fan states (on/off) via GraphQL mutations.
Fetch current states of AC and fan via GraphQL queries.
Set ESP32 IP address dynamically through a mutation.
Display a web interface for controlling the devices
Retrieve real-time temperature and humidity data using a DHT sensor.
Integrate with an external weather API (OpenWeatherMap) to fetch weather data.


***Prerequisites***
Arduino IDE installed on your machine.
ESP32 microcontroller board.
DHT sensor connected to the ESP32 for temperature and humidity sensing.
OpenWeatherMap API key for fetching weather data.
Node.js installed on your machine.
