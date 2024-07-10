package com.ruoyi.server;

import com.ruoyi.common.utils.spring.SpringUtils;
import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.*;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpServerCodec;
import io.netty.handler.codec.http.websocketx.WebSocketServerProtocolHandler;
import io.netty.handler.stream.ChunkedWriteHandler;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Component;

@Component
@Slf4j
public class NettyServer {
    private EventLoopGroup bossGroup = null;
    private EventLoopGroup workerGroup = null;
    private Channel channel = null;

    public boolean start(int port) {
        log.info("正在启动Netty...");
        bossGroup = new NioEventLoopGroup();
        if (bossGroup == null) {
            log.error("创建boss线程池失败.");
            return false;
        }

        workerGroup = new NioEventLoopGroup();
        if (workerGroup == null) {
            log.error("创建worker线程池失败.");
            return false;
        }

        ServerBootstrap bootstrap = new ServerBootstrap();
        bootstrap.group(bossGroup, workerGroup)
                .channel(NioServerSocketChannel.class)
                .option(ChannelOption.SO_BACKLOG, 1024)
                .childOption(ChannelOption.SO_KEEPALIVE, true)
                .childOption(ChannelOption.TCP_NODELAY, true)
                .childHandler(new ChannelInitializer<SocketChannel>() {
                    @Override
                    protected void initChannel(SocketChannel socketChannel) throws Exception {
                        socketChannel.pipeline()
                                .addLast(new HttpServerCodec())
                                .addLast(new ChunkedWriteHandler())
                                .addLast(new HttpObjectAggregator(1024*32))
                                .addLast(new WebSocketServerProtocolHandler("/websocket"))
                                .addLast(SpringUtils.getBean(NettyServerHandler.class));
                    }
                });

        ChannelFuture sync = null;
        try {
            sync = bootstrap.bind(port).sync();
        } catch (InterruptedException e) {
            log.error("绑定端口失败.");
            return false;
        }

        if (!sync.isSuccess()) {
            log.error("启动Netty失败.");
            return false;
        }

        channel = sync.channel();

        log.info("启动Netty.");
        return true;
    }

    public void stop() {
        log.info("正在关闭Netty...");
        if ( channel != null )
        {
            channel.close();
            channel = null;
        }

        if ( bossGroup != null )
        {
            try {
                bossGroup.shutdownGracefully().sync();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            bossGroup = null;
        }

        if ( workerGroup != null )
        {
            try {
                workerGroup.shutdownGracefully().sync();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            workerGroup = null;
        }

        log.info("关闭Netty.");
    }

}
