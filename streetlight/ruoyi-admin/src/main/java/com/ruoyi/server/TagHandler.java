package com.ruoyi.server;

import com.ruoyi.server.message.TagVo;
import io.netty.channel.ChannelHandlerContext;


public interface TagHandler {
    public boolean run(ChannelHandlerContext ctx, TagVo messageVo) throws Exception;
}
