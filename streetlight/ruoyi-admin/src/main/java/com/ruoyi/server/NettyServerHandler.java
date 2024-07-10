package com.ruoyi.server;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.ruoyi.server.handler.*;
import com.ruoyi.server.message.TagVo;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.handler.codec.http.websocketx.TextWebSocketFrame;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;

@Component
@Scope("prototype")
@RequiredArgsConstructor
@Slf4j
public class NettyServerHandler extends SimpleChannelInboundHandler<TextWebSocketFrame> {

    private final ClientMgr clientMgr;

    private final ActiveHandler activeHandler;
    private final LoginHandler loginHandler;
    private final PwmGetDutyCycleHandler pwmGetDutyCycleHandler;
    private final Pm2p5Handler pm2p5Handler;
    private final WindSpeedHandler windSpeedHandler;
    private final WindDirectionHandler windDirectionHandler;
    private final ThlHandler thlHandler;

    @Override
    protected void channelRead0(ChannelHandlerContext channelHandlerContext, TextWebSocketFrame textWebSocketFrame) throws Exception {
        //log.info(textWebSocketFrame.text());
        TagVo messageVo = new ObjectMapper().readValue(textWebSocketFrame.text(), TagVo.class);
        if (messageVo.getCommand() == NetCode.active) {
            activeHandler.run(channelHandlerContext, messageVo);
        } else if (messageVo.getCommand() == NetCode.login) {
            loginHandler.run(channelHandlerContext, messageVo);
        } else if (messageVo.getCommand() == NetCode.pwmGetDutyCycle) {
            pwmGetDutyCycleHandler.run(channelHandlerContext, messageVo);
        } else if (messageVo.getCommand() == NetCode.pm2p5) {
            pm2p5Handler.run(channelHandlerContext, messageVo);
        } else if (messageVo.getCommand() == NetCode.windSpeed) {
            windSpeedHandler.run(channelHandlerContext, messageVo);
        } else if (messageVo.getCommand() == NetCode.windDirection) {
            windDirectionHandler.run(channelHandlerContext, messageVo);
        } else if (messageVo.getCommand() == NetCode.thl) {
            thlHandler.run(channelHandlerContext, messageVo);
        }
    }

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        super.channelActive(ctx);
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        super.channelInactive(ctx);

        clientMgr.del(ctx);
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        super.exceptionCaught(ctx, cause);
    }
}
