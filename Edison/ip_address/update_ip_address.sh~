#!/bin/bash
ip_address=`ifconfig wlan0 | grep "inet " | awk -F'[: ]+' '{ print $4 }'`
curl "http://fryer.ee.ucla.edu/rest/api/ip_address/update/?ip_address=$ip_address"
