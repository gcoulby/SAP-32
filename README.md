# Ben Eater's ~~SAP-1~~ SAP-32

This is an ESP32-based emulation of Ben Eater's SAP-1 project written in C++. This project includes a `.ino` file and is intended to be loaded onto the ESP32 via the Arduino IDE.

This project has a set of complementary applications to go with it for debugging. This project can be run on its own with out the debug tools and it is recommended to comment out the following line in the `SAP-1.ino`

`#define DEBUG 1`

To view the companion app - https://github.com/gcoulby/sap-1-com-reader

# What is this project?

This project is a emulation of Ben Eater's SAP-1, 8-bit computer.

I strongly encourage you look at this project as it is a fascinating series - that is also FREE!

https://www.youtube.com/playlist?list=PLowKtXNTBypGqImE405J2565dvjafglHU

There is only one issue with this series (especially during a global chip shortage) - COST!. The chips are getting hard to source and expensive to obtain. Therefore, I wanted to build a version of the computer that uses some of the many components I have lying around.

Therefore I am currently making this with minimal components: a few switches, resistors and LEDs. I therefore, created the companions services and app to display the REG_A/REG_B/ALU/MAR/IR/OUT/BUS etc as LEDs in the browser.

This works because this emulator transmits JSON debug messages, which can be presented in the browser as states that change the values of virtual components.

# What is the goal

The goal here is to learn the fundementals of the breadboard computer without the expense. Therefore, I am not basing this on someone elses work. I am not following a tutorial on how to build the emulator in C++. I am watching Ben's course material, identifying how it works then solving the problem to acheive the same functionality in C++.

My goal is to build this as a minimal PCB-based machine with PDP10/Altair/IMSAI type controls.

I also hope to increase the RAM to 65535 bytes, but this will done as a fork of this project, once I have completed it.

# Can I use the project?

You are welcome to use this project however you see fit, but I encourage you to follow my steps. Watch Ben Eater's fanscinating and detailed tuition on the workings of each module and build the project yourself from scratch. The whole purpose here is to learn.

I will however, keep this updated with progressions and make the schematics of the final design avaialble, when I get there!
