-- このファイルはVCIから取り出したコピーです。
-- 有効にするにはファイル名先頭の'_'を削除してください 
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
vci.assets._ALL_SetMaterialColorFromName("HandM", Color.__new(1.0, 1.0, 1.0, 0.1))

function onTriggerEnter(item, hit)
    -- 右手を重ねたきに、透明化のフラグを立てる
    if item == "R_HandAttach" then --触られるオブジェクト
        if hit == "RightHand" then
            isPlaced = 1
        end
    -- 相手方のitemに当たったとき、websocket経由で通知する
        if hit == "R_HandAttach" then
            -- print("Handshaked")
            -- vci.message.Emit("Handshaked",0)
            vci.assets.GetEffekseerEmitter("EEshake")._ALL_Play()
        end
    end
    -- 左手があったときエフェクト発生
    if item == "R_HandAttach" then --触られるオブジェクト
        if hit == "LeftHand" then
            -- vci.message.Emit("Handshaked",0)
            vci.assets.GetEffekseerEmitter("EEshake")._ALL_Play()
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
        tran = 0.0
        vci.assets._ALL_SetMaterialColorFromName("HandM", Color.__new(1.0, 1.0, 1.0, tran))
end

function onUngrab(target)
        vci.assets._ALL_SetMaterialColorFromName("HandM", Color.__new(1.0, 1.0, 1.0, 0.1))
end

