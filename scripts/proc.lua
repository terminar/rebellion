-- Rebellion
--
-- File: proc.lua
-- Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
-- License: LGPLv3

local stepcnt = 0
local prevtime = 0
local nowtime = 0

local log = require "log"

function run()
    log.debug("run() start")

    log.debug("proc.lua: run_platform")
    local stat, err = pcall(function()
        local NIDevices = require "nidevices"
        NIDevices:proceed()
        --local App = require "App"
        --App.sleep(0.1)
    end)

    log.debug("stat:", stat)
    if not stat then
        log.error("unknown error: ", err or "-")
        os.exit(1)
    end
    if not stat and err then
        log.error(err)
        os.exit(1)
    end
    log.debug("run() end")
end

--finish()
log.info("load proc.lua")
