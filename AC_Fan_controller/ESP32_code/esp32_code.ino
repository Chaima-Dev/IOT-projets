#include <WiFi.h>
#include <HTTPClient.h>
//#include <Arduino_JSON.h>
#include <ArduinoJson.h>
const char* ssid = "your_ssid";
const char* password = "your_password";

#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

//we will use our dht to check the real tempeture of the room then close the ac later 
#include <DHT.h>
#define DHTPIN 21
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Simulating our AC and FAN by two leds 
const int acPin = 18; 
const int fanPin = 19; 

//etat initial est ferme
std::string AC_state = "eteinte";
std::string Fan_state = "eteinte";

String openWeatherMapApiKey = "your_api_key";
// Replace with your country code and city
String city = "your_city";
String countryCode = "your_country_code";
// For Non-HTTPS requests
WiFiClient client;
// Just the base of the URL you want to connect to
#define TEST_HOST "api.openweathermap.org"

//we defined new data type that will hold the responce of the api
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




//function that will get the enable/disabled automatic state
boolean Auto_Mode_State (){
HTTPClient http;
http.begin("http://your_graphql_server_ip_address:4000/graphql");
http.addHeader("Content-Type", "application/json");
// Requête GraphQL
String query = "{\"query\":\"query { getAUTOenabled }\"}";
int httpResponseCode = http.POST(query);
if (httpResponseCode > 0) {
String response = http.getString();
Serial.println(httpResponseCode);
Serial.println(response);

// Analyser la réponse 
DynamicJsonDocument doc(1024);
deserializeJson(doc, response);
bool Auto_Mode = doc["data"]["getAUTOenabled"];
return Auto_Mode;
}
else {
Serial.print("Erreur lors de l'envoi de la requête POST : ");
Serial.println(httpResponseCode);
}
http.end();
}

//send state message to check of automatic mode is well set or not 
void state_message ( String message){

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://your_graphql_server_ip_address:4000/graphql"); 
        http.addHeader("Content-Type", "application/json");
        String query = "{\"query\":\"mutation {setStateMessage(message: \\\"" + String(message) + "\\\") }\"}";
        int httpResponseCode = http.POST(query);
        
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);
            Serial.println(response);
        } else {
            Serial.print("Erreur lors de l'envoi de la requête POST : ");
            Serial.println(httpResponseCode);
            http.POST(query);
        }
        http.end();
    }
}


//we will be getting REST requests from the graphql server 
//THE AC
void handle_rest_POST_request_for_AC() {
    // Handle REST POST request to control AC state
    server.on("/AC", HTTP_POST, [](AsyncWebServerRequest *request) {
//to use url 
  // Retrieve the 'state' parameter from the URL-encoded body
    String stateac = request->getParam("state")->value();
//the syntax of your post url
//POST http://your_graphql_server_ip_address/AC?state=true
        // Toggle the state of the AC based on the payload
        if (stateac == "true") {
            digitalWrite(acPin, HIGH);
            AC_state = "allumee";
           request->send(200, "application/json", "{\"message\":\"AC allume\"}");
        } else if (stateac == "false") {
            digitalWrite(acPin, LOW);
            AC_state = "eteinte";
            request->send(200, "application/json", "{\"message\":\"AC eteint\"}");
        }
        else {
        request->send(200, "application/json", "{\"message\":\"error\"}");
    }
        
    });
}

void handle_rest_GET_request_for_AC() {
server.on("/AC", HTTP_GET, [](AsyncWebServerRequest *request) {
    String acStateMessage = (digitalRead(acPin) == HIGH) ? "Fan on" : "Fan off";
    request->send(200, "text/plain", acStateMessage);
});}

