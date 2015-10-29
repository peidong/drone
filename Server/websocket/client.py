# Echo client program
import socket
import time, datetime
import requests
import json
import yaml
import threading
#http://mysql-python.sourceforge.net/MySQLdb.html
#import MySQLdb

#### Connect to host, build socket connection
HOST = 'fryer.ee.ucla.edu'    # The remote host
PORT = 5000              # The same port as used by the server

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
####

####  User input variables to update to server
# Get current time
get_time = datetime.datetime.now()
time_int = int(time.mktime(get_time.timetuple()))

anchor_id = "0001"
client_id = "123456"
RSSI = -45
####

####  Wrap up messages into json format then convert to string, then send
response_dict = {"anchor_id":anchor_id,"client_id":client_id,"RSSI":str(RSSI),"time":str(time_int)}

response_str = json.dumps(response_dict)
s.sendall(response_str)
####

####  Receive data back from server side and display
data = s.recv(1024)
s.close()
print 'Received', repr(data)
####
