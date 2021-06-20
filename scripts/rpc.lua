-- Rebellion
--
-- File: rpc.lua
-- Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020,2021
-- License: closed

local niproto = require 'niproto'
local log = require 'log'
local json = require 'cjson'
local RebellionTypes = require "RebellionTypes"
local App = require "App"

local rpcfuncs = {}

local function addRpc(name)
    return function(func)
        rpcfuncs[name] = func
    end
end

addRpc 'rebellion.activateDevice' (function(name)
    log.error("DUMMY")
    return -1
end)

addRpc 'rebellion.getDevices' (function()
    local devices = niproto.CONST_DEVICES
    local res = {}
    for k,v in pairs(devices) do
        table.insert(res,k)
    end
    return res
end)

addRpc 'rebellion.getInstances' (function()
    local instances = {}
    for iname, instance in pairs(require 'nidevices':getInstances()) do
        table.insert(instances, {
            name = iname,
            device = instance:getDevice():getName()
        })
    end
    return instances
end)


addRpc 'rebellion.sendDataToDisplay' (function(serial, display, data)
    local instances = require 'nidevices':getInstances{ serial = serial } or {}
    local _,self = next(instances)

    if not self then
        return nil, "no instance found"
    end
    if not data then
        return nil, "no display data given"
    end

    local warr = {}
    if type(data) == "table" and type(data[1]) == "table" then
        local devid = self:getDevice():getId()
        local height = niproto.CONST_DEVICES[devid].dheight
        local width = niproto.CONST_DEVICES[devid].dwidth
        if not height or not width then
            return nil, "No height or width given"
        end

        local dat = data
        --[[
        --create display data table, take the data
       local dat = {}
        for y = 1,height do
            for x = 1,width do
                if not dat[y] then dat[y] = {} end

                --set black if empty
                dat[y][x] = (data[y] and data[y][x]) or niproto.CONST_DISPLAY_COLORS.BLACK
            end
        end
        --]]

        --generate pixel stream
        local tinsert = table.insert
        for y = 1, height do
            for x = 1, width do
                tinsert(warr, dat[y][x])
            end
        end

    else
        warr = data
    end
--[[
    local img = _loadPngImage("skull-480x272.png")
    for y = 1, height do
        for x=1, width do
            local pixel = img:getPixel(x,y)
            dat[y][x] = _getR565Color(pixel.R,pixel.G,pixel.B)
        end
    end
--]]
--[[
        --test: draw diagonal line on display2
        if display == 1 then
            local x = 50
            local y = 50
            local len = 200
            for i=1,len do
                dat[y+i][y+i] = niproto.gwetR565Color(0,0,0)
            end
        end
--]]

    self:sendDataToDisplay(display, warr)
end)

addRpc 'rebellion.sendLedData' (function(serial, led, color, intense)
    local instances = require 'nidevices':getInstances{ serial = serial } or {}
    local _,self = next(instances)

    if not self then
        return nil, "no instance found"
    end

    local data = self:getLedData()
    if not data then
        data = niproto.initLedData(self:getDevice():getId())
    end

    --[[ pad
    local idx = 88 + niproto._pad_num_to_code(event.data.padid)
    local color = 0
    if event.data.state == "PRESSED" then
        color = event.data.padid
    end

    local cpressure = event.data.cpressure or 1
    local intense = cpressure // 32 --128/4

    niproto.setLedColor(data,idx, color,intense)
    --]]

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

    --[[
    for i=1,#data do
        data[i] = 0
    end
    --]]
    niproto.setLedColor(data,led,color,intense)

    self:setLedData(data)
    self:sendLedData(data)

end)

local function rpcReturn(result)
    if type(result) ~= "table" then
        return -1
    end
    --log.debug("rpcReturn:",tostring(result))
    local s = json.encode(result)
    --log.debug("returning data: ", s)
    local mt = result.event and RebellionTypes.REBELLION_MT_EV
                            or RebellionTypes.REBELLION_MT_RES

    log.info("Result: ", s:sub(1,80) .. (s:len() > 80 and "..." or ""))
    App.rpc_callback(
            RebellionTypes.REBELLION_MF_JSON,
            mt,
            s, s:len()
        )
    log.debug("End of lua => rpc")
end

local function rpc(mf, mt, data, len)
    log.debug("RPC called: ", data, len)

    if mf ~= RebellionTypes.REBELLION_MF_JSON then
        log.error("Error, given message format unsupported. Please use REBELLION_MF_JSON")
        os.exit(1)
    end

    local stat, res = pcall(function()
        return json.decode(data)
    end)
    if not stat and res then
        return rpcReturn {
            error = {
                code = -32700,
                message = "Parse error"
            }
        }
    end
    local rpc = res
    local result = {
        id = rpc.id
    }
    if not rpc.method then
        result.error = {
            code = -32600,
            message = "Invalid Request: method not given"
        }
        return rpcReturn(result)
    end

    if not rpc.params or rpc.params == json.null then
        result.error = {
            code = -32600,
            message = "Invalid Request: no parameter given"
        }
        return rpcReturn(result)
    end

    if not rpcfuncs[rpc.method] then
        result.error = {
            code = -32601,
            message = "Method not found"
        }
        return rpcReturn(result)
    end

    if type(rpc.params) ~= "table" then
        result.error = {
            code = -32602,
            message = "Invalid params"
        }
        return rpcReturn(result)
    end

    log.info("rpc.method: ",rpc.method or "-")
    log.info("rpc.params: ", table.unpack(rpc.params or {}))
    log.info("rpc.id: ", rpc.id or "-")

    local res, err = rpcfuncs[rpc.method](table.unpack(rpc.params))
    if not res and err then
        log.error("Error calling rpc.method " .. rpc.method .. ": ", err or "-")
        result.error = {
            code = -32603,
            message = "Internal error",
            data = err
        }
        return rpcReturn(result)
    end

    result.result = res
    return rpcReturn( result )
end

return rpc
