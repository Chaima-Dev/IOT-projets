***Project Idea***


My project is a Controller for the AC/Fan system in an intelligent cafe (simulated by 2 leds led )
Instead of owner of the café having to manually turning on/off the air-conditioning  or the ventilation in the room , I implemented an intelligent controller that automatically adjusts temperature settings based on real-time conditions and a web interface that can still give the owner some control over the system by monitoring and adjusting settings






***Features***

* Set ESP32 IP address dynamically .
* Enable/Disable the Automatic Mode 
* Control AC and fan states (on/off) via GraphQL.
* Fetch current states of AC and fan via GraphQL.
* Display a web interface for controlling the devices manually
* Integrate with an external weather API (OpenWeatherMap) to fetch weather data.
* Retrieve real-time temperature and humidity data using a DHT sensor.



***Prerequisites***


* Arduino IDE installed on your machine.
* ESP32 microcontroller board.
* DHT sensor connected to the ESP32 for temperature and humidity sensing.
* 2 LEDs to simulate the AC/Fan
* OpenWeatherMap API key for fetching weather data.
* Node.js installed on your machine.



***Architecture***

![schema de connexion](https://github.com/shy-shai/IOT-projets/assets/165284030/4d5ea5aa-6a1c-482e-a733-74665521351c)





***UP Comming Features***

* a monitoring interface for the tempeture and humidity

* a form to configure which city/country you will read the global tempeture/humidity for 
