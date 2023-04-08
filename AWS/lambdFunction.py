import boto3
from time import strftime, localtime

def lambda_handler(event, context):
    print("The length of the event : %d",len(event))
    for i in range(len(event)):
        isSent = sendDataToDynamodb(event[i])
        if isSent == False:
            print("There was an error with parse or sending the data")
    return 0
    
def sendDataToDynamodb(data):
    try:
        dynamodb = boto3.resource('dynamodb')
        table1 = dynamodb.Table("CapstoneData")
        humidity = data["h"] / 100
        table1.put_item(
            Item = {
                "Timestamp": strftime('%Y-%m-%d %H:%M:%S', localtime(data["t"])),
                "NO2": str(data["n"]),
                "O3": str(data["o"]),
                "Dust": data["d"],
                "Humidity": str(humidity) ,
                "Temparature": data["c"],
                "id": data["i"]
            }    
        ) 
        return True
    except KeyError:
        print("There was a Key Error. Here is the data that came")
        print(data)
        return False