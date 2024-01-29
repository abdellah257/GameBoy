# Game Boy C Emulator
- This Project Aims to simulate a GameBoy 
- I started with Typescript but I thought it maybe more interesting to do it in C :) 
- Current status: still working on it  
## Z80 Flags:

- Flag Register (F):

    - Zero: 0x80, set if the last operation produced a zeo result.
    - Substraction: 0x40, Set if the last operation was a substraction.
    - Half_carry: 0x20, Set if, in the result of the last operation, the lower half of the byte overflowed past 15.
    - Carry: 0x10, if the last op produced a result ; over 255 for addition or under 0 for substraction.

** **

## Ressources:

Some resources I'm using to build this emulator.

- <a href= "https://gbdev.io/pandocs/"> Pandocs
- <a href= ""> The Linoscope Machine Page
- <a href= "http://imrannazar.com/GameBoy-Emulation-in-JavaScript" > Imran Nazar's blog
