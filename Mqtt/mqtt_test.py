import paho.mqtt.client as mqtt
import ssl
import json
from base64 import b64decode
import requests

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

    print(f'\nTemperature : {fTemp} °C \nHumidité : {Hum}%')

    donneesTemp=f"""
                    
    {{
	    "temperature":{{
	    	"type":"Property",
	    	"value":{fTemp},
		    "observedBy":{{
			    "type":"Relationship",
			    "object":"urn:ngsi-ld:Device:01"
		    }}
	    }}

	    "@context": [
	
	    ]
    }}"""

    donneesHum = f"""

        {{
    	    "humidity":{{
    	    	"type":"Property",
    	    	"value":{Hum},
    		    "observedBy":{{
    			    "type":"Relationship",
    			    "object":"urn:ngsi-ld:Device:01"
    		    }}
    	    }}

    	    "@context": [

    	    ]
        }}"""
    client.publish("application/31/device/0004a30b001bad84/decoded",donnees)
    
    url=""

    requests.patch(url, data = json.loads(donneesTemp))
    requests.patch(url, data = json.loads(donneesHum))



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
