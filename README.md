# nc2cnc
A Windows version CNC tool.

# hardware connection
[nc2cnc run on PC with serial port ] ----- [sCNC board] ----- [Step motor driver] ---- [Step Motor]

# sCNC protocol
host -> sCNC
--------------------------------------------------
[MMPPPPPP]
M,P -:   [0-9],[A-F]
[01000000]  - turn off driver
[02000000]  - turn on driver
[03000000]  - turn on motor
[04000000]  - turn off motor
[05000000]  - turn on fan
[06000000]  - turn off fan
[07NNNN00]  - x +N step
[08NNNN00]  - x -N step
[09NNNN00]  - y +N step
[0ANNNN00]  - y -N step
[0BNNNN00]  - z +N step
[0CNNNN00]   -  z -N step
[0DNNNNN]   -  x us/step
[0ENNNNN]   -  y us/step
[0FNNNNN]   -  z us/step
sCNC -> host
---------------------------------------------------
OK  -- !

# Contact me
aixi.wang@hotmail.com


