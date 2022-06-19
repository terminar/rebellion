
# Rebellion alpha3

**Date:** 2022-06-20

**Contact:** Björn Kalkbrenner <terminar@cyberphoria.org>

**Discord:** https://discord.gg/NFdCmjcp4e

**FAQ:** FAQ.md

**Please keep in mind, that this is the second alpha version. 
It will be instable and slow as hell!**

## General info

**Rebellion** is an ongoing project which lets you use **Native Instruments** 
devices (such as a **Maschine MK3** or the **Komplete Kontrol MK2** keyboards) 
outside the **Komplete Kontrol** or **Maschine** software. Of course it is 
currently possible without **Rebellion** to access some of the functions via 
MIDI without any additional software but that's limited.

**Rebellion** tries to break these limitations by adopting a proprietary IPC 
protocol which is used by Native Instruments itself when passing data between 
the Maschine and Komplete Kontrol software to NIHA/NIHIA (the Native Instruments 
Hardware Agent/Native Instruments Host Integration Agent "services").

Other existing solutions are using the MIDI API or stop NIHA/NIHIA 
to access the devices directly via USB - but when NIHA/NIHIA are stopped, 
the Native Instruments software ecosystem can't be used anymore.

**Rebellion** tries to masquerades itself as Maschine2 or Komplete Kontrol 
software instance by connecting to NIHA/NIHIA and takes over the supported 
device(s).
### Example videos
* [Rebellion (alpha1) controlling the Native Instruments Komplete Kontrol S61 MK2 - switching instances](https://youtu.be/fo09cgzsFbI)
* [Rebellion (alpha1) controlling the Native Instruments Maschine MK3](https://youtu.be/IgfNfhB3vrk)

## License

LGPLv3 with some closed-source scripts but for a more detailed info see FAQ.md.

### Implementation

* available for Windows and MacOS (via platform abstraction layer)
* core library developed with C++ (src/librebellion)
* core contains Lua 5.4 as cross platform scripting engine for user defined functions and tests/prototyping
* Go based daemon as higher level gateway (src/rebelliond) (OSC, MIDI?)
* C++ based program (src/host) which can be used to display the events and play around a bit the the current user defined test functions (load image, toggle LEDs, ...)

### Planned

* access to low level functions via library
* high level support via OSC connection
* maybe: high level support via MIDI messages
* integrated graphics API for devices with displays
* integrated state machine for handling specific use cases (e.g. pressing keys in order to have command layers, e.g. [SHIFT/GROUP_A], [LOCK] which is automatically remembered)

### What happened the last year (and first) year => 2021-06-20 - 2022-06-20?

* the discord chat now has an interesting amount of awesome people discussing ideas and wishes not only regarding the NI devices but also stuff like "mobile music/mobile devices"
* prototype/proof of concept of a hardware gateway which can connect NI devices with iPads via Bluetooth Midi and USB Midi
* first integration of OSC stuff in the rebelliond, connections to Ableton and Bitwig were successful
* new devices with basic support: Maschine JAM, Maschine Plus, Maschine MK1
* first evaluations with Traktor devices and proof that the message format is nearly the same - Rebellion can maybe support Traktor S8 or D2 (and other Traktor devices)
* proof that the Komplete M32 is some sort of different and will need some more evaluation

## Logging

Rebellion creates two logfiles in the application directory.

* print.log contains redirected "print" lines
* printdump.log contains noncaptured/unknown protocol events

Stdout will display everything (and even more) debug information.
For a test please start the application with redirected stdout into a file like

> ./rebellion_host(.exe) >> stdout.log
 
or (available on macOS)

> ./rebellion_host |tee -a stdout.log

## Privacy information

No sensible data is captured from the computer, it's just about the
communication to and from the Native Instruments devices. The logs may contain
the serial number of the devices because the communication is bound to the
serial numbers as some sort of unique identifiers.

The logfiles are human readable, in case of interest what the entries mean, 
just ask.

## Native Instrument devices

Two device types (MASCHINE_MK3, KOMPLETE_KONTROL_MK2) are currently 
automatically searched by the core when it is started/used - they are also 
tested and working (and completely mapped). That can be overwritten/extended 
via the config.lua.

Due to the fact that i don't own other devices I need external testers for 
devices. They are integrated into the core but not mapped yet (button names and 
such stuff will not work in the event messages).

It should be possible to use **Rebellion** with the following devices when a 
release version is done:


* Maschine MK3 => "MASCHINE_MK3"
* Maschine MK2 => "MASCHINE_MK2"
* Maschine MK1 => "MASCHINE_MK1"
* Maschine Plus => "MASCHINE_PLUS"
* Maschine Studio => "MASCHINE_STUDIO"
* Maschine Mikro MK3 => "MASCHINE_MIKRO_MK3"
* Maschine Mikro MK2 => "MASCHINE_MIKRO_MK2"
* Maschine Mikro MK1 => "MASCHINE_MIKRO_MK1"
* Maschine Jam => "MASCHINE_JAM"
* Komplete Kontrol MK1 => "KOMPLETE_KONTROL_MK1"
* Komplete Kontrol MK2 => "KOMPLETE_KONTROL_MK2"
* Komplete Kontrol A => "KOMPLETE_KONTROL_A"
* Komplete Kontrol M => "KOMPLETE_KONTROL_M"

Add something like
```
devices = {
    "MASCHINE_MK3",
    "KOMPLETE_KONTROL_MK2",
    "MASCHINE_MIKRO_MK3"
}
```
to a file named ```config.lua``` in the Rebellion root folder - of course 
adding your specific device to the file.

## Other NI devices? 

There are several other (or older) Native Instruments devices which may also 
work:

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
## Call for testers

***Testers for other devices appreciated!***

I am currently searching for users who can use **Rebellion** with their 
(currently unmapped) devices, test some things and send me the logs.

Reach me via email (terminar@cyberphoria.org) or the official 
Discord channel (https://discord.gg/NFdCmjcp4e).

## Call for developers

The first year showed that there is really low interest in (extended) support for NI devices in DAWs like Ableton or Reaper.
There were some interested users contacting me regarding Bitwig but most of the time, DrivenByMoss was enough as hint.

Still: If you are a developer and interested in this topic, please contact me! But - i will follow my own project goal by now.

## What can i do with rebellion?

Regarding developers: 

Take a look at
- src/host/main.cpp
- src/rebelliond/rebelliond.go

to get some first examples or contact me.
API documentation will follow within the next releases if there is something
like a stable version.

Testers - please contact me or visit the discord.
## Limitations

Regarding the KKMK2 - Currently the "led count" is fixed to the length
of the S61. When using the KNOB1 e.g. on the S49 it will not stop at the 
end (last key) of the piano light stripe and just fade away - just rotate 
counter-clockwise to get back the led. On the S88 it should stop somewhere
(12 keys?) before the end of the piano light stripe.

## Known (critical) bugs in alpha3

Sending data to the display may result in a non functional (hanging) main loop 
or may take a while to proceed. This may happen in the rebelliond testState 6.
Maybe restarting NIHA/NIHIA is helpful.

## Keys

* DBTN[number] Buttons above the displays
* KNOB[number] Knobs below the displays


| Control | Description  |
|----------|--------------|
| DBTN1  | Load Rebellion logo image into the left display (takes around 6+ seconds the first time) |
| DBTN2  | Select left display to draw on |
| DBTN4  | Clear left display |
| DBTN5  | Load Rebellion logo image into the right display (takes around 6+ seconds the first time), additionally draw a small black line onto the image |
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


## Build notes

On Windows, TDM-GCC (https://jmeubank.github.io/tdm-gcc/) is needed for the 
golang based rebelliond (uses cgo to link librebellion).
Select "GCC 10.3.0 x86_64-w64-mingw32" in Visual Studio Code.
CMake doesn't find CMAKE_AR on my system - if you have the same problem, edit 
build/CmakeCache.txt and set CMAKE_AR:FILEPATH=... to the value of
CMAKE_CXX_COMPILER_AR:FILEPATH=...

After building you have to copy the create libraries and binaries to the root folder because Rebellion 
tries to search the scripts/ folder - which is not available in the created bin/ or lib/ folder.

Currently you shouldn't try to build this if you don't really need to.
Please, just use the available binary release from github - you don't get any special if you compile it yourself.
It only shows that you can trust the build if you are paranoid. That's great but then please - if you have
problems building - try to fix them or report and stay patient ;)

# Special thanks

## Sam Lerner - Rage Against the Maschine

He did an awesome research and project at 
[Github - (NIProtocol)](https://github.com/SamL98/NIProtocol.git), wrote an 
article at [Medium - (Rage Against the Maschine)](https://lerner98.medium.com/rage-against-the-maschine-3357be1abc48) 
and was a big inspiration for this project. Some small assumptions like the 
initial bootstrap connection process were wrong which forced me to reverse 
engineer the whole stuff again on my own but his work was wonderful kickstart.

## reaKontrol project - Komplete Kontrol (MK2) support for Reaper
- James Teh / jcsteh - original reaKontrol version [Github](https://github.com/jcsteh/reaKontrol)
- Nikolas Bergerhoff / brumbear - reaKontrol fork [Github](https://github.com/brummbrum/reaKontrol)

**Rebellion** began as a discussion at the official [reaKontrol thread](https://forum.cockos.com/showthread.php?t=222331).

## Jürgen Moßgraber - DrivenByMoss

Of course he is doing great stuff at 
[The Site Of Jürgen Moßgraber (DrivenByMoss 4 Reaper/Bitwig)](http://www.mossgrabers.de/Software/Software.html) 
but regarding **Rebellion**, his comments in the reaKontrol thread about his 
experience with the NI devices were another part of the puzzle.

## Felicia Hummel

The documented work at 
[Github - Drachenkaetzchen/cabl / Native Instruments Maschine MK3 Display Protocol ](https://github.com/Drachenkaetzchen/cabl/tree/develop/doc/hardware/maschine-mk3) 
was a great help to understand the display format of the Maschine MK3. Also it 
was a bridge to realize that some data which is sent via USB to the device is 
just the same sent via IPC.

## Antonio Malara

He also did some research at [Github / biappi / Macchina ](https://github.com/biappi/Macchina) 
in 2012 trying to access the Maschine (MK1). It was nice to see that the basic 
protocol didn't change much since 2012.
