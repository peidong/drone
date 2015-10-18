#This is for iOS app design

We now have two ios application. One is for the phone carried by the drone, called Drone_onair. The other is for the phone to control the drone, called Drone_control.

Until now:

1.Drone_onair:

  Function: GPS information read, post to server; Get information from website.
  
  trick: In ios 8 or later, when using location, you need to do two extra things to get location working: Add a key to your Info.plist and request authorization from the location manager asking it to start. 
  
  reference:http://nevan.net/2014/09/core-location-manager-changes-in-ios-8/


2.Drone_control:

  Function: Using slider to control the speed of each motor. Now we can change the value of sliders(including button maximum, minimum and medium), and send these value to websits. Edison, using C code, can get these information from web. According to these values, it can then change the speed of motor. So that we can control the fly of drone by iphone.
