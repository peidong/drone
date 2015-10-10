#Set your Edison Board to send ip address automatically
git clone the drone into your home folder

        cd ~
        git clone https://github.com/peidong/drone.git

Then modify the system start service

        cd /lib/systemd/system
        touch update_ip_address.service
        vi update_ip_address.service

Copy and paste these into your file

    [Unit]
    Description=update ip address
    After=network.target
     
    [Service]
    #User=root
    ExecStart=/home/root/drone/Edison/ip_address/update_ip_address.sh
    Restart=always
    RestartSec=1800s
     
    [Install]
    WantedBy=multi-user.target

Save the file by typing :wq

        systemctl enable update_ip_address
        reboot

###To check your Edison board ip address, please go to this link [http://fryer.ee.ucla.edu/rest/api/ip_address/get/](http://fryer.ee.ucla.edu/rest/api/ip_address/get/)
