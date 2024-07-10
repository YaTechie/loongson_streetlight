package com.ruoyi.server.handler;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.ruoyi.server.ClientMgr;
import com.ruoyi.server.TagHandler;
import com.ruoyi.server.message.TagVo;
import com.ruoyi.server.message.WindSpeedVo;
import com.ruoyi.system.mapper.SensorMapper;
import io.netty.channel.ChannelHandlerContext;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
@RequiredArgsConstructor
@Slf4j
public class WindSpeedHandler implements TagHandler {
    private final ClientMgr clientMgr;

    @Autowired
    private SensorMapper sensorMapper;
    static int test = 500;
    @Override
    public boolean run(ChannelHandlerContext ctx, TagVo messageVo) throws Exception {
        // 风速
        String deviceID = clientMgr.getID(ctx);
        if (deviceID == null) {
            return false;
        }

        WindSpeedVo windSpeedVo = new ObjectMapper().readValue(messageVo.getContent(), WindSpeedVo.class);
        int count = sensorMapper.deviceExists(deviceID);
        if(count > 0) {
            sensorMapper.updateWindSpeed(clientMgr.getID(ctx),windSpeedVo.getData());

        }else {
            sensorMapper.insertWindSpeed(clientMgr.getID(ctx),windSpeedVo.getData());
        }

        return true;
    }
}
