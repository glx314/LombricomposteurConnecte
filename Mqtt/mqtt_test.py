import paho.mqtt.client as mqtt
import ssl
import json
from base64 import b64decode
import requests
auth_token = "eyJhbGciOiJSUzI1NiIsInR5cCIgOiAiSldUIiwia2lkIiA6ICJVbTIzbjBEZnZGX0lPekE3ZlBUUmtXajRrVWZFNTV1b3pLcWd3bGZPR0tnIn0.eyJleHAiOjE1OTU1ODgwNjUsImlhdCI6MTU5NTU4NDQ2NSwianRpIjoiYmU5YmJhM2YtYjZkMS00ZGZmLTg3ZjItM2NlYzZkNzZkMWU4IiwiaXNzIjoiaHR0cHM6Ly9zc28uZWdsb2JhbG1hcmsuY29tL2F1dGgvcmVhbG1zL3N0ZWxsaW8iLCJhdWQiOiJhY2NvdW50Iiwic3ViIjoiZjNlNjA2MjAtYTA2ZS00MjQ4LWEwZTEtYmE4NDE2NjMxZDY3IiwidHlwIjoiQmVhcmVyIiwiYXpwIjoiamVua2lucy1pbnRlZ3JhdGlvbiIsInNlc3Npb25fc3RhdGUiOiIxZWE2MTMxNC02NTgzLTRjYTItYTc0Zi01MjhlMWNjOGE0NDQiLCJhY3IiOiIxIiwicmVhbG1fYWNjZXNzIjp7InJvbGVzIjpbIm9mZmxpbmVfYWNjZXNzIiwidW1hX2F1dGhvcml6YXRpb24iXX0sInJlc291cmNlX2FjY2VzcyI6eyJqZW5raW5zLWludGVncmF0aW9uIjp7InJvbGVzIjpbInVtYV9wcm90ZWN0aW9uIl19LCJhY2NvdW50Ijp7InJvbGVzIjpbIm1hbmFnZS1hY2NvdW50IiwibWFuYWdlLWFjY291bnQtbGlua3MiLCJ2aWV3LXByb2ZpbGUiXX19LCJzY29wZSI6ImVtYWlsIHByb2ZpbGUiLCJjbGllbnRIb3N0IjoiMTAuNS4xLjEiLCJlbWFpbF92ZXJpZmllZCI6ZmFsc2UsImNsaWVudElkIjoiamVua2lucy1pbnRlZ3JhdGlvbiIsInByZWZlcnJlZF91c2VybmFtZSI6InNlcnZpY2UtYWNjb3VudC1qZW5raW5zLWludGVncmF0aW9uIiwiY2xpZW50QWRkcmVzcyI6IjEwLjUuMS4xIiwiZW1haWwiOiJzZXJ2aWNlLWFjY291bnQtamVua2lucy1pbnRlZ3JhdGlvbkBwbGFjZWhvbGRlci5vcmcifQ.DEGnjzGmP0itp4tH2vCH9a4mhvtq6da857kfZ7v_G617Pr1JP5ohQM6X503JjXTa96-j9iZAHr5qir5RSqi59LMyW631SGyJE0Mpt3YHCkP-JZnRnULLm5ozedKDLXK6ikinU9oz4JlHTmbrA7NC-8wmtYc_r2o01_zo57XfhZkA-xb3t3vob5MJFfsj2I8Lx65yYcHCwETXnkaVcKdW5qmcLaL3vUzsvZrUg0cyydgXKHkudKNxW2qZC8AF4H4w_QRqhn_rEgGfjDB0qXgUb-NOjUn4YhQJqMo6c1F3Kptltepc3xXiuELSeZEIKgw6Kz-ds4__jTTFQ-2H5Hyo6Q"

def get_token():
    url = "https://data-hub.eglobalmark.com/auth/realms/datahub/protocol/openid-connect/token"
    payload={"grant_type":"client_credentials","client_secret":"8c79cec3-db58-47fc-9c84-24296b26cee8","client_id":"jenkins-integration"}
    hed={"Content-Type": "application/x-www-form-urlencoded; charset=utf-8"}
    r=requests.post(url=url,data=payload,headers=hed)
    dec=json.loads(r.content)
    token=dec["access_token"]
    print(f"token : {token}\n")
    return token

auth_token=get_token()


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
    print(f'Data : {data}\n')
    b=data.hex()
    sTemp=b[:4]
    sHum=b[4:]
    iTemp=int(sTemp, 16)
    fTemp=iTemp/10
    Hum=int(sHum,16)

    donnees = f'Temperature : {fTemp} °C \nHumidité : {Hum}%'

    print(donnees)

    donneesTemp ={"temperature":{ "type":"Property", "value":fTemp, "observedBy":{ "type":"Relationship", "object":"urn:ngsi-ld:Device:01"}}}

    donneesHum = {"humidity":{"type":"Property","value":Hum,"observedBy":{"type":"Relationship","object":"urn:ngsi-ld:Device:01"}}}

    url = "https://data-hub.eglobalmark.com/ngsi-ld/v1/entities/urn:ngsi-ld:Composteur:01/attrs"



    hed = {'Link':"<https://raw.githubusercontent.com/easy-global-market/ngsild-api-data-models/master/apic/jsonld-contexts/apic-compound.jsonld>; rel=http://www.w3.org/ns/json-ld#context; type=application/ld+json",'Authorization': 'Bearer ' + auth_token}
    rt = requests.patch(url, json=donneesTemp, headers=hed)
    rh = requests.patch(url, json=donneesHum,  headers=hed)
    print("\n", rt)
    print("\n", rh)

    if (rt >= 400) or (rh >= 400):
      token=get_token()


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
