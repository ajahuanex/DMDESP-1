from pynput.keyboard import Listener
from time import process_time, sleep
import paho.mqtt.client as mqtt
from threading import Timer
import time


mqttBroker = "127.0.0.1"
client =mqtt.Client('Pepito')
# client.connect(mqttBroker,1883)
# print(client.status())

def on_connect(client, userdata, flags, rc):
    if rc==0:
        client.connected_flag=True #set flag
        print("connected OK")
    else:
        print("Bad connection Returned code=",rc)

mqtt.Client.connected_flag=False#create flag in class
# broker="192.168.1.184"
client = mqtt.Client("python1")             #create new instance 
client.on_connect=on_connect  #bind call back function
client.loop_start()
print("Connecting to broker ",mqttBroker)
client.connect(mqttBroker,1883)      #connect to broker
while not client.connected_flag: #wait in loop
    print("In wait loop")
    time.sleep(1)
print("in Main Loop")
client.publish("Temperature","ok")
client.loop_stop()    #Stop loop 
# client.disconnect() # disconnect

client.publish("Temperature","ok again")


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
                         
    if len(a) == 13:
        listToStr = ''.join(map(str, a))
        print(listToStr)
        client.publish("Temperature",listToStr)
        end=process_time()
        print(end)
        #if end > 2:        
        a.clear()    
                
with Listener(on_press=key_recorder) as l:       
    l.join()
