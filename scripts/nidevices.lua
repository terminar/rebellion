-- Rebellion
--
-- File: nidevices.lua
-- Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
-- License: LGPLv3

local NIDevice = require "nidevice"
local log = require "log"

-- local storage
local _devices = {}

--NIDevices module
local _M = {}

function _M:activate(device)
    if not device then
        log.error("Activate: no device given")
        return false
    end

    --map table to multiple calls
    if type(device) == "table" then
        local res = nil
        for _, p in pairs(device) do
            res = assert(_M:activate(p))
        end
        return res
    end

    log.info("Activating device " .. device)
    _devices[device] = assert(NIDevice(device))
    return true
end

function _M:getDevices()
    return _devices
end

function _M:getInstances(filter)
    local instances = {}
    for _, dev in pairs(_M:getDevices()) do
        for iname, instance in pairs(dev:getInstances()) do
            --log.info("INAME: ", iname)
            if filter and type(filter) == "table" then
                for skey,sval in pairs(filter) do
                    if instance[skey] == nil or instance[skey] ~= sval then
                        break
                    end
                end
            end
            instances[iname] = instance
        end
    end

    return instances
end

function _M:proceed()
    log.debug("nidevices: calling proceed")
    for k, v in pairs(_M:getDevices()) do
        log.debug("Executing proceed for: " .. k .. " > " .. v:getState())
        assert(v:proceed())

        log.debug("nidevices: device instances proceed")
        for w, x in pairs(v:getInstances()) do
            log.debug("Executing proceed for: " .. x:getDevice():getName() .. ":" .. w .. " > " .. x:getState())
            assert(x:proceed())
        end
    end
    log.debug("nidevices: done proceeding")
    return true
end

return _M
