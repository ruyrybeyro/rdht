# rdht   Jun 2016
Daemon for polling DHT11?/DHT21/DHT22/AM2301/AM2302/RHT03 Temperature and humidity sensors on Raspberry/Banana Pi.
Based on lol_dht22: https://github.com/technion/lol_dht22 
Requires wiringPi library. The readRHT03 function is not being used as the current function seems to do a better checking if the HT device stops transmitting abruptly (or we miss the transmitions).

see http://electronics.stackexchange.com/questions/236530/humidity-sensor-dht21-am2301-not-measuring-humidity-in-a-lamobo-r1-banana-pi-r/236751

change DHTPIN in dht.c for your WiringPi Pin. It defaults to 5.

change INTERVAL in dht.c if using other than rpimonitor with the default 10s delay polling.

The DHT21 chip can only be polled every 1s. Use the files created in /var/run/dht

rdht implements a very basic filter into which assumes the humidity/temperature changes cannot be abrupt to filter out the polling errors as the checksum is very basic.


./configure

make

sudo make install

sudo adduser dht

sudo chmod a+s /opt/bin/rdht

sudo su dht -c "/usr/local/bin/rdht" &

$ cat /var/run/dht/humidity 

487

$ cat /var/run/dht/temp

274

License: BSD

./configure options:
 --disable-silent-rules Show the make process (silent by default)
 --disable-secure Disable compile with FORTIY_SOURCE and stack-protector (enabled by default)
 --enable-development Compile with -g -O0 -Wall. Applies disable-secure automatically

# Requeriments to install WiringPi 

sudo apt-get install git-core

#How to install WiringPi in a Raspberry

git clone git://git.drogon.net/wiringPi

cd wiringPi

sudo ./build

#How to use WiringPi on the Orange Pi mini/Pi

Download the WiringOP on your Orange Pi mini/Pi. 

For Orange Pi mini:

git clone https://github.com/orangepi-xunlong/WiringOP -b orangepimini

For Orange Pi:

git clone https://github.com/orangepi-xunlong/WiringOP -b orangepi

After download, you need direct into the WiringOP directory and run:

chmod +x ./build

And then install the WiringOP：

sudo ./build

#How to install WiringPi for Banana/Lamobo 

For Banana Pro

    git clone https://github.com/LeMaker/WiringBP.git -b bananapro

For Banana Pi/Lamobo

    git clone https://github.com/LeMaker/WiringBP.git -b bananapi

Installation

    cd WiringBP

    chmod +x ./build

    sudo ./build


