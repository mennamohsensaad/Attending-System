

import serial
#import pymongo
import time
import array as arr

#myclient = pymongo.MongoClient("mongodb://localhost:27017/")
#mydb = myclient["task4"]
#mycol2 = mydb["rfid2"]
#mycol = mydb["rfid"]
#print("Start")

port="COM10" #This will be different for various devices and on windows it will probably be a COM port.

bluetooth=serial.Serial(port, 9600)#Start communications with the bluetooth unit

print("Connected")

bluetooth.flushInput() #This gives the bluetooth a little kick
#myArray = [1, 2, 3, 4, 5] 
##numbers = arr.array('i', [])
temp_list = []
#for i in range(5): #send 5 groups of data to the bluetooth
    #bluetooth.write(str.encode("A"))
    #print("Ping")

    #time.sleep(0.1) #A pause between bursts
	#bluetooth.write(b"BOOP "+str.encode(str(i)))#These need to be bytes not unicode, plus a number

while True:
    time.sleep(1)
    try:
       input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
       #print(input_data.decode())#These are bytes coming in so a decode is needed
      # numbers = arr.array('i', [])
       X = int(input_data.decode())
       temp_list.append((int(input_data.decode())))
       if X == 0 :
          temp_list=[]
       ##num=numbers[0]
       rfid= str(temp_list[0]) + str(temp_list[1]) + str(temp_list[2])
       print(rfid)     # Output: array('i', [1, 2, 3, 4])
       print (len(temp_list))
       if len(temp_list) >= 3:
          temp_list=[]

      
       pieces=input_data.decode().split(" ")
    ##   print(pieces[2])
#       mylist = [
#              ## {  "name":"fawry" ,"rfid": "04884502326480", "code": "110"},
#               {  "name":"HOSNA" ,"rfid": "41831", "code": "111"},
#               {  "name":"WHITE" ,"rfid": "879435", "code": "112"},
#               {  "name":"HADER" ,"rfid": "45480", "code": "113"},
#               {  "name":"MennaMohsen" ,"rfid": "48571", "code": "210"},
#               {  "name":"MennaAbdalla" ,"rfid": "43780", "code": "211"},
#               {  "name":"BLUE" ,"rfid": "868126", "code": "212"}
#                ]
#       x = mycol2.insert_many(mylist)
       myquery = { "rfid": rfid }
       mydoc = mycol2.find(myquery)
       print(mydoc)
       for x in mydoc:
          print(x)
       x = mycol.insert(x)
       
    except :
       print( "wait")
bluetooth.close() #Otherwise the connection will remain open until a timeout which ties up the /dev/thingamabob

print("Done")