package com.ruoyi.server.handler;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.ruoyi.server.ClientMgr;
import com.ruoyi.server.TagHandler;
import com.ruoyi.server.message.TagVo;
import com.ruoyi.server.message.ThlVo;
import com.ruoyi.system.mapper.SensorMapper;
import io.netty.channel.ChannelHandlerContext;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
@RequiredArgsConstructor
@Slf4j
public class ThlHandler implements TagHandler {
    private final ClientMgr clientMgr;
    @Autowired
    private SensorMapper sensorMapper;
    @Override
    public boolean run(ChannelHandlerContext ctx, TagVo messageVo) throws Exception {
//        温度，湿度，光照
        String deviceID = clientMgr.getID(ctx);
        if (deviceID == null) {
            return false;
        }

        ThlVo thlVo = new ObjectMapper().readValue(messageVo.getContent(), ThlVo.class);
//        System.out.println("3 "+thlVo.getHumidity());
//        System.out.println("33 "+thlVo.getLight());
//        System.out.println("333 "+thlVo.getTemperature());

        int count = sensorMapper.deviceExists(deviceID);
        if(count > 0) {
            sensorMapper.updateThl(clientMgr.getID(ctx),thlVo.getHumidity(),thlVo.getTemperature(),thlVo.getLight());

        }else {
            sensorMapper.insertThl(clientMgr.getID(ctx),thlVo.getHumidity(),thlVo.getTemperature(),thlVo.getLight());
        }
        return true;
    }
}
