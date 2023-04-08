![GitHub Contributors Image](https://contrib.rocks/image?repo=exposedtalent/MicrochipCapstone)

![AWS](https://img.shields.io/badge/Amazon_AWS-FF9900?style=for-the-badge&logo=amazonaws&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino_IDE-00979D?style=for-the-badge&logo=arduino&logoColor=white)

 
# Microchip Capstone

This is the repository for the Microchip Capstone project called 'Microchip Technology - Determining Air Quality Around 
ASU Temp Campus using Embedded Systems and Machine Learning'. 

The project is made by [Tanishq Mor](https://github.com/exposedtalent), [Carter Dunn](https://github.com/CarterDunn),
[Amber Kahklen](https://github.com/amberkahklen), [Maxwell Calhoun](https://github.com/Maxwell-Calhoun), [Hunter White](https://github.com/Hoopenflarg), and [David Mangoli](https://github.com/DavidMangoli)

# Project Overview
The project is a collaboration between Microchip Technology and Arizona State University. The goal of the project is to develop a fully functioning embedded system that can be used to determine the air quality around the ASU Tempe campus. The embedded system will be able to measure the air quality using a Dust PM 2.5, ZMOD4510  and BME 280 sensor and send the data to AWS IoT Core. 

# Table of Contents
- [Project Overview](#project-overview)
- [Table of Contents](#table-of-contents)
- [Hardware](#hardware)
  - [AVR-Cellular mini](#avr-cellular-mini)
  - [Dust PM 2.5](#dust-pm-25)
  - [ZMOD4510](#zmod4510)
  - [BME 280](#bme-280)
- [Software](#software)
  - [Development Environment Setup](#development-environment-setup)
  - [AWS Guide to Setting up IoT Core](#aws-guide-to-setting-up-iot-core)
    - [Create a Thing in IoT Core](#create-a-thing-in-iot-core)
    - [Create a new Role for AWS Lambda](#create-a-new-role-for-aws-lambda)
    - [Create a Rule in IoT Core](#create-a-rule-in-iot-core)
    - [Create a DynamoDB Table](#create-a-dynamodb-table)
    - [Code with AWS Lambda](#code-with-aws-lambda)
    - [Test the Lambda Function](#test-the-lambda-function)
- [Common Issues](#common-issues)
- [Conclusion](#conclusion)

# Hardware
## AVR-Cellular mini

The AVR-Cellular mini is a development board that is used to connect to the internet. It is powered by the ATmega4809 microcontroller and has a built in cellular modem. The board has a built in antenna and a micro USB port for power.  [Link for purchase](https://www.microchip.com/en-us/development-tool/EV70N78A)

## Dust PM 2.5

The Dust PM 2.5 sensor is powered by Sharp GP2Y1010AU0F. The Sharp GP2Y1010AU0F can detect fine particle larger than 0.8μm in diameter, even like the cigarette smoke. Low power consumption, analog voltage output, the output level is linear with dust density. Embedded voltage boost circuit to support wide range of power supply. [Link for purchase](https://www.waveshare.com/dust-sensor.htm)

## ZMOD4510 Module 

The ZMOD4510 is a sensor that collects NO2, and O3 gases. The sensor we got was from pcbartists. The sensor is a ZMOD4510 module with a 3.3V regulator. The sensor is powered by 3.3V and has a 0.1" header for the I2C communication. [Link for purchase](https://www.pcbartists.com/product/zmod4510-module/)  
## BME 280

This sensor is used to measure the temperature, humidity, and pressure. This comes from SparkFun and is a breakout board for the BME280 sensor. There is a QWICC connector for the I2C communication. [Link for purchase](https://www.sparkfun.com/products/15440)


# Software


## Development Environment Setup

1. An AWS account.
2. AWS CLI installed and configured in your computer. [Instructions](https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-install.html)
3. Once the AWS Cli is installed, you will configure the cli to have your credentials. Please use ``aws configure`` command. [Instructions](https://docs.aws.amazon.com/cli/latest/userguide/cli-configure-files.html)
4. In order to work with the ``BME 280`` sensor you will need to install ``Adafruit BME280 Library`` for your Library manager. Link to the github for the [library](https://github.com/adafruit/Adafruit_BME280_Library).

## AWS Guide to Setting up IoT Core
### Create a Thing in IoT Core

- In the AWS IoT Core console, choose Manage, choose All devices then choose Things. We will using this to confirm that the device is connected to AWS IoT Core.
- Now download the AVR-Cellular mini IOT Provisioning tool from the [Microchip website](https://iot.microchip.com/docs/arduino/userguide/provisioning).
- Connect the AVR-Cellular mini to your computer using a micro USB-C cable.
- Open terminal and navigate to the folder where you downloaded the IOT Provisioning tool.
- Once at the folder, please choose your operating system folder. For this project we used Mac OS.
- Now run the following command: ``./iotprovision-bin -m mar -c aws``. This will create a new thing in AWS IoT Core if everything is configured correctly.
- Verify that the thing name in the terminal is the same as the one you see in the AWS IoT Core console.
- You should see a message that says ``Operation completed successfully``. If you see this message, you can now move on to the next step.

### Create a new Role for AWS Lambda
- Make your way to the AWS IAM console by searching for it in the search bar.
- Select Roles from the left hand side menu.
- Select Create Role.
- Select AWS Service from the list of trusted entities.
- Select Lambda from the list of services that use AWS Lambda.
- Select Next: Permissions.
- Search for ``dynamodb`` and select the ``AmazonDynamoDBFullAccess`` policy.
- Select Next.
- Enter a name for the role. For this project we used ``lambda-dynamodb-role``.
- Select Create Role.

### Create a Rule in IoT Core
- In the AWS IoT Core console, under Manage heading there is a Message routing option click on that. Then select Rules.
- Select Create.
- Enter a name for the rule. For this project we used ``SentToDynamodb``.
- Select Next.
- Here is where we need to put an SQL statement. For this project we used the following statement: ``SELECT * FROM '<your MQTT topic>'``. For our project we used ``SELECT * FROM '<thingname>/sensors'``. Using this statement will send all the data from the sensors to the DynamoDB table.
- Select Next.
- Under the Rules actions section, select Add action.
- Select Lambda.
- Select create a lambda function. This will open a new tab in your browser. Give it a name for this project we called it ``messageRedirectToDynamodb``, select the runtime as Python 3.9. Now select "Change the default execution role" and select the "Use an existing role". Use the role you created in the previous step. For this project we used ``lambda-dynamodb-role``.
- Select Create function.
- Now head back to the AWS IoT Core tab and click refresh. You should see the Lambda function you just created. Select it.
- Select Next and then select Create rule.
-Your created rule should now be visible in the list of rules!

### Create a DynamoDB Table
- Navigate to the AWS DynamoDB console by searching for it in the search bar.
- In the AWS DynamoDB console, select Create table.
- Enter a name for the table. For this project we used ``MicrochipCapstone``.
- Select Primary key and enter ``Timestamp`` as the partition key.
- Select Create.

### Code with AWS Lambda
- Navigate to the AWS Lambda console by searching for it in the search bar.
- Select the function you created in the previous step. 
- Now we will be working with the code. Select the ``Code`` tab.
- Here is where we will be adding the code to send the data to the DynamoDB table. For this project we used the following code:
```python
import boto3
from time import strftime, localtime

def lambda_handler(event, context):
    # The event is a list of dictionaries. Each dictionary is a sensor reading.
    print("The length of the event : %d",len(event))
    for i in range(len(event)):
        isSent = sendDataToDynamodb(event[i])
        if isSent == False:
            # Printing for CloudWatch logs
            print("There was an error with parse or sending the data")
    return 0
    
def sendDataToDynamodb(data):
    try:
        # Establish a connection to DynamoDB. 
        dynamodb = boto3.resource('dynamodb')
        table1 = dynamodb.Table("CapstoneData")
        # Humidity is an int to keep the data size small. We need to convert it to a decimal.
        humidity = data["h"] / 100
        table1.put_item(
            Item = {
                # This is the primary key. The time we are sending from the board is in unix time so we need to convert it to a string.
                "Timestamp": strftime('%Y-%m-%d %H:%M:%S', localtime(data["t"])),
                "NO2": str(data["n"]),
                "O3": str(data["o"]),
                "Dust": data["d"],
                "Humidity": str(humidity) ,
                "Temperature": data["c"],
                "id": data["i"]
            }    
        ) 
        return True
    except KeyError:
        # Printing for CloudWatch logs
        print("There was a Key Error. Here is the data that came")
        print(data)
        return False
```
This code will parse the data from the sensors and send it to the DynamoDB table. The code is written in Python 3.9. This can also be found in the AWS folder in the github repository.

- Save the code and deploy the function.
- Now we need to test the function. Select the ``Test`` tab.
- Select ``Configure test event``.
- Select ``Create new test event``.
- Enter a name for the test event. For this project we used ``testEvent``. You can put in test data that is also in the AWS folder in the github repository. This is the data that will be sent to the DynamoDB table. Select ``Create``.
- Select ``Test``.
- You should see a message that says ``Execution result: succeeded``. If you see this message, You are almost done!

### Test the AWS infrastructure
- Now we need to test the AWS infrastructure. Upload the arduino code on the AVR-Cellular mini. You can find the code in the github repository.  
- Open the serial monitor and you should see the data being sent to the AWS IoT Core. 
- Now head back to the AWS IoT Core console. You should see the data being sent to the DynamoDB table.
- Now you are done! You can now view the data in the DynamoDB table.


# Common Issues

- If you are having issues with the AWS IoT Core, make sure you have the correct certificates and keys.
- If you are having issues with the AWS Lambda function, make sure you have the correct role and the correct code.
- If you are having issues with the DynamoDB table, make sure you have the correct table name and the correct primary key.
- If you are having issues with the AWS IoT Core rule, make sure you have the correct SQL statement and the correct Lambda function.
- If you get a timeout error in AWS Lambda, make sure you have the correct timeout value. You can change it by going to the ``Configuration`` tab and then select ``General Configuration``, click on edit and changing the value in the ``Timeout`` section.
- While using the IOT provisional tool make sure the device is connected the computer correct or else you will get an error: ``ERROR - Provisioning unable to start - no suitable IoT kits found``
# Conclusion

In this project, we were able to create a system that can monitor the air quality around ASU campus as well as send the data to AWS IOT Core. We also talked about how the AWS Pipeline looks like. 
