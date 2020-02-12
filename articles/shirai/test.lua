function update()

    if vci.me.GetAxisInput().y == 1 then
        print("enableLEDmeter")
    end

    if vci.me.GetAxisInput().y == -1 then
        print("disableLEDmeter")
    end

end

