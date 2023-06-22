-- Rebellion
--
-- File: nidevice.lua
-- Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
-- License: LGPLv3

local niproto = require 'niproto'
local log = require 'log'

--=== NIDevice module ===--
local _methods = require 'nidevice_methods'

-- generator function
local function new(name)
    local pdata = niproto.getDevice(name)
    if not pdata then
        return nil, "Error, device " .. name .. " not found"
    end
    log.info("Adding device handler for:" .. name)

    local dev = {}

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
                    assert(t == dev, "Error, not called as class instance")
                    hidden[varname] = value
                end
            end

            --fallback - it's nil
            return
        end,
        __pairs = function(t)
            return next, _methods
        end
    }

    log.debug("Setting metatable")
    setmetatable(dev, meta)

    --defaults
    log.debug("Setting defaults")
    dev:setName(name)
    dev:setId(#pdata)
    dev:setBootstrapPort(pdata.port)
    dev:setState("INIT")
    dev:setInstances({})
    dev:setDevice(dev) --compatibility loopback to make things easier (using :getDevice())

    log.debug("Returning device")
    return dev
end

return setmetatable({}, {
    __call = function(_, name)
        return new(name)
    end
})
