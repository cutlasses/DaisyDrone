# DaisyDrone

## Author

Cutlasses

## Description

<a href="http://www.youtube.com/watch?feature=player_embedded&v=_0Ykc3huzv0
" target="_blank"><img src="http://img.youtube.com/vi/_0Ykc3huzv0/0.jpg" 
alt="DaisyDrone Video" width="240" height="180" border="10" /></a>

DaisyDrone is a drone synth, with 6 sliders, Inspired by @hainbach101 ‘s use of vintage oscillators and test equipment. This combines 18 sine waves. Each slider controls the gain of a set of 3 sine waves at slightly offset pitches centred around a frequency within the key shown in the display. The frequency of the sines increases from left to right. The magic comes from the way they are summed (controlled by the 3-way switch). Either averaged (boring!), or wavefolded with a sine wave or a triangle wave. The wave folding produces the rhythmic elements. An encoder is used to set the note of the base frequency, displayed in the 7 segment display.

Build Notes:

1. Components marked [NOT USED] can be left unpopulated. They are there for future protoyping (audio in, I2C interface, extra pot)
2. It is advisable to install the Daisy Seed with Female Header, so you can remove it and use it on other projects
3. Daisy Seed needs programming via JTAG, not via USB, as the USB pins are used.

BOM

 Reference | Part | Quantity | Link
--- | --- | --- | ---
Device_Audio_Electrosmith_Daisy_Seed_Rev4 | Daisy Seed | 1 | https://www.elevatorsound.com/product/electrosmith-daisy-seed-development-board/
R1-R9 | 1K 0805 resistor | 8 | TODO
R9-R10 | 4K7 0805 resistor [NOT USED] | 2 | TODO
POT_1 | [NOT USED] | 0 | -
POT_2 | Alpha pot | 1 | Thonk 
SLIDER_1-SLIDER_6 | Bourns_PTA4543_Single_Slide | 6 | TODO
SW1 | Rotary_Encoder:RotaryEncoder_Alps_EC11E-Switch_Vertical_H20mm | 1 | TODO
U1 | Display_Character:D168K | 1 | TODO
AUDIO_IN_L | MJ-3536 audio socket [NOT USED] | 1 | -
AUDIO_OUT_L | MJ-3536 audio socket | 1 | -
