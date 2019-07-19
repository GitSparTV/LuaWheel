ScrW = 800
ScrH = 600
pf = math.floor
CarVelocity = 0
RoadSpeed = 0
CarX = (150) / 2
WheelRange = 0
CarColor = {0.7, 0, 0}
WheelButtonCacheCheck = {}

function love.load()
	Img = {}
	Img.Wheel = love.graphics.newImage("wheel.png")

	if not Wheel then
		Wheel = require("luawheel")
	end

	Img.Road = love.graphics.newCanvas(300, 1200)

	Img.Road:renderTo(function()
		love.graphics.setColor(0.4, 0.4, 0.4)
		love.graphics.rectangle("fill", 0, 0, 300, 1200)
		love.graphics.setColor(0.9, 0.9, 0.9)

		for I = 1, 20 do
			local len = 1200 / 20 - 10
			love.graphics.rectangle("fill", 145, (I - 1) * (len + 10), 10, len)
		end
	end)

	Img.Car = love.graphics.newCanvas(60, 125)

	function ReRenderCar()
		Img.Car:renderTo(function()
			love.graphics.clear()
			love.graphics.setColor(0, 0, 0)
			love.graphics.rectangle("fill", 0, 7, 2, 20)
			love.graphics.rectangle("fill", 0, 125 - 7 - 20, 2, 20)
			love.graphics.rectangle("fill", 58, 7, 2, 20)
			love.graphics.rectangle("fill", 58, 125 - 7 - 20, 2, 20)
			love.graphics.setColor(CarColor[1], CarColor[2], CarColor[3])
			love.graphics.rectangle("fill", 1, 0, 58, 125)
			love.graphics.setColor(0, 0, 0)
			love.graphics.rectangle("fill", 7, 1, 10, 5)
			love.graphics.rectangle("fill", 60 - 7 - 10, 1, 10, 5)
			love.graphics.setColor(0.2, 0.2, 0.2)
			love.graphics.rectangle("fill", 10, 30, 40, 60)
		end)
	end

	ReRenderCar()

	if Wheel.IsAvailable() then
		WheelRange = Wheel.GetWheelProperties(1).wheelRange / 2
	end

	love.inited = true
end

function love.quit()
	debug.getregistry().LuaWheelDLLFinalizer:__gc() -- Just in case
end

function love.draw()
	love.graphics.setBackgroundColor(0.7, 0.7, 0.7)
	if not Wheel.IsAvailable() or not Wheel.IsDeviceConnected(1) or Wheel.GetDeviceType(1) ~= Wheel.DeviceTypes.WHEEL or not love.inited then return end
	local R = Wheel.GetWheelPosition(1)
	local t = 100
	love.graphics.setColor(1,1,1)
	love.graphics.draw(Img.Road, 400 - 150, -600 + (RoadSpeed) % 600)
	love.graphics.draw(Img.Car, 400 + CarX, 400, 0, 1, 1, 30, 62)
	CarX = math.Clamp(CarX + math.Remap(R, -32768, 32767, -WheelRange, WheelRange) * math.Remap(CarVelocity, 0, 300, 0, 1), -WheelRange, WheelRange)
	-- CarR = math.Clamp(CarR + math.Remap(R, -32768, 32767, -80, 80) * math.Remap(CarVelocity,0,300,0,1),-150,150) Too lazy to make this work, you would do better
	love.graphics.setColor(0.6, 0.6, 0.6)
	love.graphics.rectangle("fill", 0, pf(ScrH - t - 20), ScrW, pf(t + 20))
	love.graphics.setColor(0, 0, 0)
	love.graphics.draw(Img.Wheel, pf(t / 2 + 10), pf(ScrH - t / 2 - 10), math.rad(math.Remap(R, -32768, 32767, -WheelRange, WheelRange)), 0.2, 0.2, pf(Img.Wheel:getWidth() * 0.5), pf(Img.Wheel:getHeight() * 0.5))
	love.graphics.setColor(0.3, 0.3, 0.3)
	love.graphics.draw(Img.Wheel, pf(t / 2 + 10), pf(ScrH - t / 2 - 10), math.rad(math.Remap(R, -32768, 32767, -WheelRange, WheelRange)), 0.19, 0.19, pf(Img.Wheel:getWidth() * 0.5), pf(Img.Wheel:getHeight() * 0.5))
	local Gas, Brake = math.Remap(Wheel.GetWheelAccelerator(1), -32768, 32767, 1, 0), math.Remap(Wheel.GetWheelBrake(1), -32768, 32767, 1, 0)
	love.graphics.setColor(0.4, 0.4, 0.4)
	love.graphics.rectangle("fill", pf(t) + 20, pf(ScrH - t - 10) + pf(t) - pf(t) * Gas, 20, pf(t) * Gas)
	love.graphics.rectangle("fill", pf(t) + 50, pf(ScrH - t - 10) + pf(t) - pf(t) * Brake, 20, pf(t) * Brake)
	love.graphics.rectangle("fill", ScrW - pf(t), pf(ScrH - t - 10) + pf(t) - pf(t) * math.Remap(CarVelocity, 0, 300, 0, 1), 20, pf(t) * math.Remap(CarVelocity, 0, 300, 0, 1))
	love.graphics.setColor(1, 1, 1)
	love.graphics.print(math.floor(CarVelocity), ScrW - pf(t), pf(ScrH - t - 10))
