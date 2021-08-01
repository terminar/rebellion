# Native Instruments Maschine MK3 USB HID Input

The Maschine MK3 LEDs are listening on `Interface #4`, `Endpoint 0x03`.


## Protocol Overview

The first byte indicates the type of the transmission.

- Type 0x80: LED Buttons
- Type 0x81: Pads, Touch Strip LEDs

### Notes on the color format

The color format needs to be determined. Each LED, no matter if single-color
or RGB, is controlled via a single byte.

### LED Buttons

<table style="whitespace: nowrap;">
    <tr>
        <td></td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> Offset </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
    </tr>
       <tr>
       <td rowspan="84" style="white-space:nowrap;font-family:monospace;text-align: center;"> ↑<br/>62 bytes <br/> ↓</td>
       <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 0 </td>
       <td style="white-space:nowrap;font-family:monospace;"> Always 0x80 for Buttons, Knobs etc </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 1 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Channel/MIDI
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 2 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Plugin/Instance
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 3 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Arranger
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 4 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Mixer
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 5 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Browser/Plugin
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 6 </td>
        <td style="white-space:nowrap;font-family:monospace;">
           Unicolor: Sampler
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 7 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Arrow Left
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 8 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Arrow Right
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 9 </td>
        <td style="white-space:nowrap;font-family:monospace;">
           Unicolor: File/Save As
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 10 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Settings
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 11 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Macro/Set
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 12 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Display Button 1
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 13 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Display Button 2
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 14 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Display Button 3
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 15 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Display Button 4
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 16 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Display Button 5
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 17 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Display Button 6
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 18 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Display Button 7
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 19 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Display Button 8
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 20 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Volume
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 21 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Swing
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 22 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Note Repeat / Arp
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 23 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Tempo
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 24 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Lock
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 25 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Pitch
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 26 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Mod
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 27 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Perform / FX Select
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 28 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Notes
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 29 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Group A
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 30 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Group B
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 31 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Group C
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 32 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Group D
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 33 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Group E
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 34 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Group F
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 35 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Group G
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 36 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Group H
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 37 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Restart / Loop
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 38 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Erase / Replace
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 39 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Tap / Metro
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 40 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Follow Grid
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 41 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Play
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 42 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Rec / Count In
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 43 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Stop
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 44 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Shift
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 45 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Fixed Vel
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 46 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Pad Mode
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 47 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Keyboard
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 48 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Chords
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 49 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Step
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 50 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Scene
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 51 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Pattern
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 52 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Events
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 53 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Variation / Navigate
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 54 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Duplicate / Double
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 55 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Select
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 56 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Solo
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 57 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Unicolor: Mute/Choke
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 58 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Navigation LED Up
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 59 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Navigation LED Left
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 60 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Navigation LED Right
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 61 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            RGB: Navigation LED Down
        </td>
    </tr>
</table>

### Pads / Touch Strip

<table style="whitespace: nowrap;">
    <tr>
        <td></td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> Offset </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
    </tr>
       <tr>
       <td rowspan="84" style="white-space:nowrap;font-family:monospace;text-align: center;"> ↑<br/>42 bytes <br/> ↓</td>
       <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 0 </td>
       <td style="white-space:nowrap;font-family:monospace;"> Always 0x81 for Pads</td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 1-26</td>
        <td style="white-space:nowrap;font-family:monospace;">
           Touch Pad LEDs, from left to right<br/>
           25 RGB
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 27-42 </td>
        <td style="white-space:nowrap;font-family:monospace;">
           16 RGB Pads<br/>
        </td>
    </tr>
</table>

