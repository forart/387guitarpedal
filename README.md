## Guitar Effects Pedal

# Lee Mondini
This pedal can be programmed with different effects. Currently only the chorus effect functions. Uses Arduino for 10-bit ADC. Includes a preamp to boost the guitar signal high enough for processing. One knob is a potentiometer that adjusts the gain on the preamp. Two more potentiometers serve as knobs to tweak the signal in the Arduino according to the code. The fourth knob is a 6-way rotary selector for different effects. There is also a bypass switch to have the guitar signal skip the pedal and go straight to the amp. The pedal runs on a single 9V battery. Unplugging the guitar cables shuts off power to the device.

# Pedal Input/Output
Even though electric guitars put out a mono signal, I used two 1/4" female stereo jacks for the input and output of the pedal. The reason for this was to open the battery circuit unless both plugs were in. The mono tabs (where the tips of the connectors hit) are wired normally to the DPDT switch. The stereo and ground tabs are what open and close the circuit. The trick is that when mono connectors, as used by guitars, are inserted into stereo jacks, the ground barrel of the connector connects the stereo tab to the ground tab. In effect this leaves 2 ground tabs on each jack. I just wired the two together with one tab on each, then wired one to the negative battery terminal, and the other to ground.

# Bypass
I used a DPDT toggle switch to either bypass the guitar signal or process it with the Arduino. This is in place of a DPDT stomp switch until I can switch them. They are wired identically and work the same way. Until I switch them, I guess this isn't technically a pedal in the strict sense of the word.

# Preamp
The preamp uses a TL082 Op-amp in a non-inverting configuration. I designed it after a an on-board preamp I found online (a preamp that is installed on the guitar itself). Before adding the preamp, there was a only a barely audible guitar sound when I maxed out the volume on my amp. Originally, I used the uA741 op-amp, but I found it to be too noisy. It is still noisy, but much improved with the TL082. A potentiometer controls the gain. on-board preamp circuit

# Arduino input and output
A simple circuit normalizes the input from the preamp to 0-5V and takes it to the Arduino's analog 0. The output uses PWM and a mini-resistor ladder to take the signal from digital pins 3 and 11 to the output jack. I based these circuits off of a basic Arduino pedal design that was stereo for some reason. stereo Lo-Fi pedal input output

# Control knobs
Two potentiometers use analog inputs and AREF on the Arduino to control parameters in the code. A2 controls the "depth" of the chorus effect and is the most noticeable. Using different resistors, the code is intended to use the 6-way rotary switch like a discreet potentiometer to change the effect.I did not get the 6-way selector to work, most likely due to the different code it used. It did not work in the form I downloaded it from (from a less simple pedal design where I also got the circuit design for the 6-way selector). I updated it to the newer Arduino IDE and it complied, but did nothing. Then I added parts from the chorus code and got it to output a clean guitar signal. However, I have so far been unable to get any sort of effect. Pedal design with 6-way rotary switch

# Limitations/Future Work

My soldering skills or lack thereof - I had a horrible time trying to solder the jacks, and often when I plugged them in, my soldering popped off! I need to find a way to get the connections more permanently fixed. I think this should help clean up the signal as well. Also I need to redo the soldering on the power connector for the arduino.
Coding for the multiple effects selection - Once the selector works, I can then move on to programming my own effects.
Casing - I have a good sized metal box that my wallet came in that I would like to house the pedal. I just need a drill press or something to make the holes.
Stomp switch - I just got the DPDT stomp switch to replace the toggle in the mail. It looks like I need a small PCB to mount it though.
ADC?RAM? - If I decide to keep spiraling out, I might try to interface the Arduino with a ADC chip which would most likely sound much better than the Arduino's ADC. With a RAM chip, or some other type of memory, I could try to make a loop pedal or something. I got extra stomp switches so I could add more control.

# Code

Chorus effect code is slightly updated from one left in the Lo-fi pedal's comments
sketch_apr30a.ino is the chorus effect code
dsp.h, dsp.cpp, timers.h, and timers.cpp are all required files
Arduino_Guitar_Pedal.ino is incomplete code for six effect selection with rotary switch. It only outputs the clean signal.
(See "Source" section)

# See Also

Circuit Schematic Diagram (without non-functioning 6-way rotary selector)
Picture
Demo video
(See "Downloads" section)
