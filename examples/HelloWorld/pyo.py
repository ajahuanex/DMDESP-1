from pynput.keyboard import Listener
from time import process_time, sleep
import paho.mqtt.client as mqtt
from threading import Timer


mqttBroker = "127.0.0.1"
client =mqtt.Client('Pepito')
client.connect(mqttBroker,8883)
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
