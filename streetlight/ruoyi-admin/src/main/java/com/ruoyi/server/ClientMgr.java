package com.ruoyi.server;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.ruoyi.server.message.ActiveRespVo;
import com.ruoyi.server.message.PwmSetDutyCycleVo;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelId;
import io.netty.handler.codec.http.websocketx.TextWebSocketFrame;
import org.springframework.stereotype.Component;

import java.util.HashMap;
import java.util.Map;

@Component
public class ClientMgr {
    Map<ChannelId, Client> map = new HashMap<>();

    public void add(ChannelHandlerContext ctx, String deviceID) {
        map.put(ctx.channel().id(), Client.builder().deviceID(deviceID).ctx(ctx).build());
    }

    public void del(ChannelHandlerContext ctx) {
        map.remove(ctx.channel().id());
    }

    public String getID(ChannelHandlerContext ctx) {
        if (!map.containsKey(ctx.channel().id())) {
            return null;
        }

        Client client = map.get(ctx.channel().id());
        if (client == null) {
            return null;
        }

        return client.getDeviceID();
    }

    public boolean pwmSetDutyCycle(String device_id, int dutyCycle) {

        Client client = null;

        for (Map.Entry<ChannelId, Client> entry : map.entrySet()) {
            if (entry.getValue().getDeviceID().compareTo(device_id) == 0) {
                client = entry.getValue();
                break;
            }
        }

        if (client == null) {
            return false;
        }

        PwmSetDutyCycleVo pwmSetDutyCycleVo = PwmSetDutyCycleVo.builder().command(NetCode.pwmSetDutyCycle).data(dutyCycle).build();

        String content = "";
        try {
            content = new ObjectMapper().writeValueAsString(pwmSetDutyCycleVo);
        } catch (Exception e) {
            return false;
        }

        client.getCtx().writeAndFlush(new TextWebSocketFrame(content));

        return true;
    }

}
