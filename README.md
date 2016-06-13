# rdht   Jun 2016
Daemon for polling DHT21/DHT22/AM2301/AM2302/RHT03 Temperature and humidity sensors on Raspberry/Banana Pi.
Based on lol_dht22: https://github.com/technion/lol_dht22 
Requires wiringPi library. The readRHT03 function is not being used as the current one seems to do a better checking if the HT device stops transmitting abruptly (or we miss the transmitions).

see http://electronics.stackexchange.com/questions/236530/humidity-sensor-dht21-am2301-not-measuring-humidity-in-a-lamobo-r1-banana-pi-r/236751

change DHTPIN in dht.c for your WiringPi Pin. It defaults to 5.

change INTERVAL in dht.c if using other than rpimonitor with the default 10s delay polling.

The DHT21 chip can only be polled every 1s. Use the files created in /var/run/dht

rdht implements a very basic filter into which assumes the humidity/temperature changes cannot be abrupt to filter out the polling errors as the checksum is very basic.


./configure

make

adduser dht

su dht -c "/opt/bin/rdht" &

$ cat /var/run/dht/humidity 

487

$ cat /var/run/dht/temp

274

License: BSD

./configure options:
 --disable-silent-rules Show the make process (silent by default)
 --disable-secure Disable compile with FORTIY_SOURCE and stack-protector (enabled by default)
 --enable-development Compile with -g -O0 -Wall. Applies disable-secure automatically

