package com.ruoyi.system.domain;

import java.math.BigDecimal;
import org.apache.commons.lang3.builder.ToStringBuilder;
import org.apache.commons.lang3.builder.ToStringStyle;
import com.ruoyi.common.annotation.Excel;
import com.ruoyi.common.core.domain.BaseEntity;

/**
 * 设备管理对象 z_device
 * 
 * @author ruoyi
 * @date 2024-07-10
 */
public class ZDevice extends BaseEntity
{
    private static final long serialVersionUID = 1L;

    /** 编号 */
    private Long id;

    /** 设备编号 */
    @Excel(name = "设备编号")
    private String deviceId;

    /** 设备名称 */
    @Excel(name = "设备名称")
    private String name;

    /** 是否在线 */
    @Excel(name = "是否在线")
    private Integer online;

    /** 亮度 */
    private Long percent;

    /** 纬度 */
    @Excel(name = "纬度")
    private BigDecimal latitude;

    /** 经度 */
    @Excel(name = "经度")
    private BigDecimal longitude;

    /** 风速 */
    @Excel(name = "风速")
    private BigDecimal windspeed;

    /** 风向 */
    @Excel(name = "风向")
    private String direction;

    /** 角度 */
    @Excel(name = "角度")
    private Long angle;

    /** 光照 */
    @Excel(name = "光照")
    private BigDecimal sensorLight;

    /** 温度 */
    @Excel(name = "温度")
    private BigDecimal sensorHumidity;

    /** 湿度 */
    @Excel(name = "湿度")
    private BigDecimal sensorTemperature;

    /** 亮度 */
    @Excel(name = "亮度")
    private BigDecimal sensorPwm;

    /** 空气 */
    @Excel(name = "空气")
    private BigDecimal sensorPm2;

    public void setId(Long id) 
    {
        this.id = id;
    }

    public Long getId() 
    {
        return id;
    }
    public void setDeviceId(String deviceId) 
    {
        this.deviceId = deviceId;
    }

    public String getDeviceId() 
    {
        return deviceId;
    }
    public void setName(String name) 
    {
        this.name = name;
    }

    public String getName() 
    {
        return name;
    }
    public void setOnline(Integer online) 
    {
        this.online = online;
    }

    public Integer getOnline() 
    {
        return online;
    }
    public void setPercent(Long percent) 
    {
        this.percent = percent;
    }

    public Long getPercent() 
    {
        return percent;
    }
    public void setLatitude(BigDecimal latitude) 
    {
        this.latitude = latitude;
    }

    public BigDecimal getLatitude() 
    {
        return latitude;
    }
    public void setLongitude(BigDecimal longitude) 
    {
        this.longitude = longitude;
    }

    public BigDecimal getLongitude() 
    {
        return longitude;
    }
    public void setWindspeed(BigDecimal windspeed) 
    {
        this.windspeed = windspeed;
    }

    public BigDecimal getWindspeed() 
    {
        return windspeed;
    }
    public void setDirection(String direction) 
    {
        this.direction = direction;
    }

    public String getDirection() 
    {
        return direction;
    }
    public void setAngle(Long angle) 
    {
        this.angle = angle;
    }

    public Long getAngle() 
    {
        return angle;
    }
    public void setSensorLight(BigDecimal sensorLight) 
    {
        this.sensorLight = sensorLight;
    }

    public BigDecimal getSensorLight() 
    {
        return sensorLight;
    }
    public void setSensorHumidity(BigDecimal sensorHumidity) 
    {
        this.sensorHumidity = sensorHumidity;
    }

    public BigDecimal getSensorHumidity() 
    {
        return sensorHumidity;
    }
    public void setSensorTemperature(BigDecimal sensorTemperature) 
    {
        this.sensorTemperature = sensorTemperature;
    }

    public BigDecimal getSensorTemperature() 
    {
        return sensorTemperature;
    }
    public void setSensorPwm(BigDecimal sensorPwm) 
    {
        this.sensorPwm = sensorPwm;
    }

    public BigDecimal getSensorPwm() 
    {
        return sensorPwm;
    }
    public void setSensorPm2(BigDecimal sensorPm2) 
    {
        this.sensorPm2 = sensorPm2;
    }

    public BigDecimal getSensorPm2() 
    {
        return sensorPm2;
    }

    @Override
    public String toString() {
        return new ToStringBuilder(this,ToStringStyle.MULTI_LINE_STYLE)
            .append("id", getId())
            .append("deviceId", getDeviceId())
            .append("name", getName())
            .append("online", getOnline())
            .append("createBy", getCreateBy())
            .append("createTime", getCreateTime())
            .append("updateBy", getUpdateBy())
            .append("updateTime", getUpdateTime())
            .append("remark", getRemark())
            .append("percent", getPercent())
            .append("latitude", getLatitude())
            .append("longitude", getLongitude())
            .append("windspeed", getWindspeed())
            .append("direction", getDirection())
            .append("angle", getAngle())
            .append("sensorLight", getSensorLight())
            .append("sensorHumidity", getSensorHumidity())
            .append("sensorTemperature", getSensorTemperature())
            .append("sensorPwm", getSensorPwm())
            .append("sensorPm2", getSensorPm2())
            .toString();
    }
}
