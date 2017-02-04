# RPi3withOpenCV
Implementing the RoHAWKtics image processing on a RaspberryPi 3 using OpenCV in C/C++

--- Build process
1. cmake	- generate make files from cmake script
2. make		- generates 'Video' output file
3. sudo cp Video /usr/local/bin/frcImageProcessing
	- copy the app to a place that can run at boot
4. sudo cp startProcessing.sh /usr/local/bin/startProcessing.sh
	- copy the starting script to place that can run at boot

--- File locations
/usr/local/bin
	frcImageProcessing - the OpenCV app
	startProcessing.sh - shell script that sets camera params
				and starts the OpenCV app

/etc/init.d/
	frcImageProcessing - boot level startup script
	If changed, must execute:
		sudo update-rc.d frcImageProcessing defaults

Can start/stop the video by using:
/etc/init.d/frcImageProcessing start
/etc/init.d/frcImageProcessing stop

