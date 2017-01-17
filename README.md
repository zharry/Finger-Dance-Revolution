# Finger-Dance-Revolution
Grade 11, Computer Engineering Summative. Two player DDR except played with your fingers!

### Included Libraries
- https://github.com/AndrewMascolo/CountUpDownTimer

### Commands Listing being sent from Arduino to Raspberry Pi
   /**
	 * Commands Separated by "+"
	 * 	Each Command is made up of
	 * 		- (L2/6, R5/9, T3/7, B4/8, SO, MN, DPA, FPA, HPA, NPA, CT, SC) Left, Right, Top, Bottom, Song Over
	 * 		- Followed by ":" 
	 * 		- [int button, int y] Separated by ","
	 * 			Button represents a set X coord
	 * 			y represents the current Y coord
	 * 			If it's SO
	 * 				- button represents Player 1 score
	 * 				- y represents Player 2 score
	 * 			If it's MN, DPA, FPA, HPA, NPA
	 * 				- button represents the Button (x coord)
	 * 				- y is not processed
	 * 			If it's CT
	 * 				- button represents the current time
	 * 				- y is not processed
     *      If it's SC
     *        - button represents Player 1 score
     *        - y represents Player 2 score
	 */
	 
### Quick Launch Raspberry Pi Command
export DISPLAY=:0 && cd /home/pi/Finger-Dance-Revolution && git pull && cp src/FDR_Display.java FDR_Display.java && javac FDR_Display.java && java FDR_Display
