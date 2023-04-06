// AWS defines which topic you are allowed to subscribe and publish too. This is
// defined by the policy The default policy with the Microchip IoT Provisioning
// Tool allows for publishing and subscribing on thing_id/topic. If you want to
// publish and subscribe on other topics, see the AWS IoT Core Policy
// documentation.
/*#define MQTT_SUB_TOPIC_FMT "%s/sensors"
#define MQTT_PUB_TOPIC_FMT "%s/sensors"

char mqtt_sub_topic[128];
char mqtt_pub_topic[128];

void initAWS() {
  if (!status) {
      Serial3.print("Starting initialization of MQTT Polling for AWS\r\n");
      Serial3.print("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial3.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial3.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial3.print("        ID of 0x60 represents a BME 280.\n");
      Serial3.print("        ID of 0x61 represents a BME 680.\n");
      while (1) delay(10);
  }
}

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
    for (uint8_t i = 0; i < 3; i++) {
        bool published_successfully =
            MqttClient.publish(mqtt_pub_topic, "{\"light\": 9, \"temp\": 9}");

        if (published_successfully) {
            Serial3.println("Published message");
        } else {
            Serial3.println("Failed to publish\r\n");
        }

        delay(2000);

        String message = MqttClient.readMessage(mqtt_sub_topic);

        // Read message will return an empty string if there were no new
        // messages, so anything other than that means that there were a
        // new message
        if (message != "") {
            Serial3.println("Got new message: %s\r\n" );
            Serial3.println(message.c_str());
        }

        delay(2000);
    }
}

int setupAWS() {

    Serial3.println("Starting MQTT Polling for AWS example\r\n");

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
}*/