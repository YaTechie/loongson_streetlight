package com.ruoyi.server.handler;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.ruoyi.server.ClientMgr;
import com.ruoyi.server.NetCode;
import com.ruoyi.server.TagHandler;
import com.ruoyi.server.message.ActiveRespVo;
import com.ruoyi.server.message.LoginRespVo;
import com.ruoyi.server.message.LoginVo;
import com.ruoyi.server.message.TagVo;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.http.websocketx.TextWebSocketFrame;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Component;

@Component
@RequiredArgsConstructor
@Slf4j
public class LoginHandler implements TagHandler {

    private final ClientMgr clientMgr;

    @Override
    public boolean run(ChannelHandlerContext ctx, TagVo messageVo) throws Exception {

        LoginVo loginVo = new ObjectMapper().readValue(messageVo.getContent(), LoginVo.class);

        clientMgr.add(ctx, loginVo.getId());

        LoginRespVo loginRespVo = LoginRespVo.builder().command(NetCode.loginResp).result(true).build();

        ctx.writeAndFlush(new TextWebSocketFrame(new ObjectMapper().writeValueAsString(loginRespVo)));

        return true;
    }
}
