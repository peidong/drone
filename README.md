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

###Add opkg support
        vi /etc/opkg/base-feeds.conf

And write this following line at the bottom of the file

        src/gz all http://repo.opkg.net/edison/repo/all
        src/gz edison http://repo.opkg.net/edison/repo/edison
        src/gz core2-32 http://repo.opkg.net/edison/repo/core2-32

#Server Side Installation

###Mac OSX Installation

For the server, we need Apache, php, mysql.

There is a [post](http://jason.pureconcepts.net/2015/10/install-apache-php-mysql-mac-os-x-el-capitan/).

#####Enable Apache on Mac OS X

        sudo apachectl start

#####Edit Apache preference

        cd /etc/apache2/
        sudo cp httpd.conf httpd.conf.bak
        sudo vim httpd.conf

Uncomment the following line (remove #):

        LoadModule php5_module libexec/apache2/libphp5.so

Edit these lines

        User yangyang
        Group staff

        DocumentRoot "/Users/yangyang/www/"
        <Directory "/Users/yangyang/www/">
        Options Indexes FollowSymLinks Multiviews
        MultiviewsMatch Any
        AllowOverride All
        Require all granted
        </Directory>

        sudo apachectl restart

        cd /Users/yangyang/www
        touch phpinfo.php
		echo '<?php phpinfo();' > phpinfo.php

#####Install MySQL

        brew install mysql

#####Connect PHP and MySQL

        cd /var
        sudo mkdir mysql
        cd mysql
        sudo ln -s /tmp/mysql.sock mysql.sock

#####Setup mysql permission

        mysql -u root
        CREATE USER 'yangyang';

Then type in commands in mysql terminal

        create database edison;
        grant all on edison.* to 'yangyang';


#####Setup Edison Server

        cd ~/www
        mkdir rest
        cd ~/Github/drone
        ln -s `pwd`/Server ~/www/rest/api

