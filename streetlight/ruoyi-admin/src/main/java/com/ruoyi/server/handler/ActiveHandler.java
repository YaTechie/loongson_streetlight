package com.ruoyi.server.handler;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.ruoyi.server.NetCode;
import com.ruoyi.server.TagHandler;
import com.ruoyi.server.message.ActiveRespVo;
import com.ruoyi.server.message.TagVo;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.http.websocketx.TextWebSocketFrame;
import org.springframework.stereotype.Component;

@Component
public class ActiveHandler implements TagHandler {

    @Override
    public boolean run(ChannelHandlerContext ctx, TagVo messageVo) throws Exception {
        ActiveRespVo activeRespVo = ActiveRespVo.builder().command(NetCode.activeResp).build();

        ctx.writeAndFlush(new TextWebSocketFrame(new ObjectMapper().writeValueAsString(activeRespVo)));

        return true;
    }
}
