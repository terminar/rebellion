-- Rebellion
--
-- File: udf.lua
-- Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
-- License: LGPLv3

local log = require 'log'
local png = require "pnglua.png"
local niproto = require 'niproto'
local dispatcher = require 'dispatcher'()

local tinsert = table.insert

local dispatchMapEvents = true
local _selectedDisplay = 0
local _selectedColor = {
    R = 100,
    G = 100,
    B = 100
}

local _curPos = {
    X = 1,
    Y = 1
}
local _displayData
local _pngImageCache = {}


local function _loadPngImage(name)
    local img = _pngImageCache[name]
    if not img then
        img = png("rebellion-480x272.png")
        _pngImageCache[name] = img
    end
    return img
end

local function _initDisplayData(height, width)
    _displayData = {
        [0] = {},
        [1] = {}
    }
    for i=0,1 do
        for y = 1,height do
            for x = 1,width do
                if not _displayData[i][y] then _displayData[i][y] = {} end
                if not _displayData[i][y][x] then _displayData[i][y][x] = 0 end
            end
        end
    end
end


--leds
local function _setAllLeds(self,color)
    local data = self:getLedData()
    if not data then
        data = niproto.initLedData(self:getDevice():getId())
    end

    for i=1,#data do
        niproto.setLedColor(data,i,color)
    end

    self:setLedData(data)
    return data
end

--=== BEGIN UDF FUNCTIONS ===---------------------------------------------------

--key
local function udf_test_SwitchAllKeyLeds(event)
    local self = assert(event.self, "self not provided")

    log.info("test_SwitchAllKeyLeds: trying to switch all key leds")

    local lastbtncolor=self:getLastBtnColor() or 0
    local btncolor
    if lastbtncolor == 0xff then
        btncolor=0
    else
        btncolor=0xff
    end
    self:setLastBtnColor(btncolor)

    local data = _setAllLeds(self,btncolor)
    self:sendLedData(data)
end

local function udf_test_resetAllKeyLeds(event)
    local self = assert(event.self, "self not provided")

    local data = _setAllLeds(self,0)
    self:setLastBtnColor(0)

    self:sendLedData(data)
end

local function udf_test_lightupButton(event)
    local self = assert(event.self, "self not provided")

    local ludata = self:getLightupButtonData()
    if not ludata then
        ludata = {
            last = 88,
            color = 17,
            intense = 0
        }

        local deviceid = self:getDevice():getId()
        if deviceid and niproto.CONST_DEVICES[deviceid]  then
            ludata.ledcnt = niproto.CONST_DEVICES[deviceid].ledcnt or 0
        end
    end

    local led = ludata.last
    local color = ludata.color
    local intense = ludata.intense
    if event and event.data and event.data.direction then
        if event.data.knob == "KNOB1" then
            if event.data.direction == "CLOCKWISE" then
                led = ludata.last + 1
            else
                led = ludata.last - 1
            end

            if led < 0 then
                led=1
            end
            if led > ludata.ledcnt then
                led = ludata.ledcnt
            end
        elseif event.data.knob == "KNOB2" then
            if event.data.direction == "CLOCKWISE" then
                color = ludata.color + 1
            else
                color = ludata.color - 1
            end

        elseif event.data.knob == "KNOB3" then

            if event.data.direction == "CLOCKWISE" then
                intense = ludata.intense + 1
            else
                intense = ludata.intense - 1
            end
        end

        if color < 0 then --off
            color=0
        end
        if color > 17 then --17 colors * 4 intensity
            color = 17
        end

        if intense < 0 then
            intense=0
        end
        if intense > 3 then
            intense = 3
        end

        local data = self:getLedData()
        if not data then
            data = niproto.initLedData(self:getDevice():getId())
        end

        for i=1,#data do
            data[i] = 0
        end
        niproto.setLedColor(data,led,color,intense)

        self:sendLedData(data)

        ludata.last = led
        ludata.color = color
        ludata.intense = intense
    end

    self:setLightupButtonData(ludata)
end

local function udf_select_display(event)
    local self = assert(event.self, "self not provided")

    if event.data.button == "DBTN2" then
        _selectedDisplay = 0
    elseif event.data.button == "DBTN6" then
        _selectedDisplay = 1
    end
end


