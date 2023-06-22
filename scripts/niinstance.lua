-- Rebellion
--
-- File: niinstance.lua
-- Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
-- License: LGPLv3

local niproto = require 'niproto'
local log = require 'log'

--=== NIInstance module ===--
local _methods = require 'niinstance_methods'

-- generator function
local function new(device, serial)
    if not device or not serial then
        return nil, "Error, device or serial missing"
    end

    log.info("Adding instance handler for:" .. device:getName() .. ":" .. serial)

    local instance = {}

    local hidden = {
    }

    local meta = {
        __newindex = function(_,k)
                log.error("Error, readonly: " .. k)
                os.exit(1)
            end,
        __index = function(_,k)

            if _methods[k] then
                return _methods[k]
            end

            local prefix = k:sub(1,3)
            local varname = k
            if prefix == "get" or prefix == "set" then --access hidden stuff
                varname = varname:sub(4,4):lower() .. varname:sub(5)
            end
            if prefix == "get" then
                return function()
                    return hidden[varname]
                end
            elseif prefix == "set" then
                return function(t, value)
                    assert(t == instance, "Error, not called as class instance")
                    hidden[varname] = value
                end
            end

            return
        end,
        __pairs = function(t)
            return next, _methods
        end
    }

    log.debug("Setting metatable")
    setmetatable(instance, meta)

    --defaults
    log.debug("Setting defaults")
    instance:setDevice(device)
    instance:setSerial(serial)
    instance:setState("INIT")

    log.debug("Returning device instance")
    return instance
end

return setmetatable({}, {
    __call = function(_, device, serial)
        return new(device, serial)
    end
})
