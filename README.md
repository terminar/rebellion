
# Rebellion alpha1 (usable until 2021-01-06)

**Date:** 2020-12-23

**Contact:** Björn Kalkbrenner <terminar@cyberphoria.org>

**Please don't spread this software yet, it's currently a non public test release!**

## General info

**Rebellion** is an ongoing project which lets you use **Native Instruments** devices
(such as a **Maschine MK3** or the **Komplete Kontrol MK2** keyboards) outside the
**Komplete Kontrol** or **Maschine** software. It is natively possible to access some 
of the functions via MIDI without any additional software but that's limited.

**Rebellion** tries to break these limitations by adopting a proprietary IPC protocol
which is used by Native Instruments itself when passing data between the 
Maschine and Komplete Kontrol software to NIHA/NIHIA (the Native Instruments 
Hardware Agent/Native Instruments Host Integration Agent "services").

Other existing solutions are using the limited MIDI API or stop NIHA/NIHIA 
to access the devices directly via USB but when NIHA/NIHIA are stopped, 
the Native Instruments software ecosystem can't be used anymore.
**Rebellion** masquerades itself as Maschine2 or Komplete Kontrol software 
instance by connecting to NIHA/NIHIA and takes over the device.

## Version info

This version just contains the cross platform core and implements some generic
tests, it can not be extended **yet**.

### Implementation

* core developed with C++
* available for Windows and macOS (via platform abstraction layer)
* contains Lua 5.4 as cross platform scripting engine

### Planned

* access to low level functions via library
* high level support via OSC connection
* maybe: high level support via MIDI messages
* integrated graphics API for devices with displays
* integrated state machine for handling specific use cases (e.g. pressing keys in order to have command layers, e.g. [SHIFT/GROUP_A], [LOCK] which is automatically remembered)

## Logging

Rebellion creates two logfiles in the application directory.

* print.log contains redirected "print" lines
* printdump.log contains noncaptured/unknown protocol events

Stdout will display everything (and even more) debug information.
For a test please start the application with redirected stdout into a file like

> ./rebellion(.exe) >> stdout.log
 
or (available on macOS)

> ./rebellion |tee -a stdout.log

## Privacy information

No sensible data is captured from the computer, it's just about the
communication to and from the Native Instruments devices. The logs may contain
the serial number of the devices because the communication is bound to the
serial numbers as some sort of unique identifiers.

The logfiles are human readable, in case of interest what the entries mean, just 
ask.

## Native Instrument devices

It should be possible to use **Rebellion** with the following devices when a 
release version is done:

* Maschine Jam
* Maschine Studio
* Maschine MK2
* Maschine MK1
* Maschine Mikro MK2
* Maschine Mikro MK1
* Maschine Plus
* Maschine MK3
* Maschine Mikro MK3
* Komplete Kontrol MK1
* Komplete Kontrol MK2
* Komplete Kontrol A
* Komplete Kontrol M

Also there are several other (or older) Native Instruments devices which should also  
work:

* AudioKontrol 1
* Rig Kontrol 2/3 (but i don't know if that makes sence)
* FX2
* KORE
* Kore 1
* Kore 2
* Traktor Kontrol D2
* Traktor Kontrol F1
* Traktor Kontrol S2 MK1
* Traktor Kontrol S2 MK2
* Traktor Kontrol S2 MK3
* Traktor Kontrol S3
* Traktor Kontrol S4 MK1
* Traktor Kontrol S4 MK2
* Traktor Kontrol S4 MK3
* Traktor Kontrol S5
* Traktor Kontrol X1 MK1
* Traktor Kontrol X1 MK2
* Traktor Kontrol Z1
* Traktor Kontrol Z2

Due to missing hardware that can't be tested yet. If anybody owns such devices 
please let me now, i'll try to create a special debug version of **Rebellion**.

Currently (for testing) only these devices are activated:

* Maschine MK3
* Komplete Kontrol S[number] MK2

## Limitations
Regarding the KKMK2 - Currently the "led count" is fixed to the length
of the S61. When using the KNOB1 e.g. on the S49 it will not stop at the 
end (last key) of the piano light stripe and just fade away - just rotate 
counter-clockwise to get back the led. On the S88 it should stop somewhere
(12 keys?) before the end of the piano light stripe.

## Keys

* DBTN[number] Buttons above the displays
* KNOB[number] Knobs below the displays


| Control | Description  |
|----------|--------------|
| DBTN1  | Load skull image into the left display (takes around 6+ seconds the first time) |
| DBTN2  | Select left display to draw on |
| DBTN4  | Clear left display |
| DBTN5  | Load skull image into the right display (takes around 6+ seconds the first time), additionally draw a small black line onto the image |
| DBTN6  | Select right display to draw on  |
| DBTN8  | Clear right display |
| FIXED_VEL | Toggle all leds (white)  |
| STOP  | Exit the application |
| KNOB1  | Move around the led left/right (backwards/forwards) around the device |
| KNOB2  | Select color for led, (15 colors available on Maschine MK3)  |
| KNOB3  | Select brightness for led (4 steps available on the Maschine MK3)  |
| KNOB4  | Select color (RED: 0-255) for sketch point  |
| KNOB5  | Select color (GREEN: 0-255) for sketch point  |
| KNOB6  | Select color (BLUE: 0-255) for sketch point  |
| KNOB7  | Sketch point/line on the selected display (X-Axis). clockwise = right, counter-clockwise = left  |
| KNOB8  | Sketch point/line on the selected display (Y-Axis). clockwise = down, counter-clockwise = up  |
