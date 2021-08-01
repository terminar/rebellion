# Native Instruments Maschine MK3 USB HID Input

The Maschine MK3 Buttons and Pads are transmitting on `Interface #4`, `Endpoint 0x83`.



## Protocol Overview

The first byte indicates the type of the transmission.

Type 0x01: Regular Buttons
Type 0x02: Pads

### Notes on Knobs

The Maschine MK3 has, like its predecessors, endless potentiometers. They
are very rare, and not even electronic engineers may have heard of them.

Since they are rarely used, they are quite expensive, but have a huge advantage
over well-known encoders: Their position can be read at any time. When I got the
Maschine MK1, I was very happy about Native Instruments using endless potentiometers.

As such, reading the (absolute) position for the knobs (0x12-0x27) is very easy.

### Regular Buttons, Encoders and Knobs

<table style="whitespace: nowrap;">
    <tr>
        <td></td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> Offset </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
    </tr>
       <tr>
       <td rowspan="84" style="white-space:nowrap;font-family:monospace;text-align: center;"> ↑<br/>42 bytes <br/> ↓</td>
       <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 0 </td>
       <td style="white-space:nowrap;font-family:monospace;"> Always 01 for Buttons, Knobs etc </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 1 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0: Encoder Push<br/>
            Bit 1: Pedal Connected<br/>
            Bit 2: Encoder Up<br/>
            Bit 3: Encoder Right<br/>
            Bit 4: Encoder Down<br/>
            Bit 5: Encoder Left<br/>
            Bit 6: Shift<br/>
            Bit 7: Display Button 8<br/>
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 2 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0: Group A<br/>
            Bit 1: Group B<br/>
            Bit 2: Group C<br/>
            Bit 3: Group D<br/>
            Bit 4: Group E<br/>
            Bit 5: Group F<br/>
            Bit 6: Group G<br/>
            Bit 7: Group H<br/>
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 3 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0: Notes<br/>
            Bit 1: Volume<br/>
            Bit 2: Swing<br/>
            Bit 3: Tempo<br/>
            Bit 4: Note Repeat<br/>
            Bit 5: Lock<br/>
            Bit 6: Unknown<br/>
            Bit 7: Unknown<br/>
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 4 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0: Pad Mode<br/>
            Bit 1: Keyboard<br/>
            Bit 2: Chords<br/>
            Bit 3: Step<br/>
            Bit 4: Fixed Vel<br/>
            Bit 5: Scene<br/>
            Bit 6: Pattern<br/>
            Bit 7: Events<br/>
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 5 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0: Microphone Connected<br/>
            Bit 1: Variation<br/>
            Bit 2: Duplicate<br/>
            Bit 3: Select<br/>
            Bit 4: Solo<br/>
            Bit 5: Mute<br/>
            Bit 6: Pitch<br/>
            Bit 7: Mod<br/>
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 6 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0: Perform<br/>
            Bit 1: Restart<br/>
            Bit 2: Erase<br/>
            Bit 3: Tap<br/>
            Bit 4: Follow<br/>
            Bit 5: Play<br/>
            Bit 6: Rec<br/>
            Bit 7: Stop<br/>
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 7 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0: Macro<br/>
            Bit 1: Settings<br/>
            Bit 2: Arrow Right<br/>
            Bit 3: Sampling<br/>
            Bit 4: Mixer<br/>
            Bit 5: Plugin<br/>
            Bit 6: Unknown<br/>
            Bit 7: Unknown<br/>
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 8 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0: Channel / MIDI<br/>
            Bit 1: Arranger<br/>
            Bit 2: Browser +Plugin<br/>
            Bit 3: Arrow Left<br/>
            Bit 4: File / Save<br/>
            Bit 5: Auto<br/>
            Bit 6: Unknown<br/>
            Bit 7: Unknown<br/>
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 9 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0: Display Button 1<br/>
            Bit 1: Display Button 2<br/>
            Bit 2: Display Button 3<br/>
            Bit 3: Display Button 4<br/>
            Bit 4: Display Button 5<br/>
            Bit 5: Display Button 6<br/>
            Bit 6: Display Button 7<br/>
            Bit 7: Main Knob Touch<br/>
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 10 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0: Knob Touch 8<br/>
            Bit 1: Knob Touch 7<br/>
            Bit 2: Knob Touch 6<br/>
            Bit 3: Knob Touch 5<br/>
            Bit 4: Knob Touch 4<br/>
            Bit 5: Knob Touch 3<br/>
            Bit 6: Knob Touch 2<br/>
            Bit 7: Knob Touch 1<br/>
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 11 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-3: Main Encoder Position (0-15)<br/>
            Bit 4: Unknown<br/>
            Bit 5: Unknown<br/>
            Bit 6: Unknown<br/>
            Bit 7: Unknown<br/>
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 12 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Knob 1 Position (Lower Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 13 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-1: Knob 1 Position (Upper Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 14 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Knob 2 Position (Lower Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 15 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-1: Knob 2 Position (Upper Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 16 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Knob 3 Position (Lower Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 17 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-1: Knob 3 Position (Upper Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 18 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Knob 4 Position (Lower Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 19 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-1: Knob 4 Position (Upper Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 20 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Knob 5 Position (Lower Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 21 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-1: Knob 5 Position (Upper Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 22 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Knob 6 Position (Lower Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 23 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-1: Knob 6 Position (Upper Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 24 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Knob 7 Position (Lower Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 25 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-1: Knob 7 Position (Upper Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 26 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Knob 8 Position (Lower Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 27 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-1: Knob 8 Position (Upper Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 28 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Touch Strip Finger 1 Data A
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 29 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Touch Strip Finger 1 Data B
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 30 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Touch Strip Finger 1 Data C
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 31 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Touch Strip Finger 1 Data D
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 32 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Touch Strip Finger 2 Data A
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 33 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Touch Strip Finger 2 Data B
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 34 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Touch Strip Finger 2 Data C
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 35 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Bit 0-7: Touch Strip Finger 2 Data D
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 36 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Mic In Gain (Lower Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 37 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Mic In Gain (Upper Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 38 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Headphone Volume (Lower Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 39 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Headphone Volume (Upper Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 40 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Master Volume (Lower Byte)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 41 </td>
        <td style="white-space:nowrap;font-family:monospace;">
            Master Volume (Upper Byte)
        </td>
    </tr>
</table>


### Pads

Pads don't have a fixed position in the protocol. Instead, the first pad
hit is transmitted first, up to a maximum of 21(?) pad events. Probably
this is to allow for multiple hits in one transmission cycle.

<table style="whitespace: nowrap;">
    <tr>
        <td></td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> Offset </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
    </tr>
       <tr>
       <td rowspan="84" style="white-space:nowrap;font-family:monospace;text-align: center;"> ↑<br/>64 bytes <br/> ↓</td>
       <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 0 </td>
       <td style="white-space:nowrap;font-family:monospace;"> Always 02 for Pads</td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 1 </td>
        <td style="white-space:nowrap;font-family:monospace;">
           Pad Number (Numbered from top right to bottom left)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 2 </td>
        <td style="white-space:nowrap;font-family:monospace;">
           Pad Data A (not yet reverse engineered)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> 3 </td>
        <td style="white-space:nowrap;font-family:monospace;">
           Pad Data B (not yet reverse engineered)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> n+0 </td>
        <td style="white-space:nowrap;font-family:monospace;">
           Pad Number (Numbered from top right to bottom left)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> n+1 </td>
        <td style="white-space:nowrap;font-family:monospace;">
           Pad Data A (not yet reverse engineered)
        </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> n+2 </td>
        <td style="white-space:nowrap;font-family:monospace;">
           Pad Data B (not yet reverse engineered)
        </td>
    </tr>
</table>

