-- Rebellion
--
-- File: start.lua
-- Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
-- License: LGPLv3

local App = require "App"
local RebellionTypes = require "RebellionTypes"
local json = require 'cjson'
package.path = package.path .. ";" .. App.PATH_SCRIPTS .. "/?.lua"

--luacheck: push ignore 121
App.realprint = print
function print(...)
    local fp = io.open(App.PATH_LOG, "a")
    if fp then
        local t = {...}
        for idx, val in ipairs(t) do
            t[idx] = tostring(val)
        end
        local txt = "L> " .. table.concat(t, "\t")
        fp:write(txt .. "\n")
        App.realprint(txt)
        fp:close()
    else
        App.realprint("Error: can't open print log file")
        os.exit(1)
    end
end


function get_lua_env(filename, config_env)
    local config_env = config_env or {}
    do
        local function file_exists(name)
            local f=io.open(name,"r")
            if f~=nil then io.close(f) return true else return false end
        end

        if file_exists(filename) then
            local stat, res = pcall(function()
                local config, err = loadfile(filename,"t",config_env)
                if not config then
                    return err
                end
                config()
                return true
            end)

            if stat and type(res) == "string" then
                print("Config error #1: " .. res)
                os.exit(1)
            end

            if not stat and res then
                print("Config error #2: ", res)
                os.exit(1)
            end

        end
    end
    return config_env
end

--luacheck: pop
local log = require "log"
--log.level = "debug"
--log.level = "warn"
log.level = "info"

local NIDevices = require "nidevices"


local devices = {
        "MASCHINE_MK3",
        "MASCHINE_JAM",
        "MASCHINE_MIKRO_MK3",
        "MASCHINE_MIKRO_MK2",
        "KOMPLETE_KONTROL_MK1",
        "KOMPLETE_KONTROL_MK2",
        "KOMPLETE_KONTROL_M",
        "KOMPLETE_KONTROL_A"
}

local config_env = get_lua_env("config.lua")
if config_env.devices then
    devices = config_env.devices
end

if config_env.loglevel then
    log.level = config_env.loglevel
end
--=== main code ===--
assert(
    NIDevices:activate(devices)
)

--TODO: implement string.match in dispatcher: ev:match("^/%a+/foo/bar")
--or implement table matching (if event listener name is table, match for all which is given (serial? event? ...))
--when specific data is given in the event data

--import rpc function
rpc = require 'rpc'

function start()
    --initialize udf functions
    require 'udf'

    local dispatcher = require 'dispatcher'()
    dispatcher:on('*',function(event, data)

        --print("dispatched!")
        --calling rpc callback if set
        if App.rpc_callback() then

            local stat, res = pcall(function()
                return json.encode {
                    event = event,
                    data = data.data or {}
                }
            end)

            if not stat then
                log.debug("Error calling json.encode: ", (res or "-"))
                return
            end
            --TODO: check for encoding errors
            App.rpc_callback(
                        RebellionTypes.REBELLION_MF_JSON,
                        RebellionTypes.REBELLION_MT_EV,
                        res,
                        res:len())

        end
    end)

end

start()
print("start.lua: Done.")
