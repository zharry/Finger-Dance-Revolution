# Finger-Dance-Revolution
Grade 11, Computer Engineering Summative. Two player DDR except played with your fingers!

### Included Libraries
- https://github.com/AndrewMascolo/CountUpDownTimer

### Commands Listing being sent from Arduino to Raspberry Pi
- Commands Separated by "+"
- 	Each Command is made up of
- 		- (L2/6, R5/9, T3/7, B4/8, SO, MN, DPA, FPA, HPA, NPA, CT, SC) Left, Right, Top, Bottom, Song Over
- 		- Followed by ":" 
- 		- [int button, int y] Separated by ","
- 			Button represents a set X coord
- 			y represents the current Y coord
- 			If it's SO
1. 				- button represents Player 1 score
2. 				- y represents Player 2 score
- 			If it's MN, DPA, FPA, HPA, NPA
1. 				- button represents the Button (x coord)
2. 				- y is not processed
- 			If it's CT
1. 				- button represents the current time
2. 				- y is not processed
-                      If it's SC
1.                              - button represents Player 1 score
2.                              - y represents Player 2 score
-                       If it's SS
1.                              - both btton and y are not processed
	 
### Quick Launch Raspberry Pi Command
screen /dev/ttyAMA0 115200

export DISPLAY=:0 && xset dpms force on && cd /home/pi/Finger-Dance-Revolution && git pull && cp src/FDR_Display.java FDR_Display.java && javac FDR_Display.java && java FDR_Display
