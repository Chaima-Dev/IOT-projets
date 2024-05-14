const express = require('express');
const { graphqlHTTP } = require('express-graphql');
const { makeExecutableSchema } = require('graphql-tools');
//to be able to send HTTP request to esp32
//npm install axios
const axios = require('axios');

// a function that sends http get request to fetch data from ESP32
function getStateFromESP32(device ,IP) {
  const esp32Endpoint = `http://${IP}/${device}`;
  return axios.get(esp32Endpoint)
    .then(response => {
      console.log(`Successfully fetched ${device} state from ESP32:`, response.data);
      return response.data;
    })
    .catch(error => {
      console.error(`Error fetching ${device} state from ESP32:`, error);
      throw error;
    });
}


// a function that sends http post request to ESP32
function sendStateToESP32(device, state , IP) {
const esp32Endpoint = `http://${IP}/${device}?state=${state}`;
axios.post(esp32Endpoint,{})
  .then(response => {
    console.log(`Successfully sent ${device} state to ESP32: ${state}`);
  })
  .catch(error => {
    console.error(`Error sending ${device} state to ESP32:`, error);
  });
  }


const typeDefs = `
type Query {
    getACState: String
    getFanState: String
    getIPaddress:String

    getStateMessage:String

    getAUTOenabled: Boolean
  }
  
  type Mutation {
    setACState(state: Boolean!): Boolean
    setFanState(state: Boolean!): Boolean
    setIPaddress(IPaddress:String!):String

    setStateMessage(message:String):String

    setAUTOenabled(enable:Boolean): Boolean
  }
  
`;


//initialize the state as false 
let ACState = false;
let FanState= false;
let IP ;
let enabled = false;
let StateMessage = "attention! the  automatic mode is disabled"

const resolvers = {
    Query: {
      //it works
      getACState: () => {
        console.log("ACState:", ACState);
        return getStateFromESP32('AC' , IP);
      },
      //it works
      getFanState: () => {
        console.log("FanState:", FanState);
        return getStateFromESP32('Fan' ,IP);
      },

      
      getIPaddress: ()=>{
        return IP
      },
      getStateMessage: () =>{
        return StateMessage
      },

      //i added this 
      getAUTOenabled:()=>{ return enabled }
    },
    Mutation: {
      //whenever the mutation gets triggered by the webinterface
      //it sends a post request to esp32 to light up the led 
      setACState: (_, { state }) => {
        ACState = state
        sendStateToESP32('AC', state , IP);
        return ACState;
      },
      //it works
      setFanState: (_, { state }) => {
        FanState = state
        
        sendStateToESP32('Fan', state , IP);
        return  FanState;
      },
      //we will implement a mutation so that the esp sends its address eachtime it connects to the internet 
      setIPaddress:(_,{IPaddress})=>{
        IP= IPaddress
        return IP
      },
      setStateMessage : (_, {message}) =>{
        StateMessage=message
        return StateMessage
      },
      setAUTOenabled : (_,{enable})=>{        
        enabled = enable
        return enabled} 
    }
  };


const app = express();



// Middleware to enable CORS
app.use((req, res, next) => {
  res.setHeader('Access-Control-Allow-Origin', 'http://127.0.0.1:5500'); // Replace with the appropriate origin
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type, Authorization');
  res.setHeader('Access-Control-Allow-Credentials', 'true');
  if (req.method === 'OPTIONS') {
      return res.sendStatus(200);
  }
  next();
});



const schema = makeExecutableSchema({
  typeDefs,
  resolvers,
  });

// Définir le point d'accès GraphQL avec express-graphql
app.use('/graphql', graphqlHTTP({
 schema: schema,
 graphiql: true, // Activer l'interface GraphiQL pour les tests
}));

// Add a route to serve the HTML file for the web interface
app.get('/', (req, res) => {
  res.sendFile(__dirname + '/web_interface.html');
});
app.listen(4000, () => console.log('Serveur GraphQL Express en cours d exécution sur localhost:4000/graphql'));
//http://127.0.0.1:5500/graphql-server/web_interface.html
app.listen(5500, () => {
  console.log('Web interface available at http://127.0.0.1:5500/graphql-server/web_interface.html');
});