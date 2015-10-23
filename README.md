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

###Add some environment variables
        cd /etc
        vi profile

And write this following line at the bottom of the file

        export LD_LIBRARY_PATH=/usr/local/lib
