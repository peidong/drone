#Smart Drone

###How to Install iOS app for free
With Xcode7, Apple Inc. allows developers to install iOS app for free. Here is the [tutorial](http://bouk.co/blog/sideload-iphone/).

###Intel Edison Board coding library mraa
When we want to code with Intel Edison Board using C or Python without using Arduino, we can use the library called [mraa](https://github.com/intel-iot-devkit/mraa).

###How to execute shell script when boot
[https://communities.intel.com/thread/60537](https://communities.intel.com/thread/60537)

###How to use git to manage the files
First, set up password

        cd ~/drone
        git config --global credential.helper 'cache --timeout 604800'
        git config credential.helper store
        git push

And next time, simply do this to git push
        
        cd ~/drone
        git pull
        git add -A :/
        git commit -m "comment notes"
        git push

or we can do this
        
        git pull && git add -A :/ && git commit -m "comment notes" && git push

###Add some environment variables
        cd /etc
        vi profile

And write this following line at the bottom of the file

        export LD_LIBRARY_PATH=/usr/local/lib

###Install libraries
#####[json-c library](https://github.com/json-c/json-c)

        git clone https://github.com/json-c/json-c.git
        cd json-c
        sh autogen.sh
        ./configure
        make
        make install
        make check

###Set timezone

        timedatectl set-timezone America/Los_Angeles

###Set your Edison Board to send ip address automatically

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
        ExecStart=/home/root/drone/Edison/main/ip_address/update_ip_address.sh
        Restart=always
        RestartSec=60s

        [Install]
        WantedBy=multi-user.target

Save the file by typing :wq

        systemctl enable update_ip_address
        reboot

To check your Edison board ip address, please go to this link [http://fryer.ee.ucla.edu/rest/api/ip_address/get/](http://fryer.ee.ucla.edu/rest/api/ip_address/get/)

###Install OpenCV

- 1. First we install with the extra source [http://docs.opencv.org/3.0-last-rst/doc/tutorials/introduction/linux_install/linux_install.html](http://docs.opencv.org/3.0-last-rst/doc/tutorials/introduction/linux_install/linux_install.html)

extra source []()

###Code Style
