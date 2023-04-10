// AWS defines which topic you are allowed to subscribe and publish too. This is
// defined by the policy The default policy with the Microchip IoT Provisioning
// Tool allows for publishing and subscribing on thing_id/topic. If you want to
// publish and subscribe on other topics, see the AWS IoT Core Policy
// documentation.
#define MQTT_SUB_TOPIC_FMT "%s/sensors"
#define MQTT_PUB_TOPIC_FMT "%s/sensors"

char mqtt_sub_topic[128];
char mqtt_pub_topic[128];

bool initMQTTTopics() {
  ECC608.begin();

  // Find the thing ID and set the publish and subscription topics
  uint8_t thingName[128];
  uint8_t thingNameLen = sizeof(thingName);

  // -- Get the thingname
  uint8_t err = ECC608.getThingName(thingName, &thingNameLen);
  if (err != ECC608.ERR_OK) {
      Serial3.println("Could not retrieve thingname from the ECC");
      return false;
  }

  sprintf(mqtt_sub_topic, MQTT_SUB_TOPIC_FMT, thingName);
  sprintf(mqtt_pub_topic, MQTT_PUB_TOPIC_FMT, thingName);
  return true;
}

void sendData(){
  // Test MQTT publish and receive
  for (uint8_t ii = 0; ii < AQDataSize/24; ii++) { // sending 12 blocks of data containing 24 data points 12 * 24 = 288 ENSURE THAT AQDataSize % 24 == 0
    // checks to make sure connection is still on going
    if (!MqttClient.isConnected()) {
      Serial3.println("Reconnecting to MQTT broker...");
      // Attempt to reconnect
      MqttClient.beginAWS();
      while (!MqttClient.isConnected()) {
        Serial3.println(".");
        delay(500);
      }
      Serial3.println("Connected to MQTT broker");
      // Resubscribe to the topic
      MqttClient.subscribe(mqtt_sub_topic);
    }

    bool published_successfully = false;
    uint8_t attempts = 0;
    
    // in case of failure to send the data it will try again but otherwise data will be tossed
    while (!published_successfully && attempts < 2) {
      published_successfully = MqttClient.publish(mqtt_pub_topic, stringify(ii * 24, ii * 24 + 24).c_str());
      
      if (published_successfully) {
        Serial3.println("Published message");
      } else {
        Serial3.println("Failed to publish\r\n");
        attempts++;
        delay(1500);
      }

      String message = MqttClient.readMessage(mqtt_sub_topic);

      // Read message will return an empty string if there were no new
      // messages, so anything other than that means that there were a
      // new message
      if (message != "") {
          Serial3.println("Got new message: %s\r\n" );
          Serial3.println(message.c_str());
      }
    }
  }
}

int setupAWS() {

  Serial3.println("Starting MQTT Polling for AWS\r\n");

  if (initMQTTTopics() == false) {
      Serial3.println("Unable to initialize the MQTT topics. Stopping...");
      return -1;
  }

  if (!Lte.begin()) {
      Serial3.println("Failed to connect to operator");

      // Halt here
      return -2;
  }

  // Attempt to connect to the broker
  if (MqttClient.beginAWS()) {
      Serial3.println("Connecting to broker");

      while (!MqttClient.isConnected()) {
          Serial3.println(".");
          delay(500);
      }

      Serial3.println(" OK!\r\n");

      // Subscribe to the topic
      MqttClient.subscribe(mqtt_sub_topic);
      return 0;
  } else {
      Serial3.println("\r\n");
      Serial3.println("Failed to connect to broker");
      // Halt here
      return -3;
  }   
}