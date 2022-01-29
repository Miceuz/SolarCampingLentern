# Open source Solar Camping Lantern

## Background

I started seeing a lot of disposable electronic cigarettes being used. As everyone knows, those devices contain perfectly rechargeable LiPo battery. And I've seen them thrown away everywhere - on streets, in general waste thrashcans, etc. While a lot of my friends have successfully refilled juice and re-charged the battery, the devices are not built to last - heating coil dies after several re-uses anyway. I have decided to look for a way to recycle those batteries. 

So I have decided to make a lantern for my camping needs and also investigate a product parameter space. The idea popped in to my mind during a summer music festival, actually. When you camp with your friends and need a bit of extra light in the camp at night. It was a sunny summer, so of course I had to have it solar powered. Provided unlimited supply of batteries, can it be self sufficient in it's power needs just from solar energy while still being manageable in camp environment?

It's always a good idea to check what's available at the market. Maybe there is a product on the market that does just that? Just a piece of my expenses, not worth spending my time?

## Initial requirements

 * Light output: ~100lm
 * Run time: min 4h
 * Battery: LiPo 3.7V ~2000mAh
 * Charging time: 6h

I have gauged the amount of needed light from my lumilight pocket torch I have lost and my "cyclope" torch from Decathlon I got this year. 100lm is not a huge amount of light but still not that bad - it's enough to see people and stuff around, human eye is logarithmic and adapts to low light conditions .

Run time is a modest shot from a festival use case scenario - you might need light from 11 at night to 3 in the morning roughly.

Charging time is a lazy day camping, maybe I will remember to pull it out and put on a sun sometime. It's not full effective sun time, just general time the device spends on sun semi-kinda-charging.

## Case studies

### [BioLite SunLight 100](https://www.bioliteenergy.com/products/sunlight-100?variant=1142597386254)

  * Light output: 100lm max
  * Battery: 750mAh LiPo
  * Run time: 3h
  * Solar pannel: 0.45W
  * Charging time: 7h
  * Dimensions: 85x86x23 mm
  * Weight: 95g
  * Price: 20$

### [MPOWERD Luci Outdoor 2.0](https://mpowerd.com/products/luci-outdoor-2-0-f2017)

 * Light output: 75lm
 * Battery: 1000mAh
 * Run time: 6h
 * Charging time: 7h
 * Dimensions: 127mm dia, 25mm height
 * Weight: 125g
 * Price: 24$

### [MPowerd Luci Solar String Lights](https://mpowerd.com/products/string-lights)

 * Light output: 100lm
 * Battery: 2000mAh
 * Charging time: 16h
 * Run time: 5h
 * Dimensions: 127mm dia, 50mm height
 * Weight: 320g
 * Price: 50$

There are devices that target the same light output I want. Some of these specifications are inferred as not all manufacturers provide all the information I needed. SunLight 100 does almost exactly what I want, but solar battery this size barely will produce 8.3Wh needed in 7h of charging time. All in all solar in all of these products seems to be a nice addition to USB charging, not the main power provider. 

Also I have bought some not-so-cheap crap from local hardware store that gimmicked like it can do what I want, but solar battery was total crap in those, outputting mere single digit milli-amps.

Thus, I have easily convinced myself that this is well worth as a hobby project. Besides, had to use those batteries up! So, with the battery sourcing question sorted out, let's not think about price too much and look what happens if we push on the light output efficiency and solar input efficiency.

## Design

