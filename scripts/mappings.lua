-- Rebellion
--
-- File: mapping.lua
-- Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
-- License: LGPLv3

CONST_DEVICES = importDevices {
    { 0x1500, "MASCHINE_JAM", "MJAM", "Maschine Jam", niproto.CONST_PORT_MAIN },
    { 0x1300, "MASCHINE_STUDIO", "MSTUDIO", "Maschine Studio", niproto.CONST_PORT_MAIN },
    { 0x1140, "MASCHINE_MK2", "MMK2", "Maschine MK2", niproto.CONST_PORT_MAIN },
    {  0x808, "MASCHINE_MK1", "MMK1", "Maschine MK1", niproto.CONST_PORT_MAIN },
    { 0x1200, "MASCHINE_MIKRO_MK2", "MMMK2", "Maschine Mikro MK2", niproto.CONST_PORT_MAIN },
    { 0x1110, "MASCHINE_MIKRO_MK1", "MMMK1", "Maschine Mikro MK1", niproto.CONST_PORT_MAIN },

    { 0x1820, "MASCHINE_PLUS", "MPLUS", "Maschine Plus", niproto.CONST_PORT_HOST },
    { 0x1600, "MASCHINE_MK3", "MMK3", "Maschine MK3", niproto.CONST_PORT_HOST, 103, 2, 272, 480 },
    { 0x1700, "MASCHINE_MIKRO_MK3", "MMMK3", "Maschine Mikro MK3", niproto.CONST_PORT_HOST },
    { 0x1350, "KOMPLETE_KONTROL_MK1", "KKMK1", "Komplete Kontrol MK1", niproto.CONST_PORT_HOST },
    { 0x1610, "KOMPLETE_KONTROL_MK2", "KKMK2", "Komplete Kontrol MK2", niproto.CONST_PORT_HOST, 130, 2, 272, 480 },
    { 0x1730, "KOMPLETE_KONTROL_A", "KKA", "Komplete Kontrol A", niproto.CONST_PORT_HOST },
    { 0x1860, "KOMPLETE_KONTROL_M", "KKM", "Komplete Kontrol M", niproto.CONST_PORT_HOST }
}


