-- MIT License

-- Copyright (c) 2020 Jim Schmid <jim.schmid@1up.io>

-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:

-- The above copyright notice and this permission notice shall be included in all
-- copies or substantial portions of the Software.

-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.

-- lua-event-dispatcher-0.3-1
-- Modified version, all in one module <terminar@cyberphoria.org>

--[[example:
local dispatcher = require 'dispatcher'()

local listener = function (e)
    e.data.called = true
end

dispatcher:on("some-event", listener)
local event = { called = false }
dispatcher:dispatch("some-event", event)

-- syntactic sugar to dispatch an event
dispatcher 'some-event' {
    foo = "bar"
}

--]]


local Event = {}

local EventMethods = {}
function EventMethods:stopPropagation()
    self.isPropagationStopped = true
end

--luacheck: ignore self
function Event:new (event, data)
    data = data or {}

    local state = {
        isDispatched = false,
        isPropagationStopped = false,

        event = event,
        data = data
    }

    local meta = {
        __index = function(t,k)
            if EventMethods[k] then
                return EventMethods[k]
            end
            return t
        end
    }

    return setmetatable(state, meta)
end

---------
local DispatcherMethods = {}


-- Add a new listener to the dispatcher
--
-- @param string eventName
-- @param callable listener
--
-- @return nil
function DispatcherMethods:addListener(eventName, listener, priority)
    -- set a default priority if nothing is provided
    priority = priority or 0

    assert(type(listener) == "function" or type(listener) == "table", "A registered listener must be callable")
    assert(type(priority) == "number", "priority must be a number")

    if self.listeners[eventName] == nil then
        self.listeners[eventName] = {}
    end

    if self.listeners[eventName][priority] == nil then
        self.listeners[eventName][priority] = {}
    end

    local list = self.listeners[eventName][priority]

    table.insert(list, listener)
end

-- Add a new listener to the dispatcher
--
-- @param string eventName
-- @param callable listener
--
-- @return nil
DispatcherMethods.on = DispatcherMethods.addListener

-- Remove a specific listener from the table
--
-- @param string eventName
-- @param callable listener
--
-- @return nil
function DispatcherMethods:removeListener(eventName, listener)
    local priorityQueues = self.listeners[eventName]

    for _, priorityQueue in pairs(priorityQueues) do
        for key, registeredListener in pairs(priorityQueue) do
            if registeredListener == listener then
                table.remove(priorityQueue, key)
            end
        end
    end
end

-- Remove all listeners for a given event
--
-- @param string eventName
-- @param callable listener
--
-- @return nil
function DispatcherMethods:removeListeners(eventName)
    if self.listeners[eventName] == nil then
        return
    end

    self.listeners[eventName] = {}
end

-- Remove all listeners from the dispatcher
--
-- @param string eventName
-- @param callable listener
--
-- @return nil
function DispatcherMethods:removeAllListeners()
    self.listeners = {}
end

-- Get an ordered list of listeners listening to a specific event
--
-- @param string eventName
--
-- @return table A list of listeners
function DispatcherMethods:getListeners(eventName)

    local priorityQueues = self.listeners[eventName] or {}

    local listeners = {}
    local keys = {}

    for priority in pairs(priorityQueues) do
        table.insert(keys, priority)
    end

    -- reverse iteration over priority keys
    -- this way a priority of 0 will be executed before higher priorities
    for i = #keys, 1, -1 do
        local priority = keys[i]

        for _, registeredListener in pairs(priorityQueues[priority]) do
            table.insert(listeners, registeredListener)
        end
    end

    return listeners
end


-- Dispatch an event, preferably with an event object
-- but it is possible to dispatch with any kind of table as an event
--
-- @param string eventName
-- @param mixed eventdata
--
-- @return nil
function DispatcherMethods:dispatch(name, eventdata)
    local event = Event:new(name, eventdata or {})

    --temp hack
    local listeners = self:getListeners('*')
    if #listeners == 0 then
        listener = self:getListeners(name)
    end
    --[[ --match example. param: data, match
        for k,v in pairs(self.sessions) do
            equal=true; --replace with break?
            if match then
                for w,x in pairs(t) do
                    if v[w] == nil or v[w]:match(x) == nil then
                        equal=false;
                    end
                end
            else
                for w,x in pairs(t) do
                    if v[w] == nil or v[w] ~= x then
                        equal=false;
                    end
                end
            end
            if equal then
                tinsert(res,k);
            end
        end
    --]]

    for _, listener in pairs(listeners) do
        self.executor(listener, name, event)

        if type(event) == "table" and event.isPropagationStopped then
            break
        end
    end

    if (type(event) == "table") then
        event.isDispatched = true
    end
end

-- Dispatcher ==================================================================
local _M = {}

-- Executor functions
function _M.directExecutor(listener, name, event)
    listener(name, event)
end

function _M.protectedExecutor(listener, name,  event)
    pcall(listener, name, event)
end


-- Create a new Dispatcher object
function _M.new(executor)
    executor = executor or _M.directExecutor

    assert(type(executor) == "function" or type(executor) == "table", "An executor must be a callable")

    local state = {
        listeners = {},
        usepattern = true,
        executor = executor
    }

    local meta = {
        __index = function(t,k)
            if DispatcherMethods[k] then
                return DispatcherMethods[k]
            end
            return rawget(t,k)
        end,
        __pairs = function(t) --luacheck: ignore unused t
            return next, DispatcherMethods
        end,
        __call = function(t, event)
            return function(...)
                return DispatcherMethods.dispatch(t, event, table.unpack {...})
            end
        end
    }

    return setmetatable(state, meta)
end

local _instances = {}
local _Mmeta = {
    __call = function(_, name)
        local name = name or "default" --luacheck: no redefined
        if not _instances[name] then
            _instances[name] = _M.new()
        end
        return _instances[name]
    end
}

return setmetatable(_M, _Mmeta)
