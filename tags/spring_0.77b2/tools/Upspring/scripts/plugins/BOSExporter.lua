
--[[ BOS Exporter, attempts to export some BOS texts with animation commands ]]

local function OutputVector3Anim(propInfo,f)
	local pos1 = upsAnimGetVector3Key(propInfo.prop, propInfo.curkey)
	local pos2 = upsAnimGetVector3Key(propInfo.prop, propInfo.curkey+1)
	local dif = pos2-pos1

	local interval = upsAnimGetKeyTime(propInfo.prop, propInfo.curkey+1) - upsAnimGetKeyTime(propInfo.prop, propInfo.curkey)

	if dif.x ~= 0 then f:write(string.format("Move %s to x-axis %d speed[%d];\n", propInfo.obj.name, pos2.x, dif.x / interval)) end
	if dif.y ~= 0 then f:write(string.format("Move %s to y-axis %d speed[%d];\n", propInfo.obj.name, pos2.y, dif.y / interval)) end
	if dif.z ~= 0 then f:write(string.format("Move %s to z-axis %d speed[%d];\n", propInfo.obj.name, pos2.z, dif.z / interval)) end
end

local function OutputRotationAnim(propInfo,f)
	local r1 = upsAnimGetRotationKey(propInfo.prop, propInfo.curkey) * (32768 / M_PI)
	local r2 = upsAnimGetRotationKey(propInfo.prop, propInfo.curkey+1) * (32768 / M_PI)
	local dif = r2-r1

	local interval = upsAnimGetKeyTime(propInfo.prop, propInfo.curkey+1) - upsAnimGetKeyTime(propInfo.prop, propInfo.curkey)
	
	if dif.x ~= 0 then f:write(string.format("Turn %s to x-axis %d speed<%d>;\n", propInfo.obj.name, r2.x, dif.x / interval)) end
	if dif.y ~= 0 then f:write(string.format("Turn %s to y-axis %d speed<%d>;\n", propInfo.obj.name, r2.y, dif.y / interval)) end
	if dif.z ~= 0 then f:write(string.format("Turn %s to z-axis %d speed<%d>;\n", propInfo.obj.name, r2.z, dif.z / interval)) end
end


local animKeyTypeTbl = { 
	[ANIMTYPE_VECTOR3] = OutputVector3Anim,
	[ANIMTYPE_ROTATION] = OutputRotationAnim
}

local function OutputBOS(file, objects)
	
	local time=0
	local propInfo = {}
	
	-- collect all properties of all objects in a single map
	local index=0
	for i=0, #objects-1 do
		for propindex,prop in vectoriterate(objects[i].animInfo.properties) do
			local propType = upsAnimGetType(prop)
			
			if upsAnimGetNumKeys(prop) > 0 and (prop.name == "position" or prop.name == "rotation") then
				propInfo[index] = { 
					prop = prop,
					obj = objects[i],
					output = animKeyTypeTbl[propType],
					time = upsAnimGetKeyTime(prop, 0),
					nkeys = upsAnimGetNumKeys(prop),
					curkey = 0,
					written = false
				}
				index=index+1
			end
		end
	end

	while true do
		local nextTime = nil
		-- go through all properties and find the smallest time interval to the next key time
		for i=0, #propInfo-1 do
			local pi = propInfo[i]
			if pi.curkey < pi.nkeys-1 then			
				local nextKeyTime = upsAnimGetKeyTime(pi.prop, pi.curkey+1)
				if nextTime == nil or nextTime < nextKeyTime then
					nextTime = nextKeyTime
				end
			end
		end
		--upsMsgBox("Time: ", time, "  nextTime: ", nextTime)
		if nextTime == nil then break end
		
		for i=0, #propInfo-1 do
			local pi=propInfo[i]
			if pi.curkey < pi.nkeys-1 and not pi.written then
				if pi.output then pi.output(pi,file) end
				pi.written = true
			end
		end

		file:write(string.format("Sleep %d;\n", 1000*(nextTime-time)))
		
		time = nextTime
		
		for i=0, #propInfo-1 do
			local pi = propInfo[i]
			if pi.curkey < pi.nkeys-1 then			
				if time >= upsAnimGetKeyTime(pi.prop,pi.curkey+1) then
					pi.curkey=pi.curkey+1
					pi.written=false
				end
			end
		end		
	end
end

function ExecBOSExport()
	local result = upsFileSaveDlg("Enter txt file to save to:", "BOS script file(BOS)\0bos\0")
	if result ~= nil then
		local f = assert(io.open(result, "w"))

		OutputBOS(f, upsGetModel():GetObjectList())
		
		f:close()
		
		upsMsgBox("File exported: ", result);
	end
end


-- Register the script to upspring
upsAddMenuItem ("Animation/Export BOS script", "ExecBOSExport")

