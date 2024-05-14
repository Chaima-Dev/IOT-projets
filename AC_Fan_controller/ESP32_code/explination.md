***connect to wifi***

#include <WiFi.h>
const char* ssid = "your_ssid";
const char* password = "your_password";



***Call OpenWeather API*** 


#include <ArduinoJson.h>
String openWeatherMapApiKey = "your_api_key";
String city = "your_city";
String countryCode = "your_country_code";
WiFiClient client;
#define TEST_HOST "api.openweathermap.org"
struct WeatherData {
  float temperature;
  float humidity;
};

WeatherData data;
WeatherData makeHTTPRequest(WeatherData data) {
  if (!client.connect(TEST_HOST, 80))
  {
    Serial.println(F("Connection failed"));
    return data;
  }
  yield();
  // Send HTTP request
  client.print(F("GET "));
  // This is the second half of a request (everything that comes after the base URL)
  client.print("http://api.openweathermap.org/data/2.5/weather?q=" + city + "," +countryCode + "&APPID=" + openWeatherMapApiKey); // %2C == ,
  client.println(F(" HTTP/1.1"));

  //Headers
  client.print(F("Host: "));
  client.println(TEST_HOST);
  client.println(F("Cache-Control: no-cache"));

  if (client.println() == 0)
  {
    Serial.println(F("Failed to send request"));
    return data;
  }
// Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0)
  {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return data;
  }

// Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders))
  {
    Serial.println(F("Invalid response"));
    return data;
  }

  while (client.available() && client.peek() != '{')
  {
    char c = 0;
    client.readBytes(&c, 1);
    Serial.print(c);
    Serial.println("BAD");
  }

  DynamicJsonDocument doc(192);
  DeserializationError error = deserializeJson(doc, client);
  if (!error) {
    float temperature= doc["main"]["temp"]; 
    float humidity = doc["main"]["humidity"]; 
    //for troubleshooting 
    Serial.print("temperature: ");
    Serial.println(temperature);
    Serial.print("humidity: ");
    Serial.println(humidity);
float temperatureCelsius = temperature - 273.15;
Serial.println(temperatureCelsius);
data.humidity = humidity;
data.temperature =temperatureCelsius;
  } else {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return data;
  }
   return data;
}




***Handle API server***
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);