end

function love.update(dt)
	if not Wheel.IsAvailable() or not Wheel.IsDeviceConnected(1) or Wheel.GetDeviceType(1) ~= Wheel.DeviceTypes.WHEEL or not love.inited then return end
	love.wheelbuttonthink()
	local R, Gas, Brake = Wheel.GetWheelControls(1)
	R, Gas, Brake = math.Remap(math.abs(R), 0, 32768, 1, 0.9), math.Remap(Gas, -32768, 32767, 100, 0), math.Remap(Brake, -32768, 32767, 100, 0)

	if Gas ~= 0 or Brake ~= 0 or CarVelocity ~= 0 then
		Wheel.SetWheelForceType(1, Wheel.ForceTypes.SOFTSTOP, true, math.Remap(CarVelocity, 0, 200, 100, 0))
		Wheel.SetWheelForceType(1, Wheel.ForceTypes.SURFACE_EFFECT, true, Wheel.PeriodicTypes.SINE, math.Remap(CarVelocity, 0, 300, 0, 100) * math.Remap(Gas - Brake, 100, 0, 1, 0.7), math.Remap(Gas - Brake, 100, 0, 20, 80))
	end

	RoadSpeed = RoadSpeed + (CarVelocity * 0.1)

	if Gas ~= 0 and Brake ~= 0 then
		Gas, Brake = 0, 0
	end

	CarVelocity = math.Clamp(CarVelocity + ((Gas * 0.01 * R) * math.Remap(CarVelocity, 0, 300, 1, 0.9)) - ((Brake * 0.05 * math.Remap(CarVelocity, 0, 300, 1, 0.1)) + math.Remap(CarVelocity, 0, 300, 0, 0.3)), 0, 300)
end

function love.wheelbuttonthink()
	if not Wheel.IsAvailable() or not Wheel.IsDeviceConnected(1) or Wheel.GetDeviceType(1) ~= Wheel.DeviceTypes.WHEEL or not love.inited then return end
	local CurButtons = Wheel.GetWheelButtons(1)

	for k, v in pairs(CurButtons) do
		local cacheButton = WheelButtonCacheCheck[k] or false

		if v == true and cacheButton == false and love.wheelbuttonpressed then
			love.wheelbuttonpressed(k)
		end

		WheelButtonCacheCheck[k] = v
	end

	for k, v in pairs(WheelButtonCacheCheck) do
		local cacheButton = CurButtons[k] or false

		if v == true and cacheButton == false and love.wheelbuttonreleased then
			love.wheelbuttonreleased(k)
		end

		WheelButtonCacheCheck[k] = cacheButton
	end
end

function love.wheelbuttonpressed(b)
	if b == 3 then
		CarColor = {0.7, 0, 0}
		ReRenderCar()
	elseif b == 4 then
		CarColor = {0.7, 0.7, 0}
		ReRenderCar()
	elseif b == 5 then
		CarColor = {0.7, 0, 0.7}
		ReRenderCar()
	elseif b == 6 then
		CarColor = {0, 0.7, 0.7}
		ReRenderCar()
	elseif b == 7 then
		CarColor = {0, 0.7, 0}
		ReRenderCar()
	elseif b == 8 then
		CarColor = {0, 0, 0.7}
		ReRenderCar()
	end
end

function math.Remap(value, inMin, inMax, outMin, outMax)
	return outMin + (((value - inMin) / (inMax - inMin)) * (outMax - outMin))
end

function math.Clamp(_in, low, high)
	return math.min(math.max(_in, low), high)
end