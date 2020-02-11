------------------------------------------------------------
-- R_HandAttach
-- HandM
-- EEshake
------------------------------------------------------------
-- if vci.assets.IsMine then -- 呼び出したユーザーが代表して初期化する
--     vci.state.Set('switch', 0) 
--     vci.state.Set('placed', 0)
-- end
isPlaced = 0
tran = 1.0 --透明度

vci.assets._ALL_SetMaterialColorFromName("HandMAu", Color.__new(0.0, 0.0, 1.0, 0.1))

function onUse(use)
    vci.assets.HapticPulseOnGrabbingController(use, 3999, 1)
end

function onTriggerEnter(item, hit)
    -- 右手を重ねたきに、透明化のフラグを立てる
    if item == "R_HandAttach" then --触られるオブジェクト
        if hit == "RightHand" then
            -- vci.assets._ALL_SetMaterialColorFromName("HandM", Color.__new(1.0, 1.0, 1.0, tran))
            -- vci.assets.GetEffekseerEmitter("EEshake")._ALL_Play()
            -- vci.state.Set('placed', 1)
            isPlaced = 1
        end
    -- 相手方のitemに当たったとき、websocket経由で通知する
        if hit == "R_HandAttach" then
            print("Handshaked")
            vci.message.Emit("Handshaked",0)
            vci.assets.GetEffekseerEmitter("EEshake")._ALL_Play()
        end
    end
    -- 左手があったときエフェクト発生
    if item == "R_HandAttach" then --触られるオブジェクト
        if hit == "LeftHand" then
            -- vci.message.Emit("Handshaked",0)
            vci.assets.GetEffekseerEmitter("EEshake")._ALL_Play()
            -- デバッグ用、掴んでいる状態で左手に触れたとき、振動ささせる
            vci.assets.HapticPulseOnGrabbingController("R_HandAttach", 3999, 1)
        end
    end
end

function onTriggerExit(item, hit)
    tran = 0.9
    -- print(hit)
-- 手がHandAttachから離れたときに可視化する
    if item == "R_HandAttach" then --触られるオブジェクト
        if hit == "RightHand" then
            isPlaced = 0
        end
    end
end

function onGrab(target)
    -- 右手が重ねられている状態で掴んだ時、透明になる
    if isPlaced == 1 then
        tran = 0.0
        -- vci.assets._ALL_SetMaterialColorFromName("HandM", Color.__new(1.0, 1.0, 1.0, tran))
        vci.assets._ALL_SetMaterialColorFromName("HandMAu", Color.__new(0.0, 0.0, 1.0, tran))

    end
end

function onUngrab(target)
        vci.assets._ALL_SetMaterialColorFromName("HandMAu", Color.__new(0.0, 0.0, 1.0, 0.1))
end

