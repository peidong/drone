import socket
import sys
import time, datetime
import json
import yaml
import MySQLdb


#### Open port and start listening
HOST, PORT = '', 5000

listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
listen_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
listen_socket.bind((HOST, PORT))
listen_socket.listen(1)
print 'Serving HTTP on port %s ...' % PORT
####


#### Connect to database
db=MySQLdb.connect(host="localhost",user="ucla",db="ee202c_BT")
db.autocommit(True)
cur = db.cursor() 
####

#### When connection is accepted, start to receive data messages
client_connection, client_address = listen_socket.accept()
request = client_connection.recv(1024)

print request
####

#### Decode message to json format and assign to variables
ret = json.loads(request)
anchor_id = ret["anchor_id"]
client_id = ret["client_id"]
RSSI = ret["RSSI"]
# time_raw = ret["time"]
# time_str = time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(int(time_raw)))

####

#### Update database using SQL commands
cur.execute("CREATE TABLE IF NOT EXISTS ee202c_BT.RSSI (\
id int NOT NULL PRIMARY KEY AUTO_INCREMENT,\
anchor_id varchar(20),\
client_id varchar(20),\
RSSI 		varchar(20),\
update_time timestamp  DEFAULT '0000-00-00 00:00:00'\
) ENGINE = InnoDB;")

cur.execute("INSERT INTO RSSI (anchor_id, client_id, RSSI,\
	update_time) VALUES (%s, %s, %s, now());", (anchor_id, client_id, \
	RSSI))

####

#### Prepare messages to send back to client, send, close connection
http_response = """\
HTTP/1.1 200 OK

Hello, World!
"""
client_connection.sendall(http_response)
client_connection.close()
####

#### Used to exit command in for loop
# if request != "":
#     sys.exit(0)