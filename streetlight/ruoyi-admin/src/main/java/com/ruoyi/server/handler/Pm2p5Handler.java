package com.ruoyi.server.handler;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.ruoyi.server.ClientMgr;
import com.ruoyi.server.TagHandler;
import com.ruoyi.server.message.Pm2p5Vo;
import com.ruoyi.server.message.TagVo;
import com.ruoyi.system.mapper.SensorMapper;
import io.netty.channel.ChannelHandlerContext;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
@RequiredArgsConstructor
@Slf4j
public class Pm2p5Handler implements TagHandler {

    private final ClientMgr clientMgr;
    @Autowired
    private SensorMapper sensorMapper;

    @Override
    public boolean run(ChannelHandlerContext ctx, TagVo messageVo) throws Exception {
// 空气
        String deviceID = clientMgr.getID(ctx);
        if (deviceID == null) {
            return false;
        }

        Pm2p5Vo pm2p5Vo = new ObjectMapper().readValue(messageVo.getContent(), Pm2p5Vo.class);
        int count  =sensorMapper.deviceExists(deviceID);
        if(count > 0) {
            sensorMapper.updatePm2(clientMgr.getID(ctx),pm2p5Vo.getData());
        }else {
            sensorMapper.insertPm2(deviceID,pm2p5Vo.getData());
        }

        System.out.println("pm2p5Vo = " + messageVo.getContent());
        return true;
    }
}
