# DaisyDrone

## Author

Cutlasses

## Description

<a href="http://www.youtube.com/watch?feature=player_embedded&v=_0Ykc3huzv0
" target="_blank"><img src="http://img.youtube.com/vi/_0Ykc3huzv0/0.jpg" 
alt="DaisyDrone Video" width="240" height="180" border="10" /></a>

Click image, or https://youtu.be/_0Ykc3huzv0 to watch demo.

DaisyDrone is a drone synth, with 6 sliders, Inspired by @hainbach101 ‘s use of vintage oscillators and test equipment. This combines 18 sine waves. Each slider controls the gain of a set of 3 sine waves at slightly offset pitches centred around a frequency within the key shown in the display. The frequency of the sines increases from left to right. The magic comes from the way they are summed (controlled by the 3-way switch). Either averaged (boring!), or wavefolded with a sine wave or a triangle wave. The wave folding produces the rhythmic elements. An encoder is used to set the note of the base frequency, displayed in the 7 segment display.

Build Notes:

1. You can either order PCBs from the Gerbers in the Hardware folder, or use the Schematic to lay out a design on strip board (vero board)
2. The PCB uses SMD components, you'll need a pair of tweezers and a bit of practise to do these! Once you've got the knack it's quicker and easier than through-hole in my opinion.
3. On the PCB the pots and encoders aren't marked. The encoder is the one nearest the output jacks. To the left of that is the pot that is setup to control master volume. The pot to the left of that is currently unassigned.
4. Components marked [NOT USED] can be left unpopulated. They are there for future protoyping (audio in, I2C interface, extra pot)
5. It is advisable to install the Daisy Seed with Female Header, so you can remove it and use it on other projects
6. Due to the USB pins being used on the DaisyDrone PCB, you either need to program via USB with the Daisy *unattached* to the PCB, or the Daisy Seed needs programming via JTAG on the board. https://www.electro-smith.com/daisy/stlink-v3mini
7. You will need to install the Daisy C++ toolchain to compile the code, here is a video walkthrough https://www.youtube.com/watch?v=e4KaBs6qSkU
8. You need to change LIBDAISY_DIR and DAISYSP_DIR to point to where you installed DaisyExamples
9. Most values of potentiometer (pot) should be fine, I use 10K, but if you have some 20Ks, 50Ks etc, you can use those.
10. I use M3 stand-offs with M3 bolts to raise the PCB above the desk

BOM

 Reference | Part | Quantity | Link
--- | --- | --- | ---
Device_Audio_Electrosmith_Daisy_Seed_Rev4 | Daisy Seed | 1 | https://www.elevatorsound.com/product/electrosmith-daisy-seed-development-board/
R1-R8 | 1K 0805 resistor | 8 | https://octopart.com/erj-p06j102v-panasonic-39823360?r=sp
R9-R10 | 4K7 0805 resistor [NOT USED] | 0 | https://octopart.com/crgcq0805j4k7-te+connectivity-91018232?r=sp
GND_LNK | 0R 0805 resistor | 0 | (Can also use a small piece of wire of solder)
POT_1 | 10K Alpha pot [NOT USED] | 0 |  https://www.thonk.co.uk/shop/alpha-9mm-pots/
POT_2 | 10K Alpha pot | 1 | https://www.thonk.co.uk/shop/alpha-9mm-pots/
SLIDER_1-SLIDER_6 | Bourns_PTA4543_Single_Slide | 6 | https://octopart.com/pta4543-2015dpb103-bourns-10485577?r=sp
SW1 | Rotary_Encoder:RotaryEncoder_Alps_EC11E-Switch_Vertical_H20mm | 1 | https://octopart.com/pec11r-4015f-n0024-bourns-26648251?r=sp
S1 | OS103012MU2QP1 | 1 | https://octopart.com/os103012mu2qp1-c%26k+components-2486960?r=sp
U1 | 7 segment Cathode Display | 1 | https://octopart.com/157142v12703-wurth+elektronik-119894004
AUDIO_IN_L | MJ-3536 audio socket [NOT USED] | 0 | https://octopart.com/mj-3536-cui+devices-106233900?r=sp
AUDIO_OUT_L | MJ-3536 audio socket | 1 | https://octopart.com/mj-3536-cui+devices-106233900?r=sp

![Daisy Drone Top Down](https://github.com/cutlasses/DaisyDrone/blob/main/Hardware/IMG_5485.jpeg "Daisy Drone Top Down")

![Daisy Drone Left](https://github.com/cutlasses/DaisyDrone/blob/main/Hardware/IMG_5486.jpeg "Daisy Drone Left")

![Daisy Drone Right](https://github.com/cutlasses/DaisyDrone/blob/main/Hardware/IMG_5487.jpeg "Daisy Drone Right")