//THE FAN
void handle_rest_POST_request_for_Fan() {
    // Handle REST POST request to control AC state
    server.on("/Fan", HTTP_POST, [](AsyncWebServerRequest *request) {
 // Read the POST body
    String statefan = request->getParam("state")->value();
 

        if (statefan == "true") {
            digitalWrite(fanPin, HIGH);
            Fan_state = "allumee";
           request->send(200, "application/json", "{\"message\":\"Fan allume\"}");
        } else if (statefan == "false") {
            digitalWrite(fanPin, LOW);
            Fan_state = "eteinte";
            request->send(200, "application/json", "{\"message\":\"Fan eteint\"}");
        }
        else {
        request->send(200, "application/json", "{\"message\":\"error\"}");
    }
       
    });
}

void handle_rest_GET_request_for_Fan() {

server.on("/Fan", HTTP_GET, [](AsyncWebServerRequest *request) {
    String fanStateMessage = (digitalRead(fanPin) == HIGH) ? "Fan on" : "Fan off";
    request->send(200, "text/plain", fanStateMessage);
});}


// a function that sends ip address of esp32 to the graphql server
void send_IPaddress_to_GRAPHQL_server(IPAddress ip) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://your_graphql_server_ip_address:4000/graphql"); 
        http.addHeader("Content-Type", "application/json");
        // Format the IP address as a string and include it in the GraphQL mutation query
        String query = "{\"query\":\"mutation {setIPaddress(IPaddress: \\\"" + ip.toString() + "\\\") }\"}";
        int httpResponseCode = http.POST(query);
        
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);
            Serial.println(response);
        } else {
            Serial.print("Erreur lors de l'envoi de la requête POST : ");
            Serial.println(httpResponseCode);
            // Resend the IP address
            http.POST(query);
        }
        http.end();
    }
}












void setup() {
Serial.begin(115200);

//setting the leds 
pinMode(21, INPUT);
  pinMode(acPin, OUTPUT);
  pinMode(fanPin, OUTPUT);

//connection wifi
WiFi.begin(ssid, password);
Serial.println("Connecting");
while(WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.print("Connected to WiFi network with IP Address: ");
Serial.println(WiFi.localIP());
send_IPaddress_to_GRAPHQL_server(WiFi.localIP());


//WEB SERVER 
   // REST REQUEST COMMING THE GRAPHQL SERVER 
handle_rest_POST_request_for_AC();
handle_rest_GET_request_for_AC();
handle_rest_POST_request_for_Fan();
handle_rest_GET_request_for_Fan();
//start the web server
server.begin(); 

//call openweather api
WeatherData data = makeHTTPRequest(data);

//DHT 
dht.begin();
}


//HERE WE ARE USING THE WEATHER API 
unsigned long lastUpdateTime = 0; // Variable to store the last update time
const unsigned long updateInterval = 600000; // Update interval (10 minutes to limite the number of api calls)

void loop() {

//to define the interval of making request to the api
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= updateInterval || lastUpdateTime == 0) {
        // Fetch weather data and update data variable
        data = makeHTTPRequest(data);

        // Update the last update time
        lastUpdateTime = currentTime;
    }

//read from dht 
float h = dht.readHumidity();
float t = dht.readTemperature();
delay(10000);
if (isnan(t) || isnan(h)) {
Serial.println("Failed to read from DHT");}
//for troubleshooti
Serial.println(h);
Serial.println(t);
Serial.println(data.temperature);
Serial.println(data.humidity);

//in each iteration we search for the state of the auto_mode
boolean Auto_Mode = Auto_Mode_State ();
//WHEN TO OPEN AC automatically
if (Auto_Mode == true){
  if (data.temperature > 20) { 
    // Turn on AC 
//we chose small temp for testing purposes 
    digitalWrite(acPin, HIGH); 

     } 
    else if (t<10){  
    digitalWrite(acPin, LOW); 
    }

//WHEN TO OPEN FAN AC automatically
  if (data.humidity> 60) { // Turn on fan LED
    digitalWrite(fanPin, HIGH);
 } 
    else if(h<30){
    digitalWrite(fanPin, LOW); 
    }

state_message ("le mode automatic est acitve");
    //if the automatic mode is enabled we start the automatic on/off of the ac/fan 
}
else{state_message ("attention! le mode automatic is desactive");}

/*the end of the loop*/ }
