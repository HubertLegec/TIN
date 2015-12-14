--Wireshark script that is adding handling of #RING protocol.

--[[ 
	RUNNING SCRIPT 
	1. Get into directory containing this script.
	2. type: wireshark -X lua_script:ring.lua
	3. Capture packets.
	4. Right-click on one of captured packets.
	5. Select "Decode as..."
	6. Find "RING" protocol.
	7. Click apply.
--]]
local debug = true

--Enumerations used to identify messages.
local messageTypeNames = {}
messageTypeNames[0] = "UNDEFINED"
messageTypeNames[1] = "GET"
messageTypeNames[2] = "CREATE_CATEGORY"
messageTypeNames[3] = "DESTROY_CATEGORY"
messageTypeNames[4] = "CATEGORY_LIST"
messageTypeNames[5] = "JOIN_CATEGORY"
messageTypeNames[6] = "LEFT_CATEGORY"
messageTypeNames[7] = "SIGN_UP_CATEGORY"
messageTypeNames[8] = "SIGN_OUT_CATEGORY"
messageTypeNames[9] = "RING_MESSAGE"
messageTypeNames[10]=  "NEIGHBOURS_SET"
messageTypeNames[11]=  "SERVER_INFO"



--Creating a Proto object.
local ring_proto = Proto("ring", "#RING application protocol.")

--Simple message hdr.
local hdr_fields =
{
    msg_type   		= ProtoField.int32 ("ring.type", "Type", base.DEC),
    msg_len   		= ProtoField.int32("ring.length", "Length", base.DEC),
    msg_sender_id  	= ProtoField.int64 ("ring.senderID", "Sender ID", base.DEC),

}

ring_proto.fields = hdr_fields

local simple_header_length = 12

function ring_proto.init()
    tvbs = {}
end

--Function for dissecting messages.
function ring_proto.dissector(tvbuf, pktinfo, root)
   if debug == true then
	print("ring_proto.dissector() called.")
   end

    -- get the length of the packet buffer.
    local pktlen = tvbuf:len()

    local bytes_consumed = 0

    while bytes_consumed < pktlen do

     
        local result = dissectRING(tvbuf, pktinfo, root, bytes_consumed)

        if result > 0 then

            bytes_consumed = bytes_consumed + result

        elseif result == 0 then
            -- This value of result means that an error ocurred. By returning 0 we inform 
	    -- Wireshark that this message is not for us.
            return 0
        else
            -- setting the desegment_offset to what we already consumed.
            pktinfo.desegment_offset = bytes_consumed

            -- how many additional bytes we need.
            result = -result
            pktinfo.desegment_len = result

            -- returning number of processed by this dissector bytes (in this case all).
            return pktlen
        end        
    end

    return bytes_consumed
end

dissectRING = function (tvbuf, pktinfo, root, offset)

    if debug == true then print("dissectRING called.") end

    local length_val, length_tvbr = checkLength(tvbuf, offset)

    if length_val <= 0 then
        return length_val
    end

    -- Whole message is in the buffer - Dissection

    -- set the protocol column to show our protocol name

   pktinfo.cols.protocol:set("RING")
    
    -- We start by adding our protocol to the dissection display tree.
    local tree = root:add(ring_proto, tvbuf:range(offset, length_val))

    -- dissect type
    tree:add_le(hdr_fields.msg_type, tvbuf:range(offset, 4))
    local msg_type  = tvbuf:range(offset, 4):le_int()

    -- set the info column to show our protocol type
    if messageTypeNames[msg_type] ~= nil then
	        pktinfo.cols.info:set(messageTypeNames[msg_type])
    end


    -- dissect the length field
    tree:add_le(hdr_fields.msg_len, length_tvbr)

    -- dissect id
    tree:add_le(hdr_fields.msg_sender_id, tvbuf:range(offset + 8, 8))

    return length_val
end

-- The function to check the length field.
checkLength = function (tvbuf, offset)

    -- "msglen" is the number of bytes remaining in the Tvb buffer which we
    -- have available to dissect in this run
    local msglen = tvbuf:len() - offset

    --The TCP packet might be cut-off, because the user set Wireshark to limit the size of packets being captured, in this case packet is ignored.
    if msglen ~= tvbuf:reported_length_remaining(offset) then
        if debug == true then print("cutoff.") end
        return 0
    end

    --In this case, we do not have enough byted of data  to find out the length of message.
    if msglen < simple_header_length then
        if debug == true then print("Need more bytes to figure out messsage length field") end
        return -DESEGMENT_ONE_MORE_SEGMENT
    end

    --Figuring out total length of messagr
    local length_tvbr = tvbuf:range(offset + 4, 4)
    local length_val  = length_tvbr:le_int()

	if debug == true then print(length_val) end
	if debug == true then print(msglen) end

    if msglen < length_val then
        -- we need more bytes to get the whole FPM message
        if debug == true then print("Need more bytes to to get the whole message") end
        return -(length_val - msglen)
    end

    if debug == true then print("OK length") end

    return length_val, length_tvbr
end

--Registering protocol
DissectorTable.get("tcp.port"):add(2137, ring_proto)
