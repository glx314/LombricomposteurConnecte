import paho.mqtt.client as mqtt
import ssl
import json
from base64 import b64decode
import requests
auth_token = "eyJhbGciOiJSUzI1NiIsInR5cCIgOiAiSldUIiwia2lkIiA6ICJVbTIzbjBEZnZGX0lPekE3ZlBUUmtXajRrVWZFNTV1b3pLcWd3bGZPR0tnIn0.eyJleHAiOjE1OTU1MTg1MTgsImlhdCI6MTU5NTUxNDkxOCwianRpIjoiY2U1ZjY3OWUtODJmMi00MTUyLWJiOTYtMDM3NWQ0N2MxZmY2IiwiaXNzIjoiaHR0cHM6Ly9zc28uZWdsb2JhbG1hcmsuY29tL2F1dGgvcmVhbG1zL3N0ZWxsaW8iLCJhdWQiOiJhY2NvdW50Iiwic3ViIjoiZjNlNjA2MjAtYTA2ZS00MjQ4LWEwZTEtYmE4NDE2NjMxZDY3IiwidHlwIjoiQmVhcmVyIiwiYXpwIjoiamVua2lucy1pbnRlZ3JhdGlvbiIsInNlc3Npb25fc3RhdGUiOiJjZTdhODEyMC1mODkzLTRiY2UtOGMzNy1kNTllNGY0MjYzMDMiLCJhY3IiOiIxIiwicmVhbG1fYWNjZXNzIjp7InJvbGVzIjpbIm9mZmxpbmVfYWNjZXNzIiwidW1hX2F1dGhvcml6YXRpb24iXX0sInJlc291cmNlX2FjY2VzcyI6eyJqZW5raW5zLWludGVncmF0aW9uIjp7InJvbGVzIjpbInVtYV9wcm90ZWN0aW9uIl19LCJhY2NvdW50Ijp7InJvbGVzIjpbIm1hbmFnZS1hY2NvdW50IiwibWFuYWdlLWFjY291bnQtbGlua3MiLCJ2aWV3LXByb2ZpbGUiXX19LCJzY29wZSI6ImVtYWlsIHByb2ZpbGUiLCJjbGllbnRIb3N0IjoiMTAuNS4xLjEiLCJlbWFpbF92ZXJpZmllZCI6ZmFsc2UsImNsaWVudElkIjoiamVua2lucy1pbnRlZ3JhdGlvbiIsInByZWZlcnJlZF91c2VybmFtZSI6InNlcnZpY2UtYWNjb3VudC1qZW5raW5zLWludGVncmF0aW9uIiwiY2xpZW50QWRkcmVzcyI6IjEwLjUuMS4xIiwiZW1haWwiOiJzZXJ2aWNlLWFjY291bnQtamVua2lucy1pbnRlZ3JhdGlvbkBwbGFjZWhvbGRlci5vcmcifQ.EzsHAOJdbzTVFAo3vlg9Mw2mSUbAKJofaTFTOn6-6C8iKkb7Uh_g3wHVnQxvCntaOB55c6J7q9_fCLkqsYYJWOD88dwnRt-4xiEtMn7yfjMc3cm7Pooeo2y6tYZuMy2QG7uud9pWahGah73uVsmlppbw5d3e3m1j0sQ2bNecWsY32lcFxlIdlZDIE8J-KKcjlrIFjz7kttsPuQ5zUAoFfokkcC9_EBRG---UfUZpfdFSrsbBNziR_cPK4PzCYsydKaUvM_wbNFckt8jCBv9-NTI-Zr4mOr2Uku_9mxcjskJS0OePptlwOMIcSJXIZguV2_S0ohfGkIEFUcNHkzeKGw"
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
    hed = {'Authorization': 'Bearer ' + auth_token}
    rt = requests.patch(url, json=donneesTemp, headers=hed)
    rh = requests.patch(url, data=donneesHum, headers=hed)
    print("\n", rt, " : ", rt.content)
    print("\n", rh, " : ", rh.content)

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
