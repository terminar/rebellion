##### Table of Contents

- [Native Instruments Maschine MK3 Display Protocol](#native-instruments-maschine-mk3-display-protocol)
- [Display Information](#display-information)
- [Protocol Overview](#protocol-overview)
- [Header Overview](#header-overview)
    - [Header Part 1](#header-part-1)
    - [Header Part 2](#header-part-2)
- [Command Overview](#command-overview)
- [RGB565 Pixel Format](#rgb565-pixel-format)

<h2 id="native-instruments-maschine-mk3-display-protocol">Native Instruments Maschine MK3 Display Protocol</h2>

The Maschine MK3 Displays are listening on `Interface #5`, `Endpoint 0x04`.

<h2 id="display-information">Display Information</h2>

Two RGB565 displays with a resolution of 480x272 pixels each.

<h2 id="protocol-overview">Protocol Overview</h2>

USB packets for left and right screens can be sent in any order.

A separate USB data packet is sent for each screen, consisting of a `Header`, drawing `Command` block with color and pixel count data and `Command` to end drawing block.
 
The package `Header` contains the screen index (left or right), the coordinates of the start of the drawing area, and the size of the area in pixels.

There are several types of drawing `Command`, and they can be combined into a single block of pixel data. It looks like:

```
Header, 
Command 1, Command parameters 1, Command data 1, 
Command 2, Command parameters 2, Command data 2, 
...
Command N, Command parameters N, Command data N, 
Command to end data block
```

The commands fill the drawing area line by line from left to right starting from the pixel in the upper left corner, which matches the coordinates given in the header. Upon reaching the end of the current line, the cursor moves to the beginning of the next line of pixels.

There is no line break command or it was not found. Instead, if necessary, you can use skip pixels command to the end of the current line.

The total number of pixels in commands must match the number of pixels in the area specified in the header, including pixels that are skipped. 

The USB packet itself may have zero bytes at the end of the payload.

If the USB package of the display contains incorrect data, such as header errors or incorrect command format, the screens on the Maschine may freeze. Only physical switching off and on of the controller helps. But despite all experiments, I have not been able to harm the device itself.   

It is possible to update the entire screen, while in the header the drawing area will be like this:

    X=0
    Y=0 
    Width=480 
    Height=272

It is also possible to update only part of the screen.
For example, to update only the right half of one screen, the drawing area for that screen would be:

    X=240
    Y=0 
    Width=240 
    Height=272
  
You can skip drawing `n` pixels in the specified drawing area, keeping the pixels of the previous frame on the screen. This can be useful when only certain parts of the screen need to be updated. <br/>
For example, you need to animate the fader bars for multiple channels, but you don't need to update the UI for the entire background image. Combined with the repeat pixel command and skip pixels `n` times, this significantly saves USB bus traffic.
If you completely refresh both screens every frame and send information about every pixel on the screen, performance will be significantly degraded. In my case, the USB bandwidth was not enough to update the screens and LED buttons correctly. FPS of screens dropped and there was a long delay in updating LED panels and buttons.

<h2 id="header-overview">Header Overview</h2>

<table style="whitespace: nowrap;">
    <tr>
        <td colspan="4" style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 32 bytes → </td>
    </tr>
    <tr>
        <td colspan="2" style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 16 bytes → </td>
        <td colspan="2" style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 16 bytes → </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 8 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 8 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 8 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 8 bytes → </td>
    </tr>
    <tr valign="top">
        <td colspan="2" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header Part 1</b>
        </td>
        <td colspan="2" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header Part 2</b>
        </td>
    </tr>
    <tr valign="top">
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Command</b>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Command or Data</b>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Command or Data</b>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Command or Data</b>
        </td>
    </tr>
    <tr valign="top">
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>…</b>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>…</b>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>…</b>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>…</b>
        </td>
    </tr>
    <tr valign="top">
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Command or Data</b>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Command or Data</b>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Command or Data</b>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>End of Transmission Command</b>
        </td>
    </tr>
</table>

<h3 id="header-part-1">Header Part 1</h3>

<table style="whitespace: nowrap;">
    <tr>
        <td colspan="8" style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 16 bytes → </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
    </tr>
    <tr valign="top">
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 1</b><br/><br/>
            Always 0x84
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 2</b><br/><br/>
            Always 0x00
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 3</b><br/><br/>
            Output Display:<br/><br/>
            0x00: Left Display<br/>
            0x01: Right Display
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 4</b><br/><br/>
            Always 0x60
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 5</b><br/><br/>
            Always 0x00
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 6</b><br/><br/>
            Always 0x00
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 7</b><br/><br/>
            Always 0x00
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 8</b><br/><br/>
            Always 0x00
        </td>
    </tr>
</table>

The table above lists the header format. Apart from `Header 3`, which specifies the output display,
all other values seem to be fixed in value.

<h3 id="header-part-2">Header Part 2</h3>
<table style="whitespace: nowrap;">
    <tr>
        <td colspan="8" style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 16 bytes → </td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 2 bytes → </td>
    </tr>
    <tr valign="top">
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 9</b><br/><br/>
            X Start Address (MSB)
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 10</b><br/><br/>
            X Start Address (LSB)
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 11</b><br/><br/>
            Y Start Address (MSB)
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 12</b><br/><br/>
            Y Start Address (LSB)
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 13</b><br/><br/>
            Width (MSB)
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 14</b><br/><br/>
            Width (LSB)
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 15</b><br/><br/>
            Height (MSB)
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Header 16</b><br/><br/>
            Height (LSB)
        </td>
    </tr>
</table>

<h2 id="command-overview">Command overview</h2>
There are several types of commands.
 
- ```0x00``` Transmits the `n*2` given pixels. You may specify any number of `Pixels*2`, which need to be included as data.

- ```0x01``` Repeat the two given pixels `n` times.

- ```0x02``` Move the pixel cursor `n*2` pixels forward along the lines, skipped pixels remain unchanged and keep its current color. 

- ```0x03, 0x04``` service commands for completing a block of pixel data.

The commands for drawing and skipping pixels in the data block can be in any order.

The data block in all cases ends with a combination of service commands of the form:
```
0x03, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00
````

Each command consists of 4 bytes with an optional multiple of 4 bytes data:

<table style="whitespace: nowrap;">
    <tr>
        <td colspan="8" style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 4+n*4 bytes →</td>
    </tr>
    <tr>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;"> ← 1 byte → </td>
    </tr>
    <tr valign="top">
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Command Code</b><br/><br/>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Command Parameter 1</b><br/><br/>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Command Parameter 2</b><br/><br/>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Command Parameter 3</b><br/><br/>
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Data <br/>n*2</b><br/><br/>
            Optional
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Data <br/>n*2+1</b><br/><br/>
            Optional
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Data <br/>(n+1)*2</b><br/><br/>
            Optional
        </td>
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Data <br/>(n+1)*2+1</b><br/><br/>
            Optional
        </td>
    </tr>
    <tr valign="top">
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>0x00</b>
        </td>
        <td colspan="3" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>The count of (n*2) pixel to send.</b><br/>
            24 bit integer<br/>
            MSB in Parameter 1<br/>
            LSB in Parameter 3<br/><br/>
            Example: if n=3, you must specify 6 RGB565 pixels as a data.
        </td>
        <td colspan="2" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Pixel n</b><br/>
            RGB565 Format
        </td>
        <td colspan="2" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Pixel n+1</b><br/>
            RGB565 Format
        </td>
    </tr>
    <tr valign="top">
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>0x01</b>
        </td>
        <td colspan="3" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Count (n) of repetitions of two given pixels</b><br/>
            24 bit integer<br/>
            MSB in Parameter 1<br/>
            LSB in Parameter 3<br/><br/>
            Example: If you transmit white,black 5 times, you end up with:<br/>
            white,black,white,black,white,black,white,black,white,black
        </td>
        <td colspan="2" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Pixel n=0</b><br/>
            RGB565 Format
        </td>
        <td colspan="2" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Pixel n=1</b><br/>
            RGB565 Format
        </td>
    </tr>
    <tr valign="top">
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>0x02</b>
        </td>
        <td colspan="3" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>The number of (n*2) pixels by which the cursor will be shifted relative to the current position.</b><br/>
            24 bit integer<br/>
            MSB in Parameter 1<br/>
            LSB in Parameter 3<br/><br/>            
        </td>
        <td colspan="4" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Not used</b><br/>
            Must not be present
        </td>
    </tr>
    <tr valign="top">
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>0x03</b>
        </td>
        <td colspan="3" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Unknown</b><br/>
            Mandatory<br/>
            Must be 0x00 each            
        </td>
        <td colspan="4" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Not used</b><br/>
            Must not be present
        </td>
    </tr>
    <tr valign="top">
        <td style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>0x40</b>
        </td>
        <td colspan="3" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Not used</b><br/>
            Mandatory<br/>
            Must be 0x00 each
        </td>
        <td colspan="4" style="white-space:nowrap;font-family:monospace;text-align: center;">
            <b>Not used</b><br/>
            Must not be present
        </td>
    </tr>
</table>

<h2 id="rgb565-pixel-format">RGB565 Pixel Format</h2>

The protocol uses the RGB565 pixel format, which is basically 24 bit RGB converted to 16 bits:

- Red uses 5 bits
- Green uses 6 bits
- Blue uses 5 bits

To convert from RGB888, I will provide some sample Java code that I am using in my project.

```java
/**
 * Converting RGB888 pixel to RGB565 two byte
 * @param red color 0-255
 * @param green color 0-255
 * @param blue color 0-255
 * @return Two byte with RGB565 color
 */
public static byte[] sPixelFromRGB(final int red, final int green, final int blue) {
    int pixel_to_send_int = toRGB565(red, green, blue);
    return int2Byte(pixel_to_send_int);
}

/**
 * Convert RGB888 to RGB565 integer
 * @param r 0-255
 * @param g 0-255
 * @param b 0-255
 * @return RGB565 integer
 */
public static int toRGB565(int r, int g, int b) {
    int blue = (b >> 3) & 0x1F;
    int green = ((g >> 2) & 0x3F) << 5;
    int red = ((r >> 3) & 0x1F) << 11;

    return red | green | blue;
}

/**
 * Convert integer to byte array
 * @param value Integer value
 * @return Byte array, size=2
 */
public static byte[] int2Byte(int value) {
    final byte[] data = new byte[2];

    short valueShort;
    valueShort = (short) value;

    data[0] = (byte)((valueShort >> 8) & 0x0FF);
    data[1] = (byte)(valueShort & 0x0FF);

    return data;
}
```
