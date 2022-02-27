from pynput.keyboard import Listener
from time import process_time, sleep
import paho.mqtt.client as mqtt
from threading import Timer
import time

# python 3.6

import random
import time

from paho.mqtt import client as mqtt_client


broker = '127.0.0.1'
port = 1883
topic = "python/mqtt"
mensaje = "Ok"
# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 1000)}'
# username = 'emqx'
# password = 'public'

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    # client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def publish(client):
    a=[]
    start=process_time()
    def key_recorder(key):
        def twoArgs():               
            a.clear() 
        r = Timer(1.0, twoArgs, ())
        r.start()
            
        key=str(key)      
        
        a.append(key.replace("'",""))
        if key == 'Key.shift':
            del a[-1]   
        if key == 'Key.ctrl':
            del a[-1]  
                            
        if len(a) == 13:
            listToStr = ''.join(map(str, a))
            print(listToStr)
            client.publish(topic,listToStr)
            end=process_time()
            print(end)
            #if end > 2:        
            a.clear()    
                    
    with Listener(on_press=key_recorder) as l:       
        l.join()    
        
    


def run():
    client = connect_mqtt()
    client.loop_start()
    publish(client)


if __name__ == '__main__':
    run()
    
