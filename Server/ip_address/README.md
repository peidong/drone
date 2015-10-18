#ip address update part
1. To get your edison board ip address

        http://fryer.ee.ucla.edu/rest/api/ip_address/get/
2. Update the ip address

        curl --data "ip_address=192.168.0.1&network_name=test" "http://fryer.ee.ucla.edu/rest/api/ip_address/post/"
