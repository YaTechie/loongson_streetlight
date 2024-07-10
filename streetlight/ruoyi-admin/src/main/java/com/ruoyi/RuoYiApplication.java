package com.ruoyi;

import com.ruoyi.server.NettyServer;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.ApplicationArguments;
import org.springframework.boot.ApplicationRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.jdbc.DataSourceAutoConfiguration;
import org.springframework.context.ApplicationListener;
import org.springframework.context.event.ContextClosedEvent;

import java.net.InetAddress;
import java.net.UnknownHostException;

/**
 * 启动程序
 *
 * @author ruoyi
 */
@SpringBootApplication(exclude = { DataSourceAutoConfiguration.class })
@RequiredArgsConstructor
@Slf4j
public class RuoYiApplication implements ApplicationRunner, ApplicationListener<ContextClosedEvent>
{
    private final NettyServer nettyServer;

    @Value("${server.port}")
    private int portServer;

    @Value("${netty.port}")
    private int portNetty;

    public static void main(String[] args) throws UnknownHostException {
        SpringApplication.run(RuoYiApplication.class, args);
        runsout();
    }



    public static void runsout() throws UnknownHostException {
        String ip = InetAddress.getLocalHost().getHostAddress();
        System.out.println("springBoot启动成功："+ip);
    }

    @Override
    public void run(ApplicationArguments args) throws Exception {
        if ( !nettyServer.start(portNetty) )
        {
            return;
        }

        log.info("服务器启动成功：http://127.0.0.1:" + portServer + " Netty Port:" + portNetty);
    }



    @Override
    public void onApplicationEvent(ContextClosedEvent event) {
        nettyServer.stop();
    }
}
