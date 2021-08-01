# Native Instruments Maschine MK3 Hardware Overview

## USB Endpoints

```
T:  Bus=04 Lev=02 Prnt=02 Port=05 Cnt=03 Dev#= 28 Spd=480 MxCh= 0
D:  Ver= 2.00 Cls=ef(misc ) Sub=02 Prot=01 MxPS=64 #Cfgs=  1
P:  Vendor=17cc ProdID=1600 Rev=01.41
S:  Manufacturer=Native Instruments
S:  Product=Maschine MK3
S:  SerialNumber=REMOVED
C:  #Ifs= 7 Cfg#= 1 Atr=80 MxPwr=480mA
I:  If#= 0 Alt= 0 #EPs= 0 Cls=01(audio) Sub=01 Prot=20 Driver=snd-usb-audio
I:  If#= 1 Alt= 1 #EPs= 1 Cls=01(audio) Sub=02 Prot=20 Driver=snd-usb-audio
I:  If#= 2 Alt= 0 #EPs= 0 Cls=01(audio) Sub=02 Prot=20 Driver=snd-usb-audio
I:  If#= 3 Alt= 0 #EPs= 2 Cls=01(audio) Sub=03 Prot=00 Driver=snd-usb-audio
I:  If#= 4 Alt= 0 #EPs= 2 Cls=03(HID  ) Sub=00 Prot=00 Driver=usbhid
I:  If#= 5 Alt= 0 #EPs= 1 Cls=ff(vend.) Sub=bd Prot=00 Driver=(none)
I:  If#= 6 Alt= 0 #EPs= 0 Cls=fe(app. ) Sub=01 Prot=01 Driver=(none)
```

The Native Instruments Maschine MK3 provides several USB endpoints:

| Interface Number | Endpoint  | Description                                                |
|------------------|-----------|------------------------------------------------------------|
| #0               | USB Audio | -                                                          |
| #1               | USB Audio | -                                                          |
| #2               | USB Audio | -                                                          |
| #3               | USB Audio | -                                                          |
| #4               | 0x83      | [Input Data (Buttons, Knobs etc)](MaschineMK3-HIDInput.md) |
| #4               | 0x03      | [Output Data (LEDs)](MaschineMK3-HIDOutput.md)             |
| #5               | 0x04      | [Bulk, Display Data](MaschineMK3-Display.md)               |
| #6               | -         | Firmware Update                                            |