
# Frequency asked questions

## General questions

#### Q: Why is Rebellion so slow at the moment?

This is a known (and currently, accepted) situation due to extreme 
logging and debugging. This will be optimized in the future.

#### Q: The configuration is ugly and just stupid!

Yes. Give me some time, that will change. It was the easiest for the moment
just to look for a file named ```config.lua``` without fiddling with different
OS user folders and such stuff.

#### Q: Why? Why did you do this project?

I really like the NI hardware but the software support is really limited.
Also the integration in Reaper (where the idea started, in the Reaper forum, 
reaKontrol thread) is nonexisting and only driven by great projects 
like ***reaKontrol*** and ***drivenByMoss***. Also the support in Ableton just 
basic transport functions.

I wanted to see how much control I can get.

#### Q: Is there a timeline?

No. I'm doing this completely on my own in my spare time, it is a hobby project.
I will not promise release dates, functionality or something else.
That may change if there is more interest in the project.

#### Q: What does this ***mapping*** mean that is mentioned in the README?

The devices will send events to the library (and host) and the library react to 
RPC commands. To identify the correct buttons and pads (or keys) librebellion 
contains a generic mapping for the buttons like "PLAY", "VOLUME", "SHIFT" and 
so on. It is translated to the specific IDs which are used for the devices and 
vice versa.

#### Q: Do you plan to support other non Native Instruments devices?

Technically it's possible and may make sence regarding the architectural core 
but currently there is no such plan yet - maybe later.

#### Q: Can i use Rebellion to support my old Maschine MK1 on newer macOS?

No. As described, this project uses NIHA/NIHIA which needs the hardware driver for the
 Maschine MK1.

Please take a look at https://github.com/shaduzlabs/cabl

(There is a POC of Rebellion on Linux to access the hardware as gateway and some sort of 
Maschine Plus like thing on steroids but it is a hardware proof of concept and will likely 
never be released).

#### Q: Can i use Rebellion to attach the NI devices to my iPad?

No. There is a PoC using a cheap, well available microcontroller with a custom firmware as 
gateway between NI devices and Bluetooth MIDI / USB MIDI but that's just a PoC.

Since the WWDC 2022 announcement that M1 powered devices like the iPad Pro will support 
DriverKit - it's possible that (if NI doesn't create drivers for the iPad) i will maybe 
take a look at it and create a driver for it but just an idea yet.

## Developer questions

#### Q: Why are three different different programing languages used?

The reason is performance, usability and - just for fun.

**C++**: is used for the core because this is the low-level/library part which
eventially needs to be fast. Also it can be easily linked as client library to 
nearly every possibility programing language. Also this part should not change
that often.

**Lua**: is used for fast prototyping the Native Instruments protocol and to 
extend the core with higher level (scripted) user defined functions and some 
logic. Some functionality will move from Lua to the C++ core later on for 
(speed) optimization.

**Go**: Golang is used for the daemon/gateway which will maybe support virtual 
MIDI and OSC connections in the future.

#### Q: Is there an example how to use librebellion directly?

Yes, take a look at src/host. It contains a small C++ console example which 
links to librebellion and talks via the RPC DLL functions.

#### Q: Where does the development happen?

The development is currently hosted and done in a private Gitea instance and 
git repository. A public mirror of the releases is available on Github 
at https://github.com/terminar/rebellion - that may change in the future.

#### Q: Why are some parts (closed) binary scripts?

Legal reasons i have to examine. That may also change in the future.

#### Q: There are bugs in the code! Maybe memory leaks! You left comments! Todos!

Yes! It's a project moving from "proof of concept/alpha" to a usable
state. Many things are temporarily implemented to check if it's working and
a good idea. Some things may change completely in the future.
It's a sandbox playground.

#### Q: Rebelliond crashes (occasionally) on my macOS system!

Yes! That happens somewhere since a newer golang version i think.
Currently I can only guess (because it worked previously and nothing was changed on the code):

MacOS is throwing an "urgent I/O condition" (which is SIGURG, described as:
Urgent condition on socket: high bandwidth data is available.") when new data is waiting
on the mach ports. SIGURG seems to be defined as signal 16. The macos kernel now sends
signal 16/SIGURG to the go application. Rebelliond uses 'cgo' to interop with C and the callback
in librebellion. 

Go now throws an error:

```
signal 16 received but handler not on signal stack
fatal error: non-Go code set up signal handler without SA_ONSTACK flag
```

That can be tracked down to golang's  func sigNotOnStack(sig uint32).
https://github.com/golang/go/blob/51885c1fa2eff36c421d6669816f971b54b63a33/src/runtime/signal_unix.go#L1045

Something changed how signals are interpreted in go (or something in macOS changed, not sure).


#### Q: How is this stuff licensed?

- scrips: currently closed source distributed as binaries
- librebellion, host, rebelliond: LGPLv3
- external Lua libraries used (in scripts folder) are licensed MIT
- Rebellion logo: Björn Kalkbrenner <terminar@cyberphoria.org>
