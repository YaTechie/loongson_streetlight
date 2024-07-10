package com.ruoyi.server.handler;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.ruoyi.server.ClientMgr;
import com.ruoyi.server.TagHandler;
import com.ruoyi.server.message.TagVo;
import com.ruoyi.server.message.WindDirectVo;
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
public class WindDirectionHandler implements TagHandler {
    private final ClientMgr clientMgr;
    @Autowired
    private SensorMapper sensorMapper;
    @Override
    public boolean run(ChannelHandlerContext ctx, TagVo messageVo) throws Exception {
//        风向
        String deviceID = clientMgr.getID(ctx);
        if (deviceID == null) {
            return false;
        }

        WindDirectVo windDirectVo = new ObjectMapper().readValue(messageVo.getContent(), WindDirectVo.class);
//        System.out.println("4 "+windDirectVo.getDirection());
//        System.out.println("44 "+windDirectVo.getAngle());
        int count  =sensorMapper.deviceExists(deviceID);
        if(count > 0) {
            sensorMapper.updateDirection(deviceID,windDirectVo.getDirection(),windDirectVo.getAngle());
        }else {
            sensorMapper.insertDirection(deviceID,windDirectVo.getDirection(),windDirectVo.getAngle());
        }

        return true;
    }
}
