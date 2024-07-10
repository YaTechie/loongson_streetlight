package com.ruoyi.server;

import io.netty.channel.ChannelHandlerContext;
import lombok.Builder;
import lombok.Data;

@Data
@Builder
public class Client {
    String deviceID;
    ChannelHandlerContext ctx;
}
