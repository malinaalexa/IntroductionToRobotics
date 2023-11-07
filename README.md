# Introduction to Robotics (2023 - 2024)
Welcome to Introduction to Robotics laboratory Repository. I'm Malina-Alexa Steica, a third-year student at the Faculty of Mathematics and Computer Science, University of Bucharest. Within this repo, you'll find my homework assignments, complete with detailed requirements, in-depth implementation, source code, and relevant images. Please feel welcome to delve into my robotics journey as a fellow student. Your feedback, inquiries and insights are greatly appreciated.

<pre>
   __,_,
  [_|_/ 
   //
 _//    __
(_|)   |@@|
 \ \__ \--/ __
  \o__|----|  |   __
      \ }{ /\ )_ / _\
      /\__/\ \__O (__
     (--/\--)    \__/
     _)(  )(_
    `---''---`
</pre>
<details>
<summary>
Homework 1
</summary>
<br>
In this homework, I installed the Arduino IDE, created a private GitHub repository named "IntroductionToRobotics", granted access to specified users, and provided an informative repository description.
</details>

<details>
<summary>
Homework 2
For this assignment, I tried controlling an RGB LED using three potentiometers with Arduino, in order to use each potentiometer to independently control the intensity of the Red, Green, and Blue channels of the RGB LED. </summary>

<br>
I used the following components: 
- one RGB LED
- three potentiometers
- three resistors 
- wires to set up the connections. Note that dark colors - blue and black - were used for minus and bright colors - red and yellow mostly - for plus.

I read the analog values from each potentiometer using Arduino and then mapped those values to control the respective LED channels.

I included the code used for the project in the homework2_RGB folder, and a picture of the setup below: 

![Setup Homework 1](https://github.com/malinaalexa/IntroductionToRobotics/assets/104028370/51d724ed-13fb-4ef1-9c93-123c39966926)

I have additionally added a link to a video showing that the setup is indeed working, which can be viewed at https://youtu.be/PtYLPSejM6A?si=s0xpZvtWt_mFTpGi.
</details>
<details>
<summary>Homework 3</summary>
<br>
For this homework, I've built a 3-floor elevator control system. There are three LEDs, each representing a floor, with the current floor's LED illuminated. An additional LED indicates the elevator's state, blinking during movement and remaining steady when the elevator is stationary. Three buttons simulate call buttons on the floors. When pressed, the elevator moves towards the corresponding floor after a short delay.
The buzzer produces distinct sounds when the elevator arrives at a floor and when the doors are closing or it's in motion.

If the elevator is already at the desired floor, pressing the button for that floor does nothing. Otherwise, it waits for the doors to close before moving. If it's already in motion, it stacks the decision until it reaches the first programmed floor, opens the doors, waits, and then closes them.

I have attached a picture:
![Elevator](https://github.com/malinaalexa/IntroductionToRobotics/assets/104028370/d0718fbf-8ed2-443b-80fd-00e023bb3c60)

And the link to a video showcasing functionality: https://youtu.be/atauKeMnFAM
</details>

<details>
<summary>
Homework 4</summary>
<br>
For this homework, I used a 7 segment display and a joystick, in order to "draw" on the display. The segment flickers when selected. When pressing the button shortly, the segment state remains on. When long pressing, it resets the display and it starts again from the dot segment.
</details>

I attached a picture of the setup:
![7 Segment](https://github.com/malinaalexa/IntroductionToRobotics/assets/104028370/48b36a5b-3087-4b5c-85c0-7bc86cc35414)

And a video for demonstrating functionality: https://youtu.be/QS-kTNX2JGU
