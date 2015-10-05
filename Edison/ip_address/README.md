		ifconfig wlan0 | grep "inet " | awk -F'[: ]+' '{ print $4 }'
