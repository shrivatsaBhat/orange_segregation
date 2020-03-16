# Orange Segregation
CV2 is used to identify colour of orange and arduino is used to actuate servo motors
## How to install CV2
To [install OpenCV Python module](https://linuxize.com/post/how-to-install-opencv-on-raspberry-pi/), run the following commands:

    	sudo apt update
    	sudo apt install python3-opencv
  The command above will install all packages necessary to run OpenCV.
  

    python3 -c "import cv2; print(cv2.__version__)"
 To verify the installation, import the `cv2` module and print the OpenCV version: 

> output: 
> 3.2.0
### Install OpenCV from Source
Start by increasing the swap space to avoid compilation hangups due to memory problems:

    sudo nano /etc/dphys-swapfile
Change the `CONF_SWAPSIZE` value from the default `100` to `1024`:
/etc/dphys-swapfile

```ini
CONF_SWAPSIZE=1024
```
Save the file and run the following command for changes to take effect:

    sudo /etc/init.d/dphys-swapfile restart
We can now start building OpenCV. First, update the packages index and install the build tools and required dependencies:

    sudo apt update
    sudo apt install build-essential cmake git pkg-config libgtk-3-dev "libcanberra-gtk*"
    sudo apt install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev
    sudo apt install libjpeg-dev libpng-dev libtiff-dev gfortran openexr libatlas-base-dev opencl-headers
    sudo apt install python3-dev python3-numpy libtbb2 libtbb-dev libdc1394-22-dev
Create the build directory

    mkdir ~/opencv_build && cd ~/opencv_build
    git clone https://github.com/opencv/opencv.git
    git clone https://github.com/opencv/opencv_contrib.git
  At the time of writing, the default version in the GitHub repositories is version 4.1.1. If you want to install an older version of OpenCV, navigate to both  `opencv`  and  `opencv_contrib`  directories and run  `git checkout <opencv-version>`.
  Once the repositories are cloned, create a temporary build directory, and [change](https://linuxize.com/post/linux-cd-command/) to it:

      mkdir -p ~/opencv_build/opencv/build && cd ~/opencv_build/opencv/build
 Set up the OpenCV build configuration with `cmake`:
``` ini
     cmake -D CMAKE_BUILD_TYPE=RELEASE \
        -D CMAKE_INSTALL_PREFIX=/usr/local \
        -D INSTALL_C_EXAMPLES=OFF \
        -D INSTALL_PYTHON_EXAMPLES=OFF \
        -D OPENCV_GENERATE_PKGCONFIG=ON \
        -D ENABLE_NEON=ON \
        -D OPENCV_EXTRA_EXE_LINKER_FLAGS=-latomic \
        -D ENABLE_VFPV3=ON \
        -D BUILD_TESTS=OFF \
        -D OPENCV_ENABLE_NONFREE=ON \
        -D OPENCV_EXTRA_MODULES_PATH=~/opencv_build/opencv_contrib/modules \
        -D BUILD_EXAMPLES=OFF ..
```
The output will look something like below:
``` output
...
-- Configuring done
-- Generating done
-- Build files have been written to: /home/pi/opencv_build/opencv/build
```
Run `make` to start the compilation process:
``` ini
make -j4
```
The process will take some time, around 1 - 2 hours, depending on the Raspberry Pi model. If the compilation fails at some point, due to resources not available, run the  `make`  command again and the process will continue from where it stopped.

When completed you will see something like below:
``` output
...
[100%] Linking CXX shared module ../../lib/python3/cv2.cpython-35m-arm-linux-gnueabihf.so
[100%] Built target opencv_python3
```
The final step is to install compiled OpenCV files:

    sudo make install
``` output
...
-- Installing: /usr/local/bin/opencv_version
-- Set runtime path of "/usr/local/bin/opencv_version" to "/usr/local/lib"
```
To check whether OpenCV has been installed successfully type the following commands and you should see the OpenCV version:
**C++ library:**
```
pkg-config --modversion opencv4
```
``` output
4.1.1
```
**Python library:**

```
python3 -c "import cv2; print(cv2.__version__)"
```

```output
4.1.1-pre
```

### Clean Up

If you don’t have much free space on your SD card,  [delete](https://linuxize.com/post/how-to-remove-files-and-directories-using-linux-command-line/)  the source files:

```
rm -rf ~/opencv_build
```
Heavy swap usage could damage your SD card. Change back the swap space to its original size:
```
sudo nano /etc/dphys-swapfile
```
Edit the  `CONF_SWAPSIZE`  value to  `100`:
   

					     /etc/dphys-swapfile
```ini
CONF_SWAPSIZE=100
```
Copy
Save the file and activate the changes:
```
sudo /etc/init.d/dphys-swapfile restart
```

#
## Prerequisites 
### Software and Librarys
 - Install [ARDUINO IDE](https://www.arduino.cc/en/main/software)
 - Install library [CircularBuffer](https://www.arduinolibraries.info/libraries/circular-buffer). *(The clear explanation is given at # [rlogiacco](https://github.com/rlogiacco)/**[CircularBuffer](https://github.com/rlogiacco/CircularBuffer)**)*
 - 