CONST_CONTROLS = {

    { "TUNE", "BUTTON",
        { #CONST_DEVICES.MJAM, 103 },
    },
    { "LEVEL", "BUTTON",
        { #CONST_DEVICES.MJAM, 96 },
    },
    { "MST", "BUTTON",
        { #CONST_DEVICES.MJAM, 90 },
    },
    { "GRP", "BUTTON",
        { #CONST_DEVICES.MJAM, 91 },
    },
    { "AUX", "BUTTON",
        { #CONST_DEVICES.MJAM, 97 },
    },
    { "IN1", "BUTTON",
        { #CONST_DEVICES.MJAM, 92 },
    },
    { "HEADPHONE", "BUTTON",
        { #CONST_DEVICES.MJAM, 93 },
    },
    { "CONTROL", "BUTTON",
        { #CONST_DEVICES.MJAM, 98 },
    },
    { "SONG", "BUTTON",
        { #CONST_DEVICES.MJAM, 0 },
    },
    { "CLEAR", "BUTTON",
        { #CONST_DEVICES.MJAM, 11 },
    },


    { "KNOB_4D", "BUTTON",
        { #CONST_DEVICES.MMK3, 71 },
        { #CONST_DEVICES.MJAM, 115 },
    },

    { "CONFIRM", "KNOB_4D", --OK? CHOOSE? PRESSED?
        { #CONST_DEVICES.MMK3, 0 },
        { #CONST_DEVICES.KKMK2, 46 },
        { #CONST_DEVICES.KKM, 33 },
        { #CONST_DEVICES.MJAM, 116 },
    },
    { "UP", "KNOB_4D",
        { #CONST_DEVICES.MMK3, 2 },
        { #CONST_DEVICES.KKMK2, 45 },
        { #CONST_DEVICES.MJAM, 13 },
    },
    { "RIGHT", "KNOB_4D",
        { #CONST_DEVICES.MMK3, 3 },
        { #CONST_DEVICES.KKMK2, 47 },
        { #CONST_DEVICES.MJAM, 15 }, --RIGHT button?
    },
    { "DOWN", "KNOB_4D", --duplicate from 0 ("OK")? / "DOWN"?
        { #CONST_DEVICES.MMK3, 4 },
        { #CONST_DEVICES.KKMK2, 46 },
        { #CONST_DEVICES.MJAM, 16 },
    },
    { "LEFT", "KNOB_4D",
        { #CONST_DEVICES.MMK3, 5 },
        { #CONST_DEVICES.KKMK2, 44 },
        { #CONST_DEVICES.MJAM, 14 }, --LEFT button?
    },

    { "METRO", "BUTTON",
        { #CONST_DEVICES.MSTUDIO, 31, nil, 15 },
    },
    { "GRID", "BUTTON",
        { #CONST_DEVICES.MSTUDIO, 27, nil, 15 },
        { #CONST_DEVICES.MJAM, 111, nil, 15 },
    },
    { "SHIFT", "BUTTON",
        { #CONST_DEVICES.MMK3, 6, nil, 15 },
        { #CONST_DEVICES.KKMK2, 15, nil, 15 },
        { #CONST_DEVICES.MSTUDIO, 26, nil, 15 },
        { #CONST_DEVICES.MJAM, 105, nil, 15 },
        { #CONST_DEVICES.KKM, 0, nil, 15 },
    },
    { "NOTES", "BUTTON",
        { #CONST_DEVICES.MMK3, 16, nil, 15 },
        { #CONST_DEVICES.MJAM, 101, nil, 15 },
    },
    { "VOLUME", "BUTTON",
        { #CONST_DEVICES.MMK3, 17, nil, 15 }
    },
    { "SWING", "BUTTON",
        { #CONST_DEVICES.MMK3, 18, nil, 15 },
        { #CONST_DEVICES.MJAM, 104, nil, 15 },
    },
    { "TEMPO", "BUTTON",
        { #CONST_DEVICES.MMK3, 19, nil, 15 },
        { #CONST_DEVICES.MJAM, 110, nil, 15 },
    },
    { "NOTE_REPEAT", "BUTTON",
        { #CONST_DEVICES.MMK3, 20, nil, 15 },
        { #CONST_DEVICES.MSTUDIO, 11, nil, 15 },
        { #CONST_DEVICES.MJAM, 17, nil, 15 },
    },
    { "LOCK", "BUTTON",
        { #CONST_DEVICES.MMK3, 21, nil, 15 },
        { #CONST_DEVICES.MJAM, 102, nil, 15 },
    },

    { "PAD_MODE", "BUTTON",
        { #CONST_DEVICES.MMK3, 24, nil, 15 },
        { #CONST_DEVICES.MJAM, 10, nil, 15 },
    },
    { "KEYBOARD", "BUTTON",
        { #CONST_DEVICES.MMK3, 25, nil, 15 }
    },
    { "CHORDS", "BUTTON",
        { #CONST_DEVICES.MMK3, 26, nil, 15 }
    },
    { "STEP", "BUTTON",
        { #CONST_DEVICES.MMK3, 27, nil, 15 },
        { #CONST_DEVICES.MSTUDIO, 9, nil, 15 },
        { #CONST_DEVICES.MJAM, 9, nil, 15 },
    },


    { "FIXED_VEL", "BUTTON",
        { #CONST_DEVICES.MMK3, 28, nil, 15 },
        { #CONST_DEVICES.KKMK2, 58, nil, 15 }
    },
    { "SCENE", "BUTTON",
        { #CONST_DEVICES.MMK3, 29, nil, 15 },
        { #CONST_DEVICES.KKMK2, 26, nil, 15 }
    },
    { "PATTERN", "BUTTON",
        { #CONST_DEVICES.MMK3, 30, nil, 15 },
        { #CONST_DEVICES.KKMK2, 27, nil, 15 }
    },
    { "EVENTS", "BUTTON",
        { #CONST_DEVICES.MMK3, 31, nil, 15 },
        { #CONST_DEVICES.MSTUDIO, 24, nil, 15 },
    },
    { "VARIATION", "BUTTON",
        { #CONST_DEVICES.MMK3, 33, nil, 15 }
    },
    { "DUPLICATE", "BUTTON",
        { #CONST_DEVICES.MMK3, 34, nil, 15 },
        { #CONST_DEVICES.MJAM, 12, nil, 15 },
    },
    { "SELECT", "BUTTON",
        { #CONST_DEVICES.MMK3, 35, nil, 15 },
        { #CONST_DEVICES.MJAM, 114, nil, 15 },
    },
    { "SOLO", "BUTTON",
        { #CONST_DEVICES.MMK3, 36, nil, 15 },
        { #CONST_DEVICES.KKMK2, 25, nil, 15 },
        { #CONST_DEVICES.MJAM, 112, nil, 15 },
    },
    { "MUTE", "BUTTON",
        { #CONST_DEVICES.MMK3, 37, nil, 15 },
        { #CONST_DEVICES.KKMK2, 24, nil, 15 },
        { #CONST_DEVICES.MJAM, 113, nil, 15 },
    },
    { "PITCH", "BUTTON",
        { #CONST_DEVICES.MMK3, 38, nil, 15 }
    },
    { "MOD", "BUTTON",
        { #CONST_DEVICES.MMK3, 39, nil, 15 }
    },
    { "PERFORM", "BUTTON",
        { #CONST_DEVICES.MMK3, 40, nil, 15 },
        { #CONST_DEVICES.MJAM, 100, nil, 15 },
    },
    { "RESTART", "BUTTON",
        { #CONST_DEVICES.MMK3, 41, nil, 15 },
        { #CONST_DEVICES.MSTUDIO, 28, nil, 15 },
    },
    { "ERASE", "BUTTON",
        { #CONST_DEVICES.MMK3, 42, nil, 15 },
        { #CONST_DEVICES.MSTUDIO, 25, nil, 15 },
    },
    { "TAP", "BUTTON",
        { #CONST_DEVICES.MMK3, 43, nil, 15 },
        { #CONST_DEVICES.MSTUDIO, 8, nil, 15 },
    },
    { "TRACK", "BUTTON",
        { #CONST_DEVICES.KKMK2, 28, nil, 15 }
    },
    { "FOLLOW", "BUTTON",
        { #CONST_DEVICES.MMK3, 44, nil, 15 }
    },
    { "PLAY", "BUTTON",
        { #CONST_DEVICES.MMK3, 45, nil, 15 },
        { #CONST_DEVICES.MSTUDIO, 29, nil, 15 },
        { #CONST_DEVICES.MJAM, 106, nil, 15 },
    },
    { "REC", "BUTTON",
        { #CONST_DEVICES.MMK3, 46, nil, 15 },
        { #CONST_DEVICES.MSTUDIO, 30, nil, 15 },
        { #CONST_DEVICES.MJAM, 107, nil, 15 },
    },
    { "OCTAVE_DOWN", "BUTTON",
        { #CONST_DEVICES.KKMK2, 56, nil, 15 }
    },
    { "OCTAVE_UP", "BUTTON",
        { #CONST_DEVICES.KKMK2, 57, nil, 15 }
    },
    { "STOP", "BUTTON",
        { #CONST_DEVICES.MMK3, 47, nil, 15 }
    },

    { "MACRO", "BUTTON",
        { #CONST_DEVICES.MMK3, 48, nil, 15 },
        { #CONST_DEVICES.MSTUDIO, 10, nil, 15 },
        { #CONST_DEVICES.MJAM, 95, nil, 15 },
    },
    { "SETTINGS", "BUTTON",
        { #CONST_DEVICES.MMK3, 49, nil, 15 }
    },
    { "RIGHT", "BUTTON",
        { #CONST_DEVICES.MMK3, 50, nil, 15 },
        { #CONST_DEVICES.KKMK2, 21, nil, 15 },
        { #CONST_DEVICES.MJAM, 109, nil, 15 },
    },
    { "SAMPLING", "BUTTON",
        { #CONST_DEVICES.MMK3, 51, nil, 15 }
    },
    { "ARP", "BUTTON",
        { #CONST_DEVICES.KKMK2, 10, nil, 15 }
    },
    { "AUTO", "BUTTON",
        { #CONST_DEVICES.MMK3, 61, nil, 15 },
        { #CONST_DEVICES.KKMK2, 8, nil, 15 },
        { #CONST_DEVICES.MJAM, 99, nil, 15 },
    },
    { "ARRANGER", "BUTTON",
        { #CONST_DEVICES.MMK3, 57, nil, 15 }
    },
    { "IDEAS", "BUTTON",
        { #CONST_DEVICES.KKM, 5, nil, 15 }
    },
    { "BROWSER", "BUTTON",
        { #CONST_DEVICES.MMK3, 58, nil, 15 },
        { #CONST_DEVICES.KKMK2, 34, nil, 15 },
        { #CONST_DEVICES.MJAM, 94, nil, 15 },
        { #CONST_DEVICES.KKM, 16, nil, 15 },
    },
    { "CHANNEL", "BUTTON",
        { #CONST_DEVICES.MMK3, 56, nil, 15 }
    },
    { "LEFT", "BUTTON",
        { #CONST_DEVICES.MMK3, 59, nil, 15 },
        { #CONST_DEVICES.KKMK2, 23, nil, 15 },
        { #CONST_DEVICES.MJAM, 108, nil, 15 },
    },
    { "FILE", "BUTTON",
        { #CONST_DEVICES.MMK3, 60, nil, 15 }
    },
    { "MIXER", "BUTTON",
        { #CONST_DEVICES.MMK3, 52, nil, 15 },
        { #CONST_DEVICES.KKMK2, 32, nil, 15 }
    },
    { "PLUG_IN", "BUTTON",
        { #CONST_DEVICES.MMK3, 53, nil, 15 },
        { #CONST_DEVICES.KKMK2, 33, nil, 15 },
        { #CONST_DEVICES.KKM, 17, nil, 15 }
    },
    { "QUANTIZE", "BUTTON",
        { #CONST_DEVICES.KKMK2, 9, nil, 15 }
    },
    { "SCALE", "BUTTON",
        { #CONST_DEVICES.KKMK2, 11, nil, 15 },
        { #CONST_DEVICES.KKM, 1, nil, 15 }
    },
    { "UNDO", "BUTTON",
        { #CONST_DEVICES.KKMK2, 14, nil, 15 }
    },
    { "ARP", "BUTTON",
        { #CONST_DEVICES.KKMK2, 10, nil, 15 },
        { #CONST_DEVICES.KKM, 2, nil, 15 }
    },
    { "PRESET_UP", "BUTTON",
        { #CONST_DEVICES.KKMK2, 20, nil, 15 }
    },
    { "PRESET_DOWN", "BUTTON",
        { #CONST_DEVICES.KKMK2, 22, nil, 15 }
    },

    { "GROUP_A", "BUTTON",
        { #CONST_DEVICES.MMK3, 8, 30, 15 },
        { #CONST_DEVICES.MSTUDIO, 16, 30, 15 },
        { #CONST_DEVICES.MJAM, 82 },
    },
    { "GROUP_B", "BUTTON",
        { #CONST_DEVICES.MMK3, 9, 31, 15 },
        { #CONST_DEVICES.MSTUDIO, 19, 30, 15 },
        { #CONST_DEVICES.MJAM, 83 },
    },
    { "GROUP_C", "BUTTON",
        { #CONST_DEVICES.MMK3, 10, 32, 15 },
        { #CONST_DEVICES.MSTUDIO, 20, 30, 15 },
        { #CONST_DEVICES.MJAM, 84 },
    },
    { "GROUP_D", "BUTTON",
        { #CONST_DEVICES.MMK3, 11, 33, 15 },
        { #CONST_DEVICES.MSTUDIO, 23, 30, 15 },
        { #CONST_DEVICES.MJAM, 85 },
    },
    { "GROUP_E", "BUTTON",
        { #CONST_DEVICES.MMK3, 12, 34, 15 },
        { #CONST_DEVICES.MSTUDIO, 17, 30, 15 },
        { #CONST_DEVICES.MJAM, 86 },
    },
    { "GROUP_F", "BUTTON",
        { #CONST_DEVICES.MMK3, 13, 35, 15 },
        { #CONST_DEVICES.MSTUDIO, 18, 30, 15 },
        { #CONST_DEVICES.MJAM, 87 },
    },
    { "GROUP_G", "BUTTON",
        { #CONST_DEVICES.MMK3, 14, 36, 15 },
        { #CONST_DEVICES.MSTUDIO, 21, 30, 15 },
        { #CONST_DEVICES.MJAM, 88 },
    },
    { "GROUP_H", "BUTTON",
        { #CONST_DEVICES.MMK3, 15, 37, 15 },
        { #CONST_DEVICES.MSTUDIO, 22, 30, 15 },
        { #CONST_DEVICES.MJAM, 89 },
    },

    { "PAD1", "PAD",
        { #CONST_DEVICES.MMK3, nil, 100, 15 }
    },
    { "PAD2", "PAD",
        { #CONST_DEVICES.MMK3, nil, 101, 15 }
    },
    { "PAD3", "PAD",
        { #CONST_DEVICES.MMK3, nil, 102, 15 }
    },
    { "PAD4", "PAD",
        { #CONST_DEVICES.MMK3, nil, 103, 15 }
    },
    { "PAD5", "PAD",
        { #CONST_DEVICES.MMK3, nil, 96, 15 }
    },
    { "PAD6", "PAD",
        { #CONST_DEVICES.MMK3, nil, 97, 15 }
    },
    { "PAD7", "PAD",
        { #CONST_DEVICES.MMK3, nil, 98, 15 }
    },
    { "PAD8", "PAD",
        { #CONST_DEVICES.MMK3, nil, 99, 15 }
    },
    { "PAD9", "PAD",
        { #CONST_DEVICES.MMK3, nil, 92, 15 }
    },
    { "PAD10", "PAD",
        { #CONST_DEVICES.MMK3, nil, 93, 15 }
    },
    { "PAD11", "PAD",
        { #CONST_DEVICES.MMK3, nil, 94, 15 }
    },
    { "PAD12", "PAD",
        { #CONST_DEVICES.MMK3, nil, 95, 15 }
    },
    { "PAD13", "PAD",
        { #CONST_DEVICES.MMK3, nil, 88, 15 }
    },
    { "PAD14", "PAD",
        { #CONST_DEVICES.MMK3, nil, 89, 15 }
    },
    { "PAD15", "PAD",
        { #CONST_DEVICES.MMK3, nil, 90, 15 }
    },
    { "PAD16", "PAD",
        { #CONST_DEVICES.MMK3, nil, 91, 15 }
    },

    { "DBTN1", "BUTTON",
        { #CONST_DEVICES.MMK3, 64, 13, 15 },
        { #CONST_DEVICES.KKMK2, 4, 3, 15 },
        { #CONST_DEVICES.MJAM, 1 },
    },
    { "DBTN2", "BUTTON",
        { #CONST_DEVICES.MMK3, 65, 14, 15 },
        { #CONST_DEVICES.KKMK2, 5, 4, 15 },
        { #CONST_DEVICES.MJAM, 2 },
    },
    { "DBTN3", "BUTTON",
        { #CONST_DEVICES.MMK3, 66, 15, 15 },
        { #CONST_DEVICES.KKMK2, 6, 5, 15 },
        { #CONST_DEVICES.MJAM, 3 },
    },
    { "DBTN4", "BUTTON",
        { #CONST_DEVICES.MMK3, 67, 16, 15 },
        { #CONST_DEVICES.KKMK2, 7, 6, 15 },
        { #CONST_DEVICES.MJAM, 4 },
    },
    { "DBTN5", "BUTTON",
        { #CONST_DEVICES.MMK3, 68, 17, 15 },
        { #CONST_DEVICES.KKMK2, 0, 7, 15 },
        { #CONST_DEVICES.MJAM, 5 },
    },
    { "DBTN6", "BUTTON",
        { #CONST_DEVICES.MMK3, 69, 18, 15 },
        { #CONST_DEVICES.KKMK2, 1, 8, 15 },
        { #CONST_DEVICES.MJAM, 6 },
    },
    { "DBTN7", "BUTTON",
        { #CONST_DEVICES.MMK3, 70, 19, 15 },
        { #CONST_DEVICES.KKMK2, 2, 9, 15 },
        { #CONST_DEVICES.MJAM, 7 },
    },
    { "DBTN8", "BUTTON",
        { #CONST_DEVICES.MMK3, 7, 20, 15 },
        { #CONST_DEVICES.KKMK2, 3, 10, 15 },
        { #CONST_DEVICES.MJAM, 8 },
    },

    { "KNOB1", "BUTTON",
        { #CONST_DEVICES.MMK3, 79 },
        { #CONST_DEVICES.KKMK2, 55 },
        { #CONST_DEVICES.MSTUDIO, 88 },
    },
    { "KNOB2", "BUTTON",
        { #CONST_DEVICES.MMK3, 78 },
        { #CONST_DEVICES.KKMK2, 54 },
        { #CONST_DEVICES.MSTUDIO, 89 },
    },
    { "KNOB3", "BUTTON",
        { #CONST_DEVICES.MMK3, 77 },
        { #CONST_DEVICES.KKMK2, 53 },
        { #CONST_DEVICES.MSTUDIO, 90 },
    },
    { "KNOB4", "BUTTON",
        { #CONST_DEVICES.MMK3, 76 },
        { #CONST_DEVICES.KKMK2, 52 },
        { #CONST_DEVICES.MSTUDIO, 91 },
    },
    { "KNOB5", "BUTTON",
        { #CONST_DEVICES.MMK3, 75 },
        { #CONST_DEVICES.KKMK2, 51 },
        { #CONST_DEVICES.MSTUDIO, 92 },
    },
    { "KNOB6", "BUTTON",
        { #CONST_DEVICES.MMK3, 74 },
        { #CONST_DEVICES.KKMK2, 50 },
        { #CONST_DEVICES.MSTUDIO, 93 },
    },
    { "KNOB7", "BUTTON",
        { #CONST_DEVICES.MMK3, 73 },
        { #CONST_DEVICES.KKMK2, 49 },
        { #CONST_DEVICES.MSTUDIO, 94 },
    },
    { "KNOB8", "BUTTON",
        { #CONST_DEVICES.MMK3, 72 },
        { #CONST_DEVICES.KKMK2, 48 },
        { #CONST_DEVICES.MSTUDIO, 95 },
    },
    { "KNOB9", "KNOB",
        { #CONST_DEVICES.MMK3 },
    },
    { "KNOB10", "KNOB",
        { #CONST_DEVICES.MMK3 },
    },

}

--25 leds for the touchstrip on the MASCHINE_MK3, beginning at 62
for i=1,25 do
    insert(CONST_CONTROLS, { "TOUCHSTRIP" .. string(i), "TOUCHSTRIP",
        { #CONST_DEVICES.MMK3, nil, 62+i, 15 },
     })
end

--5*12 leds for the keyboard on the KOMPLETE_KONTROL_MK2, beginning at 70
for i=1,60 do
    insert(CONST_CONTROLS, { "KEYBOARD" .. string(i), "KEYBOARD",
        { #CONST_DEVICES.MMK2, nil, 70+i, 15 },
     })

end

-- 64 pads on maschine jam
for i=1,64 do
    insert(CONST_CONTROLS, { "BTN" .. string(i), "BUTTON",
        { #CONST_DEVICES.MJAM, 17+i },
     })
end

CONST_CONTROLS = importControls(CONST_CONTROLS)

--[[

const uint8_t kKompleteKontrolKeyStateLightOff = 0x00;
const uint8_t kKompleteKontrolButtonLightOff = 0x00;

// We have 16 colors accross the rainbow, with 4 levels of intensity.
// Additionally there is white (sort of), again with 4 levels of intensity.
const size_t kKompleteKontrolColorCount = 17;
const size_t kKompleteKontrolColorIntensityLevelCount = 4;

const uint8_t kKompleteKontrolColorRed = 0x04;
const uint8_t kKompleteKontrolColorOrange = 0x08;
const uint8_t kKompleteKontrolColorYellow = 0x10;
const uint8_t kKompleteKontrolColorGreen = 0x1C;
const uint8_t kKompleteKontrolColorBlue = 0x2C;
const uint8_t kKompleteKontrolColorPurple = 0x34;
const uint8_t kKompleteKontrolColorPink = 0x38;
const uint8_t kKompleteKontrolColorWhite = 0x44;

const uint8_t kKompleteKontrolColorMask = 0xfc;
const uint8_t kKompleteKontrolIntensityMask = 0x03;

const uint8_t kKompleteKontrolIntensityLow = 0x00;
const uint8_t kKompleteKontrolIntensityMedium = 0x01;
const uint8_t kKompleteKontrolIntensityHigh = 0x02;
const uint8_t kKompleteKontrolIntensityBright = 0x03;

const uint8_t kKompleteKontrolColorLightBlue = kKompleteKontrolColorBlue | kKompleteKontrolIntensityHigh;
const uint8_t kKompleteKontrolColorBrightBlue = kKompleteKontrolColorBlue | kKompleteKontrolIntensityBright;

const uint8_t kKompleteKontrolColorLightGreen = kKompleteKontrolColorGreen | kKompleteKontrolIntensityHigh;
const uint8_t kKompleteKontrolColorBrightGreen = kKompleteKontrolColorGreen | kKompleteKontrolIntensityBright;

const uint8_t kKompleteKontrolColorLightYellow = kKompleteKontrolColorYellow | kKompleteKontrolIntensityHigh;

const uint8_t kKompleteKontrolColorLightOrange = kKompleteKontrolColorOrange | kKompleteKontrolIntensityHigh;
const uint8_t kKompleteKontrolColorBrightOrange = kKompleteKontrolColorOrange | kKompleteKontrolIntensityBright;

const uint8_t kKompleteKontrolColorLightWhite = kKompleteKontrolColorWhite | kKompleteKontrolIntensityHigh;
const uint8_t kKompleteKontrolColorBrightWhite = kKompleteKontrolColorWhite | kKompleteKontrolIntensityBright;

   // TODO: This is just a very rough, initial approximation of the actual palette of the S-series controllers.
    const unsigned char palette[17][3] = {
        { 0xFF, 0x00, 0x00 },   // 0: red
        { 0xFF, 0x3F, 0x00 },   // 1:
        { 0xFF, 0x7F, 0x00 },   // 2: orange
        { 0xFF, 0xCF, 0x00 },   // 3: orange-yellow
        { 0xFF, 0xFF, 0x00 },   // 4: yellow
        { 0x7F, 0xFF, 0x00 },   // 5: green-yellow
        { 0x00, 0xFF, 0x00 },   // 6: green
        { 0x00, 0xFF, 0x7F },   // 7:
        { 0x00, 0xFF, 0xFF },   // 8:
        { 0x00, 0x7F, 0xFF },   // 9:
        { 0x00, 0x00, 0xFF },   // 10: blue
        { 0x3F, 0x00, 0xFF },   // 11:
        { 0x7F, 0x00, 0xFF },   // 12: purple
        { 0xFF, 0x00, 0xFF },   // 13: pink
        { 0xFF, 0x00, 0x7F },   // 14:
        { 0xFF, 0x00, 0x3F },   // 15:
        { 0xFF, 0xFF, 0xFF }    // 16: white
    };

--]]