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
local debug = false

--Enumerations used to identify messages.
local message_type_names = {
[0] = "UNDEFINED",
[1] = "GET",
[2] = "CREATE_CATEGORY",
[3] = "DESTROY_CATEGORY",
[4] = "CATEGORY_LIST",
[5] = "JOIN_CATEGORY",
[6] = "LEFT_CATEGORY",
[7] = "SIGN_UP_CATEGORY",
[8] = "SIGN_OUT_CATEGORY",
[9] = "RING_MESSAGE",
[10]=  "NEIGHBOURS_SET",
[11]=  "SERVER_INFO",
[12]=  "USER_SIGN_UP",
[13]=  "USER_DISCONNECTED",
}

local get_message_type_names = {
[1] = "CAT_LIST",
[2] = "NEIGHBOURS"
}

local server_info_message_type_names = {
[0] = "OK",
[1] = "FAIL",
[2] = "PERMISSION_DENIED"
}


--Creating a Proto object.
local ring_proto = Proto("ring", "#RING application protocol.")

--Fields that can be part of RING message.
local ring_fields =
{
    	msg_type   		= ProtoField.int32 ("ring.type", "Type", base.DEC),
    	msg_len   		= ProtoField.int32("ring.length", "Length", base.DEC),
    	msg_sender_id  		= ProtoField.int64 ("ring.sender_id", "Sender ID", base.DEC),
    
    	get_message_request_type = ProtoField.int32("ring.getMessage.requestType", "Request type", base.DEC),

	category_management_message_category_id = ProtoField.int64 ("ring.category_management_message.category_id", "Category ID", base.DEC),
	category_management_message_category_name = ProtoField.new("Category name", "ing.category_management_message.category_name", ftypes.STRING),
	category_management_message_user_id = ProtoField.int64 ("ring.category_management_message.user_id", "User ID", base.DEC),
	category_management_message_user_name = ProtoField.new("User name", "ring.category_management_message.user_name", ftypes.STRING),

	category_list_message_categories = ProtoField.new("Categories","ring.category_list_message.categories", ftypes.BYTES),

	ring_message_category_id = ProtoField.int64 ("ring.ring_message.category_id", "Category ID", base.DEC),
	ring_message_text = ProtoField.new("Message text", "ring.ring_message.text", ftypes.STRING),
	ring_message_confirmations = ProtoField.new("Confirmations", "ring.ring_message.confirmations", ftypes.BYTES),

	neighbour_set_category_id = ProtoField.int64 ("ring.neighbour_set.category_id", "Category ID", base.DEC),
	neighbour_set_l_neighbour_name = ProtoField.new("Left neighbour name", "ring.neighbour_set.l_neighbour_name", ftypes.STRING),
	neighbour_set_l_neighbour_ip = ProtoField.new("Left neighbour ip", "ring.neighbour_set.l_neighbour_ip", ftypes.STRING),
	neighbour_set_l_neighbour_port = ProtoField.int32 ("ring.neighbour_set.l_neighbour_port", "Left neighbour port", base.DEC),
	neighbour_set_r_neighbour_name = ProtoField.new("Right neighbour name", "ring.neighbour_set.r_neighbour_name", ftypes.STRING),
	neighbour_set_r_neighbour_ip =  ProtoField.new("Right neighbour ip", "ring.neighbour_set.r_neighbour_ip", ftypes.STRING),
	neighbour_set_r_neighbour_port = ProtoField.int32 ("ring.neighbour_set.r_neighbour_port", "Right neighbour port", base.DEC),

	server_info_type = ProtoField.int32 ("ring.server_info.type", "Info type", base.DEC),
	server_info_extra_info = ProtoField.int64 ("ring.server_info.extra_info", "Extra info", base.DEC),
	server_info = ProtoField.new("Info", "ring.server_info.info", ftypes.STRING),

	user_management_message_port = ProtoField.int32 ("ring.user_management_message.port", "Port", base.DEC),
	user_management_message_ip = ProtoField.new("Ip", "ser_management_message_.ip", ftypes.STRING),
	user_management_message_user_name = ProtoField.new("User name", "ser_management_message_.user_name", ftypes.STRING),

}

