math.randomseed(os.time())
ConfettiTarget = vci.assets.GetSubItem("ConfettiTarget")
ConfettiTarget.SetLocalPosition(Vector3.__new(0, 1.5, 0))
vci.assets._ALL_SetMaterialColorFromName("Transparent",
 Color.__new(1.0, 1.0, 1.0, 0.5))
EEConfetti = vci.assets.GetSubItem("EEconfetti")
gen_height = 0
size = 1
ToumeiFlag = 0

function fallConfetti()
    local rand_xpos = math.random(0, 100) / 100
    local rand_zpos = math.random(0, 100) / 100
    local rand_xvel = math.random(-100, 100) / 100
    local rand_zvel = math.random(-100, 100) / 100
    local ConfettiTarget_pos = ConfettiTarget.GetLocalPosition()
    print(ConfettiTarget_pos)
    local pos = Vector3.__new(ConfettiTarget_pos.x,
     ConfettiTarget_pos.y + gen_height, ConfettiTarget_pos.z)
    local scale = Vector3.__new(size, size, size)
    EEConfetti.SetLocalPosition(pos)
    EEConfetti.SetLocalScale(scale)
    vci.assets.GetEffekseerEmitter("EEconfetti")._ALL_Play()
end

function update()
    if vci.me.GetButtonInput(1) then
        fallConfetti()
    end
    if vci.me.GetAxisInput().y == 1 then
        print("enableLEDmeter")
    end
    if vci.me.GetAxisInput().y == -1 then
        print("disableLEDmeter")
    end
end

function onUse(use)
    if use == "ConfettiTarget" then
        if ToumeiFlag == 0 then
            vci.assets._ALL_SetMaterialColorFromName(
                "Transparent", Color.__new(1.0, 0, 1.0, 0.0))
            ToumeiFlag = 1
        end
    end
end
