#use this script to build the log4c library from source on every platform
# NOTE: that's for first time installation or if rebuilding EVERYTHING !
# NOTE 2: for now, in order for this srcipt to work, the log4c-1.2.4 should be in the
# 		  root directory of the project (the top directory [..../geofence/]

cd ../../log4c-1.2.4/
.configure --prefix=/home/pi/Desktop/geofence/log4c/
make
make install