#!/bin/bash
ip_address=`ifconfig wlan0 | grep "inet " | awk -F'[: ]+' '{ print $4 }'`
network_name=`iwgetid -r`
mac_address=`ifconfig wlan0 | grep -Eo '([[:xdigit:]]{1,2}[:-]){5}[[:xdigit:]]{1,2}' | head -n1`
curl --data "ip_address=$ip_address&mac_address=$mac_address&network_name=$network_name" "http://fryer.ee.ucla.edu/rest/api/ip_address/post/"
