# MakoPad
This is a 6 key macropad that I designed in KiCad and is programmed with QMK. It features a 1.8" LCD display, 6 hot-swappable keys, a rotary encoder, and 2 buttons to switch the layers. This Project has been made specially for programmers on FRC 5199 Robot Dolphins From Outer Space to help them with FRC Programming. 

PCB and Schematic
-----------------------------------------------------
<img width="2223" height="1600" alt="image" src="https://github.com/user-attachments/assets/664de750-03a9-413b-8121-444d038ea296" />
<img width="3290" height="1649" alt="image" src="https://github.com/user-attachments/assets/26af7d6d-4ce1-476b-a800-9822942498cc" />

Below is A picture of the assembly I made in Fusion 360.
------------------------------------------------------------------------------------
PCB Assembly [mind the crappy display model, i couldn't find a better one since there wasn't any for the specific one i wanted to use]
<img width="2282" height="1468" alt="image" src="https://github.com/user-attachments/assets/a0eaaf5a-b31b-4a60-b94b-13b90e0ab802" />

Here is the plan for developing the programming
--------------------------------------------------
Phase 1: Keyboard matrix
- Program the keyboard matrix and build the layers
- Test matrix to make sure it works

Phase 2: Rotary encoder
- Add the rotary encoder and rotary encoder switch into the programming
- Program the special functions for the rotary encoder
  - Make it so that whenever the rotary encoder is pressed down, you can rotate the knob to select what function you want to rotary encoder to do
- Test rotation and clicking

Phase 3: Layer buttons
- Program the button to change the keyboard matrix layers
  - The top button switches to the next layer and the bottom one switches to the previous layer

Phase 4: The display
- Make the display interface with the microcontroller

Phase 5: Make it awesome
- Layer-dependent key labels
- Encoder modes
- Custom macros
- Startup screen
- MakoPad logo
- Current layer display
- Key function display
