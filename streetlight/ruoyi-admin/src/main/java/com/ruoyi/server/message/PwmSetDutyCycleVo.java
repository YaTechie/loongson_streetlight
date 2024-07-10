package com.ruoyi.server.message;

import lombok.Builder;
import lombok.Data;

@Data
@Builder
public class PwmSetDutyCycleVo {
    int command;
    int data;
}
