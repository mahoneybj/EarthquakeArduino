# Earhquake sensor setup

## Design
I wanted my project to be small and inexpencive this is because for my project to acually work I need hundreds of sensors placed across the country. I decided not to solder my circuit as there is still a lot of variables up in the air that could change and I didn't want to make my design 100% compelete. 

## Analysis of potential problem
Problems I ran into was the lack of real world testing I had done, this mean a lot of my tollorences etc are guess work. However I have coded this to be very modular so can be tweaked very easily. This project has a very large scope so I setteled to make a very small scale demo. To make a propper alert system I would need at least 3 sensors to be able to trianglate the eppicenter of the earthquake to then calculate the distance/time it will take to reach the user. My sensor can calculate the distacne the shaking came from using the diiffrence in time it took for the swave to arive after the pwave as they move at diffrent speeds. Then as a demo used that distance to calculate how long it will take for shaking to arive to the user (This is a demo as I would need 3 to get a real location and the sensor would have to be away from the alert device) 

## Hardware choices
- I decided to use an arduino mega for my sencing device as I needed a microcontroller that could read data very fast
- For my sensor I neeeded a sensor that could pick up slight tremmors and had very little noise. I found the ADXL355 sensor, it was made for sisemic imaging and had very low noise
- I used a raspberry pi attatched to the mega as I needed a way to transmit alerts and make quick calculatings (Pi reads the com port to recieve ADXL data). An arduino would be too slow

## Discoveries
I wanted my sensing device to be in a weather proof box to be able to be used in the open, however I soon relised that I would need to 3d print my own mount for the arduino, raspberrypi and the sensor. The sensor calbrates not long after powing on so is the sensor isnt secure and moves sligtly then it will be considered as an earthquake.


## Alert device breadboard diagram
![image](https://github.com/mahoneybj/EarthquakeArduino/assets/65274137/e9dfa076-27dd-4e06-aee8-1e4f29f2ce5f)