ring_proto.fields = ring_fields

--Minimal length of message in the buffer to figure out, what is the actual length of message.
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

	--If length is not greater than zero, it means that we cannot dissect message, because of error of lack of full message content.
    	if length_val <= 0 then
        	return length_val
    	end


    	-- set the protocol column to show our protocol name
   	pktinfo.cols.protocol:set("RING")
    
    	-- We start by adding our protocol to the dissection display tree.
    	local tree = root:add(ring_proto, tvbuf:range(offset, length_val))

   	 -- dissect type
	local msg_type  = tvbuf:range(offset, 4):le_int()
    	tree:add_le(ring_fields.msg_type, tvbuf:range(offset, 4)):append_text(", meaning: " .. message_type_names[msg_type])


    	-- set the info column to show our protocol type
    	if string.find(tostring(pktinfo.cols.info), "^messages:") == nil then
            pktinfo.cols.info:set("messages:")
	end

        pktinfo.cols.info:append(" " .. message_type_names[msg_type] .. ",")
  
    	-- dissect the length field
    	tree:add_le(ring_fields.msg_len, length_tvbr)
	
    	-- dissect id
    	tree:add_le(ring_fields.msg_sender_id, tvbuf:range(offset + 8, 8))

	local inner_offset = offset + 16;

	-- Handling diffrent message types.
	if msg_type == 1 then
		
		local request_type = tvbuf:range(inner_offset, 4):le_int()
		tree:add_le(ring_fields.get_message_request_type, tvbuf:range(inner_offset, 4)):append_text(", meaning: " .. get_message_type_names[request_type])

	elseif msg_type == 2 or msg_type == 3 or msg_type == 5 or msg_type == 6 or msg_type == 7 or msg_type == 8 then
		
		tree:add_le(ring_fields.category_management_message_category_id, tvbuf:range(inner_offset, 8))
		inner_offset = inner_offset + 8

		tree:add_le(ring_fields.category_management_message_user_id, tvbuf:range(inner_offset, 8))
		inner_offset = inner_offset + 8

		local category_name = getString(tvbuf, inner_offset);	inner_offset = inner_offset + 8
		tree:add(ring_fields.category_management_message_category_name, tvbuf:range(inner_offset, category_name:len()), category_name)
		inner_offset = inner_offset  + category_name:len()

		local user_name = getString(tvbuf, inner_offset); inner_offset = inner_offset + 8
		tree:add(ring_fields.category_management_message_user_name, tvbuf:range(inner_offset, user_name:len()), user_name)

	elseif msg_type == 4 then

		local map_size = tvbuf:range(inner_offset, 8):le_int64():tonumber(); inner_offset = inner_offset + 8
		local start_offset = inner_offset
		local map_contents = {}

		while map_size > 0 do

			local key = tostring( tvbuf:range(inner_offset, 8):le_int64() ); inner_offset = inner_offset + 8
			local value = getString(tvbuf, inner_offset); inner_offset = inner_offset + 8 + value:len()
			
			table.insert(map_contents, key .. "->" .. value)
			map_size = map_size-1	
		end

		local map_representation = table.concat(map_contents, "\n") 

		tree:add(ring_fields.category_list_message_categories, tvbuf:range(start_offset, inner_offset-start_offset)):append_text("\n[readable_representation] \n" .. map_representation)

	elseif msg_type == 9 then
		tree:add_le(ring_fields.ring_message_category_id, tvbuf:range(inner_offset, 8))
		inner_offset = inner_offset + 8

		local message_text = getString(tvbuf, inner_offset); inner_offset = inner_offset + 8
		tree:add(ring_fields.ring_message_text, tvbuf:range(inner_offset, message_text:len()), message_text); inner_offset = inner_offset + message_text:len()

		local vector_size = tvbuf:range(inner_offset, 8):le_int64():tonumber(); inner_offset = inner_offset + 8
		local start_offset = inner_offset
		local vector_contents = {}

		while vector_size > 0 do

			local confirmation = getString(tvbuf, inner_offset); inner_offset = inner_offset + 8 + confirmation:len()
			
			table.insert(vector_contents, confirmation)
			vector_size = vector_size-1	
		end

		local vector_representation = table.concat(vector_contents, ", ") 
		tree:add(ring_fields.ring_message_confirmations, tvbuf:range(start_offset, inner_offset-start_offset)):append_text("\n	readable representation:" .. vector_representation)

	elseif msg_type == 10 then
		tree:add_le(ring_fields.neighbour_set_category_id, tvbuf:range(inner_offset, 8))
		inner_offset = inner_offset + 8

		local l_neighbour_name = getString(tvbuf, inner_offset); inner_offset = inner_offset + 8
		tree:add(ring_fields.neighbour_set_l_neighbour_name, tvbuf:range(inner_offset, l_neighbour_name:len()), l_neighbour_name)
		inner_offset = inner_offset  + l_neighbour_name:len()

		local l_neighbour_ip = getString(tvbuf, inner_offset); inner_offset = inner_offset + 8
		tree:add(ring_fields.neighbour_set_l_neighbour_ip, tvbuf:range(inner_offset, l_neighbour_ip:len()), l_neighbour_ip)
		inner_offset = inner_offset  + l_neighbour_ip:len()

		tree:add_le(ring_fields.neighbour_set_l_neighbour_port, tvbuf:range(inner_offset,4)); inner_offset = inner_offset + 4

		local r_neighbour_name = getString(tvbuf, inner_offset); inner_offset = inner_offset + 8
		tree:add(ring_fields.neighbour_set_r_neighbour_name, tvbuf:range(inner_offset, r_neighbour_name:len()), r_neighbour_name)
		inner_offset = inner_offset  + r_neighbour_name:len()

		local r_neighbour_ip = getString(tvbuf, inner_offset); inner_offset = inner_offset + 8
		tree:add(ring_fields.neighbour_set_r_neighbour_ip, tvbuf:range(inner_offset, r_neighbour_ip:len()), r_neighbour_ip)
		inner_offset = inner_offset  + r_neighbour_ip:len()

		tree:add_le(ring_fields.neighbour_set_r_neighbour_port, tvbuf:range(inner_offset,4)); inner_offset = inner_offset + 4

	elseif msg_type == 11 then

		local info_type =  tvbuf:range(inner_offset,4):le_int()
		tree:add_le(ring_fields.server_info_type, tvbuf:range(inner_offset,4)):append_text(", meaning: " .. server_info_message_type_names[info_type])
	        inner_offset = inner_offset + 4

		tree:add_le(ring_fields.server_info_extra_info, tvbuf:range(inner_offset,8)); inner_offset = inner_offset + 8
		
		local server_info = getString(tvbuf, inner_offset); inner_offset = inner_offset + 8
		tree:add(ring_fields.server_info, tvbuf:range(inner_offset, server_info:len()), server_info)
		inner_offset = inner_offset  + server_info:len()

	elseif msg_type == 12 or msg_type == 13 then
		
		tree:add_le(ring_fields.user_management_message_port, tvbuf:range(inner_offset, 4))
		inner_offset = inner_offset + 4

		
		local ip_val = getString(tvbuf, inner_offset); inner_offset = inner_offset + 8
		tree:add(ring_fields.user_management_message_ip, tvbuf:range(inner_offset, ip_val:len()), ip_val)
		inner_offset = inner_offset  + ip_val:len()

		local user_name = getString(tvbuf, inner_offset); inner_offset = inner_offset + 8
		tree:add(ring_fields.user_management_message_user_name, tvbuf:range(inner_offset, user_name:len()), user_name)
		inner_offset = inner_offset  + user_name:len()
	
	end


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
DissectorTable.get("tcp.port"):add(8888, ring_proto)

--Function that helps in parsing strings from buffer contents.
getString = function (tvbuf, offset)

    	local string_length  = tvbuf:range(offset, 8):le_int64()
	offset = offset + 8 

	local string_value   = tvbuf:range(offset, string_length:tonumber()):string()
	offset = offset + string_length

	return string_value
end 




