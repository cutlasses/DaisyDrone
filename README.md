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
6. Daisy Seed needs programming via JTAG, not via USB, as the USB pins are used.
7. Most values of potentiometer (pot) should be fine, I use 10K, but if you have some 20Ks, 50Ks etc, you can use those.

BOM

 Reference | Part | Quantity | Link
--- | --- | --- | ---
Device_Audio_Electrosmith_Daisy_Seed_Rev4 | Daisy Seed | 1 | https://www.elevatorsound.com/product/electrosmith-daisy-seed-development-board/
R1-R9 | 1K 0805 resistor | 8 | TODO
R9-R10 | 4K7 0805 resistor [NOT USED] | 2 | TODO
POT_1 | [NOT USED] | 0 |  https://www.thonk.co.uk/shop/alpha-9mm-pots/
POT_2 | 10K Alpha pot | 1 | https://www.thonk.co.uk/shop/alpha-9mm-pots/
SLIDER_1-SLIDER_6 | Bourns_PTA4543_Single_Slide | 6 | https://octopart.com/pta4543-2015dpb103-bourns-10485577?r=sp
SW1 | Rotary_Encoder:RotaryEncoder_Alps_EC11E-Switch_Vertical_H20mm | 1 | [TODO](https://octopart.com/pec11r-4015f-n0024-bourns-26648251?r=sp)
S1 | OS103012MU2QP1 | 1 | https://octopart.com/os103012mu2qp1-c%26k+components-2486960?r=sp
U1 | Display_Character:D168K | 1 | 7 segment Cathode Display
AUDIO_IN_L | MJ-3536 audio socket [NOT USED] | 1 | https://octopart.com/mj-3536-cui+devices-106233900?r=sp
AUDIO_OUT_L | MJ-3536 audio socket | 1 | https://octopart.com/mj-3536-cui+devices-106233900?r=sp
