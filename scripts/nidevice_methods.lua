-- Rebellion
--
-- File: nidevice_methods.lua
-- Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
-- -- License: LGPLv3

local App = require 'App'
local niproto = require 'niproto'
local log = require 'log'
local NIInstance = require 'niinstance'

local dispatcher = require 'dispatcher'()

--=== NIDevice methods ===--
local _M = {}

--=== state handling ===
function _M:resolveStateFunc(state)
    return self["on" .. state]
end

function _M:switchState(state, ...)
    if not state or state == "" then
        return nil,"Error, state not given"
    end
    local _state = state:upper();

    if not self:resolveStateFunc(_state) then
        return nil, "Error, device state func for '" .. _state .. "' not found"
    end

    self:setState(_state)
    self:setStateParams({...} or {})
    return true
end

function _M:proceed()
    log.debug("proceed")
    local state = self:getState()
    if not state then
        return false, "proceed error: no state active"
    end

    local f = self:resolveStateFunc(state)
    if f then
        --print("ok, state function for " .. state .. " defined")
        f(self, table.unpack(self:getStateParams() or {}))
        return true
    else
        log.error("no state func")
        return nil, "Error, state function for '" .. state .. "' undefined"
    end

end

function _M:getInstanceSerials()
    local instances = self:getInstances()
    if not instances then
        instances = {}
        self:setInstances(instances)
    end

    local serials = {}
    for k,_ in pairs(instances) do
        table.insert(serials, k)
    end

    return serials
end

--TODO: add support for multiple instances per device
function _M:getInstance(serial)
    log.info("Trying to get instance for serial " .. serial)
    local instances = self:getInstances()
    local instance = instances[serial]
    if not instance then
        log.info("Creating new instance")
        instance = NIInstance(self, serial)
        instances[serial] = instance
    end

    if not instance then
        return nil, "Error, no instance found and no new instance created"
    end

    return instance
end

function _M:removeInstance(serial)
    local instances = self:getInstances()
    log.info("Removing instance")
    instances[serial] = nil
end

--=== general ===
function _M:print()
    print("Name: " .. self:getName() .. " State: " .. self._state)
end

local function notificationPortCallback(self, data, len)
    log.info(self:getName() .. "> Notification port callback called")
    if not data then
        log.error("No data in notificationPortCallback given!")
        return
    end

    --        log.error("CWD: " .. App.cwd())
    niproto.writedump(self, "D", data, len)

    local res, err = niproto.parseEventMessage(data, len, self)
    if not res and err then
        log.error(err)
        return
    end

    if res and res.name and (res.name == "DEVICE_STATE_ON" or res.name == "DEVICE_STATE_OFF") then

        --print("dispatching event: device.state")
        dispatcher 'device.state'(res.data)
        log.info(self:getName() .. "> DEVICE: ", res.data.serial, "STATE: " .. res.data.state)
        local instance = self:getInstance(res.data.serial)
        if res.data.state == "ON" then
            instance:switchState("init")
        elseif res.data.state == "OFF" then
            instance:switchState("reset")
        else
            instance:switchState("error", "unknown state: " .. res.data.state)
        end
    end

end

--=== state methods ===
function _M:onINIT()
    log.info(self:getName() .. "> onInit")

    --reset session variables
    self:setReqPortName()
    self:setNotifPortName()
    self:setNotifPort()
    self:setReqPort()

    local bport = niproto.openPort(self:getBootstrapPort(),10)
    if not bport then
        log.error(self:getName() .. "> bootstrap port not opened")
        self:switchState("error", "bootstrap port not opened")
        return
    end

    local reqportname, notifportname = niproto.PARSE_PID_CONNECT_RESULT(
        bport:send( niproto.MSG_PID_CONNECT(self:getId() ) )
    )
    bport:close()

    --- connection ---
    if not reqportname or not notifportname then
        log.error(self:getName() .. "> Error getting request or notification port")
        self:switchState("error", "error getting request or notification port")
        return
    end

    log.info(self:getName() .. "> PID-Connect result: ", reqportname, notifportname)
    self:setReqPortName(reqportname)
    self:setNotifPortName(notifportname)

    --notification port creation
    local notifport = niproto.createPort(notifportname, function(data, len)
        notificationPortCallback(self, data, len)
    end, 10)

    if not notifport then
        log.error(self:getName() .. "> Error creating notification port")
        self:switchState("error", "error creating notification port")
        return
    end
    self:setNotifPort(notifport)

    local reqport = niproto.openPort(reqportname, 10)
    if not reqport then
        log.error(self:getName() .. "> Error opening request port: ", reqportname)
        self:switchState("error", "error opening request port", reqportname)
        return
    end
    self:setReqPort(reqport)

    log.info(self:getName() .. "> Sending notification port name: ", notifportname)
    local res = niproto.PARSE_ACK_NOT_PORT_RESULT(
        reqport:push(niproto.MSG_ACK_NOT_PORT(notifportname))
    )
    log.info(self:getName() .. "> reqport MSG_ACK_NOT_PORT result: ", res)

    --send initial devstate msg
    local res = niproto.PARSE_DEVSTATE_RESULT(
        reqport:push(niproto.MSG_DEVSTATE())
    )
    log.info(self:getName() .. "> DEVSTATE result: ", res)
    self:switchState("loop")
end

--TODO: optimize loop(0.1) blocking. this costs some time.
function _M:onLOOP(...)
    local notifport = self:getNotifPort()
    --notifport:loop(0.1)
    notifport:loop(0)
    --assert(self:switchState("halt", "no error"))
end

function _M:onERROR(...)
    log.info("onError called: ", table.unpack({...}))

    local errcnt = self:getErrorCount() or 0
    errcnt = errcnt + 1

    if errcnt > 3 then
        log.error(self:getName() .. "> Max error count of 3 reached. Bailing out")
        os.exit(1)
    end
    self:setErrorCount(errcnt)

    self:switchState("init")
end

function _M:onHALT()
    log.info(self:getName() .. "> HALT")
    App.sleep(999999999)
end

return _M
