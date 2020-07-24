import paho.mqtt.client as mqtt
import ssl
import json
from base64 import b64decode
import requests
auth_token = "eyJhbGciOiJSUzI1NiIsInR5cCIgOiAiSldUIiwia2lkIiA6ICJVbTIzbjBEZnZGX0lPekE3ZlBUUmtXajRrVWZFNTV1b3pLcWd3bGZPR0tnIn0.eyJleHAiOjE1OTU1ODEwOTUsImlhdCI6MTU5NTU3NzQ5NSwianRpIjoiMTg1NzBkNWUtNWJhMS00MGI4LTg2ZGEtNWJjOWI0MTZiMmEyIiwiaXNzIjoiaHR0cHM6Ly9zc28uZWdsb2JhbG1hcmsuY29tL2F1dGgvcmVhbG1zL3N0ZWxsaW8iLCJhdWQiOiJhY2NvdW50Iiwic3ViIjoiZjNlNjA2MjAtYTA2ZS00MjQ4LWEwZTEtYmE4NDE2NjMxZDY3IiwidHlwIjoiQmVhcmVyIiwiYXpwIjoiamVua2lucy1pbnRlZ3JhdGlvbiIsInNlc3Npb25fc3RhdGUiOiJlY2M1ZjUyMy0xNzExLTRlOGUtODJiMC1lNDc1MjUxY2E4YmEiLCJhY3IiOiIxIiwicmVhbG1fYWNjZXNzIjp7InJvbGVzIjpbIm9mZmxpbmVfYWNjZXNzIiwidW1hX2F1dGhvcml6YXRpb24iXX0sInJlc291cmNlX2FjY2VzcyI6eyJqZW5raW5zLWludGVncmF0aW9uIjp7InJvbGVzIjpbInVtYV9wcm90ZWN0aW9uIl19LCJhY2NvdW50Ijp7InJvbGVzIjpbIm1hbmFnZS1hY2NvdW50IiwibWFuYWdlLWFjY291bnQtbGlua3MiLCJ2aWV3LXByb2ZpbGUiXX19LCJzY29wZSI6ImVtYWlsIHByb2ZpbGUiLCJjbGllbnRIb3N0IjoiMTAuNS4xLjEiLCJlbWFpbF92ZXJpZmllZCI6ZmFsc2UsImNsaWVudElkIjoiamVua2lucy1pbnRlZ3JhdGlvbiIsInByZWZlcnJlZF91c2VybmFtZSI6InNlcnZpY2UtYWNjb3VudC1qZW5raW5zLWludGVncmF0aW9uIiwiY2xpZW50QWRkcmVzcyI6IjEwLjUuMS4xIiwiZW1haWwiOiJzZXJ2aWNlLWFjY291bnQtamVua2lucy1pbnRlZ3JhdGlvbkBwbGFjZWhvbGRlci5vcmcifQ.NGmT4gi-9komstN3CEJyZXIAk56uIVn8gLCfDhZf8rN2TaQk1-JCMldqLG42zEdpoyL0NYMg4cuc7R7ZRkXjoTI9hDL6jMNWpAGCzfMFep0XaedX2H5zFwcyDGfNDAupVx--BRafegkJSkR2uKVcSgFJn_7UR6xyFOh3adi0fctzd_uipgnZn_WztO6iWV6b6blxJN4cAHoOYARlEWM06fe3j4LG9mq-SbJ3Bd49smb7ZrT3_xDMNOMIj90lS9j-zVqF8wPrJMek91fKH9v0Nz1leZFUkR0197DyGtkexGJjotS1x1TQ34XfB-URy5To9oHn0MNIY3VqlLNiWtD0Qg"
def on_connect(client, userdata, flags, rc):
    if rc != 0:
        print("Failed to connect to MQTT broker")
        exit(-1)
    #print("Successfully connected to MQTT")    
    client.subscribe("application/31/device/0004a30b001bad84/rx")

def on_message(client, userdata, message):
    print(f"RX:{message.topic}:{message.payload}")
    decoded = json.loads(message.payload)
    data = b64decode(decoded['data'])
    print(data)
    b=data.hex()
    sTemp=b[:4]
    sHum=b[4:]
    iTemp=int(sTemp, 16)
    fTemp=iTemp/10
    Hum=int(sHum,16)

    donnees = f'\nTemperature : {fTemp} °C \nHumidité : {Hum}%'

    print(donnees)

    donneesTemp ={"temperature":{ "type":"Property", "value":fTemp, "observedBy":{ "type":"Relationship", "object":"urn:ngsi-ld:Device:01"}}}

    donneesHum = {"humidity":{"type":"Property","value":Hum,"observedBy":{"type":"Relationship","object":"urn:ngsi-ld:Device:01"}}}

    url = "https://data-hub.eglobalmark.com/ngsi-ld/v1/entities/urn:ngsi-ld:Composteur:01/attrs"
    hed = {'Link':"<https://raw.githubusercontent.com/easy-global-market/ngsild-api-data-models/master/apic/jsonld-contexts/apic-compound.jsonld>; rel=http://www.w3.org/ns/json-ld#context; type=application/ld+json",'Authorization': 'Bearer ' + auth_token}
    rt = requests.patch(url, json=donneesTemp, headers=hed)
    rh = requests.patch(url, json=donneesHum,  headers=hed)
    print("\n", rt)
    print("\n", rh)

    client.publish("application/31/device/0004a30b001bad84/decoded",donnees)
    



client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.tls_set(
    ca_certs="secret_tetris/ca.crt",
    certfile="secret_tetris/gorazi.crt",
    keyfile="secret_tetris/gorazi.key",
    cert_reqs=ssl.CERT_REQUIRED,
    tls_version=ssl.PROTOCOL_TLS,
    ciphers=None
)            

client.connect("lora.smart-territory.eu", 8883, True)
client.loop_forever()
