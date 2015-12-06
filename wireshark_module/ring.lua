--Sketch of Wireshark script that is adding handling of #RING protocol.

--[[ 
	RUNNING SCRIPT 
	1. Get into directory containing this script.
	2. type: wireshark -X lua_script:ring.lua
--]]

local debug = true

--Enumerations used to identify messages.
local MessageType = {
    UNDEFINED = 0,
    GET = 1,

    CREATE_CATEGORY = 2,
    DESTROY_CATEGORY = 3,
    CATEGORY_LIST = 4,
    JOIN_CATEGORY = 5,
    LEFT_CATEGORY = 6,
    SIGN_UP_CATEGORY = 7,
    SIGN_OUT_CATEGORY = 8,
    RING_MESSAGE = 9,
    NEIGHBOURS_SET = 10,
    SERVER_INFO = 11
}

local GetMessageType = {
    CAT_LIST = 1,
    NEIGHBOURS = 2
}
local ServerInfoMessageType = {
    OK = 0,
    FAIL = 1,
    PERMISSION_DENIED = 2
}

--Creating a Proto object.
local ring_proto = Proto("ring", "#RING application protocol.")

--This function will be called by Wireshark at initialisation.
function ring_proto.init()
end

--Function for dissecting messages.
function ring_proto.dissector(tvbuf, pktinfo, root)
   if debug == true then
	print("dissector() called.")
   end
   local tree = root:add(ring_proto, tvbuf:range(offset, length_val))
end

--Registering protocol
DissectorTable.get("tcp.port"):add(2137, ring_proto)
