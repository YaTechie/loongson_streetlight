package com.ruoyi.server.handler;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.ruoyi.server.ClientMgr;
import com.ruoyi.server.TagHandler;
import com.ruoyi.server.message.PwmGetDutyCycleVo;
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
public class PwmGetDutyCycleHandler implements TagHandler {
    private final ClientMgr clientMgr;
    @Autowired
    private SensorMapper sensorMapper;
    @Override
    public boolean run(ChannelHandlerContext ctx, TagVo messageVo) throws Exception {
//        光照
        String deviceID = clientMgr.getID(ctx);
        if (deviceID == null) {
            return false;
        }

        PwmGetDutyCycleVo pwmGetDutyCycleVo = new ObjectMapper().readValue(messageVo.getContent(), PwmGetDutyCycleVo.class);

//        System.out.println("2 "+pwmGetDutyCycleVo.getData());
        int count = sensorMapper.deviceExists(deviceID);
        if(count > 0) {
            sensorMapper.updatePWM(clientMgr.getID(ctx),pwmGetDutyCycleVo.getData());

        }else {
            sensorMapper.insertPWM(clientMgr.getID(ctx),pwmGetDutyCycleVo.getData());
        }
        System.out.println("PWMctx = " + messageVo);
        return true;
    }
}