local function udf_select_color(event)
    local self = assert(event.self, "self not provided")

    if event and event.data and event.data.direction and event.data.knob then

        local cname = "R"
        if event.data.knob == "KNOB4" then
            cname="R"
        elseif event.data.knob == "KNOB5" then
            cname="G"
        elseif event.data.knob == "KNOB6" then
            cname="B"
        end

        if event.data.direction == "CLOCKWISE" then
            _selectedColor[cname] = _selectedColor[cname] + 1
        else
            _selectedColor[cname] = _selectedColor[cname] - 1
        end
        if _selectedColor[cname] < 0 then
            _selectedColor[cname] = 0
        end
        if _selectedColor[cname] > 255 then
            _selectedColor[cname] = 255
        end
        log.info("POS: ",_curPos.X or 1, _curPos.Y or 1, "COL: ", _selectedColor.R or 0, _selectedColor.G or 0, _selectedColor.B or 0)
    end
end


--display
local function udf_draw_color(event)
    local self = assert(event.self, "self not provided")
    local devid = self:getDevice():getId()

    local height = niproto.CONST_DEVICES[devid].dheight
    local width = niproto.CONST_DEVICES[devid].dwidth
    if not height or not width then
        log.error("Unknown height or width for device: ",string.format(devid))
        return
    end

    if not _displayData then
        log.info("udf_draw_color: initializing display data")
        _initDisplayData(height, width)
    end

    local display=_selectedDisplay or 0

    local axis = "X"
    if event.data.knob == "KNOB7" then
        axis = "X"
    elseif event.data.knob == "KNOB8" then
        axis = "Y"
    end

    if event.data.pad == "PAD12" or event.data.pad == "PAD8" or event.data.pad == "PAD9" or event.data.pad == "PAD5" then
        axis = "X"
    elseif event.data.pad == "PAD14" or event.data.pad == "PAD15" or event.data.pad == "PAD2" or event.data.pad == "PAD3" then
        axis = "Y"
    end

    local maxPos = height
    if axis == "X" then
        maxPos = width
    end

    if event.data.pad == "PAD12" or event.data.pad == "PAD8" or event.data.pad == "PAD2" or event.data.pad == "PAD3" then
        event.data.direction = "CLOCKWISE"
    elseif event.data.pad == "PAD9" or event.data.pad == "PAD5" or event.data.pad == "PAD14" or event.data.pad == "PAD15" then
        event.data.direction = "COUNTER_CLOCKWISE"
    end

    if event.data.direction == "CLOCKWISE" then
        _curPos[axis] = _curPos[axis] + 1
    else
        _curPos[axis] = _curPos[axis] - 1
    end
    if _curPos[axis] < 1 then
        _curPos[axis] = 1
    end
    if _curPos[axis] > maxPos then
        _curPos[axis] = maxPos
    end

    local dat = _displayData[display]
    dat[_curPos.Y or 1][_curPos.X or 1] = niproto.getRGB565Color(_selectedColor.R or 0,_selectedColor.G or 0,_selectedColor.B or 0)

    self:sendDataToDisplay(display, dat)

end

--pad-num-led - mk3
local function udf_test_colorPad(event)
    --log.error("test_colorPad1: trying to set color of ",event.data.pad)
    local self = assert(event.self, "self not provided")

    local data = self:getLedData()
    if not data then
        data = niproto.initLedData(self:getDevice():getId())
    end

    local idx = 88 + niproto._pad_num_to_code(event.data.padid)
    local color = 0
    if event.data.state == "PRESSED" then
        color = event.data.padid
    end

    local cpressure = event.data.cpressure or 1
    local intense = cpressure // 32 --128/4

    niproto.setLedColor(data,idx, color,intense)

    self:setLedData(data)
    self:sendLedData(data)

    if event.data.state == "PRESSED" and event.data.cpressure > 63 then
        udf_draw_color(event)
    end
end