### MPPT options

 * SPV1040T
 * ADP5090ACPZ-2-R7
 * BQ24650
 * [MPPT Solar Panel Controller with DC Converter](https://www.tindie.com/products/electronicstore/mppt-solar-panel-controller-with-dc-converter-2)

In the times of supply chain collapse, choosing a MMPT capable converter chip for storing solar energy to the battery was not an easy thing. They all are either not available or expensive. Luckily I have stumbled on this board by [PC Tech](https://www.tindie.com/stores/electronicstore/) featuring CN3791. Basically this project could have ended just there, as this board was all I needed - I have made my first prototype using it, but I had to extract a bit more efficiency and add some computer controll to the thing. 

### Light Source

 * L130-4070003000W21 - Vf = 6V, I = 100mA, Light output = 100lm

It's one of the most efficient LEDs available at electronic distributors at the time. What's interesting and handy is that it's practically voltage controlled - it has a nice shallow IV curve. Provided you extract the heat from them though. They will happily do 100lm at 0.6W, the efficiency is even higher if you run them at half the rating. That's why I have doubled them up in my design.

Also I have decided to throw in a red light source, to see if it might be usable too. Red light does not dazzle humans too much, so it makes a good low level stanbly light source. I did not want to add additional complication of channel switching for led driver chip so decided to power red led directly from MCU pin.  

### Photovoltaics

 * [Solar pannel](https://www.evita.lt/u-9045-saules-baterijos-modulis-6v-3-5w-165x135mm?search=saul%C4%97s) 6V, 3.5W

These pannels available at local suppliers looked like a good tradeoff between size and output power. Besides, not a lot to choose from, really. I have measured them and they were outputing the rated power, but it dropped drastically with non-perfect orientation to the sun, thus it has to be derated significantly. I have guesstimated MPPT point of it to be somewhere at 80% of the pannel's open circuit voltage and measured the same with a potentiometer hooked up to it.

### Power

 * Battery: 3.7 LiPo, capacity varies: 380mAh, 500mAh total capacity up to 2Ah
 * MCU powered directly from battery
 * DC-DC led driver - MCP1664

Batteries are re-used from electronic cigarettes. I don't really know if they retain their safety ratings after they have been re-charged, but people around are re-chargint them and I've heard of no fires happening. 

They come with a protection board and I have tried shorting and over-discharging them - seem to work fine, also I've seen my friends ridiciously abusing them with high charge currents on par of 3C. Their longevity is to be proven though as they might not be optimized to a lot of re-charge cycles, but it seems their safety is on par with standards mandatory with CE marking. I am happy to delay their travel to a landfill or a recycler. **If you replicate this design, be warned that battery failure is a risk to be considered!** 

### Operational parameters

 * Energy consumed: 0.1A * 6V * 4h = 2.4Wh + losses
 * Solar power available: 3.5W minus conversion losses
 * Light output: 120lm

It has plenty of solar - 1h of perfect sunlight should provide enough of charge for 4 required hours. Seems to make sense to have more battery capacity, but there is a small tradeoff with the weight. It will be interesting to experiment with battery sizes and see what run times are achievable.

### Mechanical construction

I've chosen 4 holes to attach the lantern with rope - to a branch or a tarp. Also alternatively it can be used as a coaster for a water bottle. This makes a nice dispersed table light.

Batteries are glued to the PCB. PCB is glued and soldered to the solar panel. Both the solar panel and plexiglass are glued to a 3D printed plastic frame.

Of course it should have some level of ingress protection. Provided all glue seams are tight, the only place for water to seep in is thru a button hole. I am going to experiment with printing of the frame from flexible plastics and have the button hole closed, but for the first revision there's going to be a small seam between hole and plunger and IPX6 rated button behind it.

### MCU and software

I have chosen my beloved attiny441 - I have a stock of them and I am used to them and it's a relief to spend some time in a simple naive world of 8-bit microcontrollers. It's pretty low power too.

Software functions:

 * On/Off
 * Different levels of lighting
 * Cycle through white and red leds
 * Indicate battery level via charlieplexed leds
 * Auto level reduction on low battery [todo]
 * Fade everywhere [todo]
 * Sky's the limit, what can you do with a single button interface?

### Bells and whistles

 * Charlieplexed leds for battery level indication

### Weight analysis

| Item | Weight, g |
|------|-----------|
| 3.5W solar pannel | 96 |
| Plexiglass | 73 |
| Frame | 56 |
| PCB 0.8mm | 36 |
| Battery 380mAh 7g, 3pack | 22 |
| Electronic components | 4 |
| Glue | 10 |
| **Total** | **297** |

## Price analysis

| Item | Price, € |
|------|----------|
| Solar pannel | 13 |
| Electronic components | 6.3 |
| PCB | 5.1 |
| Frame | 2 |
| Plexiglass | 0.6 |
| Batteries | 0 |
| **Total** | **27** | 

Prices are for a small batch of 10 units.

## Final specifications

 * Light output: 120lm
 * Battery: LiPo 3.7V up to 4000mAh 
 * Run time: >4h
 * Solar pannel: 3.5W
 * Charging time for 4h run time: 1h (perfect sun conditions)
 * Dimensions: 193x163x15 mm
 * Weight: 297g
 * Price: 27€

## Final thoughts

I already have several plans for improvement - button is in a weird position, hard to find in the dark. I should put it in the midle of a edge. Also battery level indication leds are a fail - I should have spaced them out along the edge so they would be distinguishable behind the plexiglass.

Weight is a concern, but I am limited here to materials I have access to. I have made some improvements by using a thin PCB, maybe I can make the frame lighter, but the real limiting factors are solar pannel and plexiglass.

I have tried using the first prototype while camping at sunny autumn and it proved to be working well. Working time with 870mAh of battery was a little too short for long autumn evenings, but it was able to fully charge during the day. Later I brought the pannel to a rainy camping trip in December. Pannel was barely charging the 1.5Ah battery, but it held almost two evenings allowing us to set up camp, prepare wood and chill under the red light in addition to fire. I think it's not much useful for rainy winter in Easter Europe, but it will be a perfect light for summer camping.

