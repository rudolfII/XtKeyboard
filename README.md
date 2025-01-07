# XtKeyboard

This is about the simplest arduino program for an ATMEGA32U4 processor
that I used to connect my old XT keyboard to a computer using a USB
port. I used a 'Beetle' board for the job.

When I first searched for a similar program, I found some that were
either unnecesarily complicated and/or did not work reliably. So, in
the end, I wrote my own. Although one could use interrupts, I found
no good reason for it -- polling is just good enough, and is simpler.
(Some codes that I saw and that used interrupts had not been
carefully checked for race conditions; and I would bet there are 
related errors in them.) 

ATMEGA32U4 is nice (and used) because it has a hardware USB
controller inside.

Arduino (language) is used because there is the Keyboard library
available, which takes care (and hides complexity) of the USB-related
stuff.

