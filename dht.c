/*
 *      dht.c:
 *	Simple test program to test the wiringPi functions
 *	Based on the existing dht22.c that was based in dht11.c
 *	Amended by technion@lolware.net
 */

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/stat.h>

/* WiringPi  pin */
#define DHTPIN 5

/* waiting time */
#define INTERVAL 8 /* with rpimonitor default, better be 8 */

#define DATADIR "/var/run/dht"

#define MAXTIMINGS 85
static int dht22_dat[5] = {0,0,0,0,0};

static uint8_t sizecvt(const int read)
{
  /* digitalRead() and friends from wiringpi are defined as returning a value
  < 256. However, they are returned as int() types. This is a safety function */

  if (read > 255 || read < 0)
  {
    printf("Invalid data from wiringPi library\n");
    exit(EXIT_FAILURE);
  }
  return (uint8_t)read;
}

static int readDHT(int pin, int *temp, int *rh)
{
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;

  dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

  // pull pin down for 18 milliseconds
  pinMode(pin, OUTPUT);
  // digitalWrite(pin, HIGH); 
  // delay(10);		   
  digitalWrite(pin, LOW);  
  delay(18);
  // then pull it up for 40 microseconds
  digitalWrite(pin, HIGH);
  delayMicroseconds(40); 
  // prepare to read the pin
  pinMode(pin, INPUT);

  // detect change and read data
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (sizecvt(digitalRead(pin)) == laststate) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break;
      }
    }
    laststate = sizecvt(digitalRead(pin));

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      dht22_dat[j/8] <<= 1;
      if (counter > 16)
        dht22_dat[j/8] |= 1;
      j++;
    }
  }

  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
  // print it out if data is good
  if ((j >= 40) && 
      (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {

        *rh = dht22_dat[0] * 256 + dht22_dat[1];
        *temp = (dht22_dat[2] & 0x7F)* 256 + dht22_dat[3];

        /* negative temp */
        if ((dht22_dat[2] & 0x80) != 0)  *temp = -*temp;

    return 1;
  }
  else
  {
    //printf("Data not good, skip\n");
    return 0;
  }
}

int main (int argc, char *argv[])
{
//  int tries = 10;
  FILE * f;
  int newTemp, newRh;

  if (wiringPiSetup () == -1)
    exit(EXIT_FAILURE) ;

  mkdir(DATADIR, 0755 );
  if ( chown(DATADIR, getuid(), -1) == -1 )
  {
     fprintf(stderr, "Was not able to create /var/run/dht\n");
     exit(1);
  }
  if (setuid(getuid()) < 0)
  {
    perror("Dropping privileges failed\n");
    exit(EXIT_FAILURE);
  }
  //chdir(DATADIR);

  f = fopen("/var/run/dht/pid", "w" );
  fprintf(f, "%d", getpid() );
  fclose(f);  
  while(1)
  {
//  while (read_dht22_dat() == 0 && tries--) 
//  {
//     delay(1000); // wait 1sec to refresh
//  }
//   if (readRHT03 (DHTPIN, &newTemp, &newRh))
     if ( readDHT(DHTPIN, &newTemp, &newRh) )
     {
        static int t2, h2, ft = 1, terror=0, herror=0;

        if (ft || (terror>4) || (herror>4) )
        {  
           ft=0; 
           h2=newRh; t2=newTemp;
           terror=herror=0;
        }

        if ( (newRh > 0) && (newRh < 950)  && (abs(h2-newRh)<10) )
        {
           f=fopen("/var/run/dht/humidity.x", "w");
           fprintf(f, "%d", newRh );
           fclose(f);
           rename("/var/run/dht/humidity.x", "/var/run/dht/humidity");
           h2 = newRh;
           herror=0;
         }
         else 
           herror++;
         if ( ( newTemp < 600 ) && (abs(abs(t2)-abs(newTemp))<10) )
         {
            f=fopen("/var/run/dht/temp.x", "w");
            fprintf(f, "%d", newTemp );
            fclose(f);
            rename("/var/run/dht/temp.x", "/var/run/dht/temp");
            t2 = newTemp;
            terror=0;
          }
          else 
            terror++;

     }
     sleep(INTERVAL);
  }

  return 0 ;
}