--display
local function udf_reset_display(event)
    local self = assert(event.self, "self not provided")
    local devid = self:getDevice():getId()

    log.info("udf_reset_display: ",string.format("0x%x",devid))
    local height = niproto.CONST_DEVICES[devid].dheight
    local width = niproto.CONST_DEVICES[devid].dwidth

    if not height or not width then
        log.error("Unknown height or width for device: ", string.format("0x%x",devid))
        return
    end

    local clear=false
    if not _displayData then 
        log.info("udf_reset_display: initializing display data")
        _initDisplayData(height, width)
    end

    local display=0
    if event.data.button == "DBTN5" or event.data.button == "DBTN8" then
        display=1
    end

    if event.data.button == "DBTN4" or event.data.button == "DBTN8" then
        clear = true
    end

    local dat = _displayData[display]
    if clear then
        local col = niproto.CONST_DISPLAY_COLORS.BLACK
        for y = 1, height do
            for x=1, width do
                dat[y][x] = col
            end
        end

    else
        --test: load image
        local img = _loadPngImage("rebellion-480x272.png")
        for y = 1, height do
            for x=1, width do
                local pixel = img:getPixel(x,y)
                dat[y][x] = niproto.getRGB565Color(pixel.R,pixel.G,pixel.B)
            end
        end

        --test: draw diagonal line on display2
        if display == 1 then
            local x = 50
            local y = 50
            local len = 200
            for i=1,len do
                dat[y+i][y+i] = niproto.getRGB565Color(0,0,0)
            end
        end

    end

    self:sendDataToDisplay(display, dat)

end


local function udf_test_stop()
    os.exit(0)
end

local function udf_toggle_dispatchMapEvents()
    dispatchMapEvents = dispatchMapEvents and false or true
end

--=== register UDF functions which are later called in dispatcher function below
local mapEvents = {
    ["DBTN1/RELEASED"] = udf_reset_display,
    ["DBTN2/RELEASED"] = udf_select_display,
    ["DBTN4/RELEASED"] = udf_reset_display,
    ["DBTN5/RELEASED"] = udf_reset_display,
    ["DBTN6/RELEASED"] = udf_select_display,
    ["DBTN8/RELEASED"] = udf_reset_display,
    ["FIXED_VEL/PRESSED"] = udf_test_SwitchAllKeyLeds,
    ["KNOB1/PRESSED"] = udf_test_resetAllKeyLeds,
    ["KNOB1/RELEASED"] = udf_test_resetAllKeyLeds,
    ["KNOB1/CLOCKWISE"] = udf_test_lightupButton,
    ["KNOB1/COUNTER_CLOCKWISE"] = udf_test_lightupButton,
    ["KNOB2/CLOCKWISE"] = udf_test_lightupButton,
    ["KNOB2/COUNTER_CLOCKWISE"] = udf_test_lightupButton,
    ["KNOB3/CLOCKWISE"] = udf_test_lightupButton,
    ["KNOB3/COUNTER_CLOCKWISE"] = udf_test_lightupButton,
    ["KNOB4/CLOCKWISE"] = udf_select_color,
    ["KNOB4/COUNTER_CLOCKWISE"] = udf_select_color,
    ["KNOB5/CLOCKWISE"] = udf_select_color,
    ["KNOB5/COUNTER_CLOCKWISE"] = udf_select_color,
    ["KNOB6/CLOCKWISE"] = udf_select_color,
    ["KNOB6/COUNTER_CLOCKWISE"] = udf_select_color,
    ["KNOB7/CLOCKWISE"] = udf_draw_color,
    ["KNOB7/COUNTER_CLOCKWISE"] = udf_draw_color,
    ["KNOB8/CLOCKWISE"] = udf_draw_color,
    ["KNOB8/COUNTER_CLOCKWISE"] = udf_draw_color,
    ["STOP/RELEASED"] = udf_test_stop,
    ["CHANNEL/RELEASED"] = udf_toggle_dispatchMapEvents
}
for i=1,16 do
    mapEvents["PAD" .. tostring(i) .. "/PRESSED"] = udf_test_colorPad
    mapEvents["PAD" .. tostring(i) .. "/RELEASED"] = udf_test_colorPad
end

--=== register dispatcher when module is loaded
dispatcher:on('*',function(eventname, data)
    local event = data.data

    if eventname and
        type(event.data) == "table" then
        local lname
        local levent

        if  eventname == "BTN_DATA" and
                event.data.state and
                event.data.button then
                lname = event.data.button
                levent = event.data.state

        elseif eventname == "KNOB_ROTATE" and
                event.data.direction and
                event.data.knob then

                lname = event.data.knob
                levent = event.data.direction
            elseif eventname == "PAD_DATA" and
                event.data.state and
                event.data.pad then

                lname = event.data.pad
                levent = event.data.state
        end

        --channel button can deactivate/activate the "demo" stuff
        if lname and (
            lname == "CHANNEL" or dispatchMapEvents == true) and
            levent and mapEvents[lname .. "/" .. levent] then
                mapEvents[lname .. "/" .. levent](event)
        end

    end

end)
