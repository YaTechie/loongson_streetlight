package com.ruoyi.system.domain;

import org.apache.commons.lang3.builder.ToStringBuilder;
import org.apache.commons.lang3.builder.ToStringStyle;
import com.ruoyi.common.annotation.Excel;
import com.ruoyi.common.core.domain.BaseEntity;

/**
 * 设备巡检对象 z_inspection
 * 
 * @author ruoyi
 * @date 2024-07-10
 */
public class ZInspection extends BaseEntity
{
    private static final long serialVersionUID = 1L;

    /** 编号 */
    private Long id;

    /** 设备id */
    @Excel(name = "设备id")
    private String deviceId;

    /** 检查情况 */
    @Excel(name = "检查情况")
    private Long status;

    /** 用户id */
    @Excel(name = "用户id")
    private Long userId;

    /** 检查人员 */
    @Excel(name = "检查人员")
    private String inspectionName;

    /** 检查方式 */
    @Excel(name = "检查方式")
    private String method;

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
    public void setStatus(Long status) 
    {
        this.status = status;
    }

    public Long getStatus() 
    {
        return status;
    }
    public void setUserId(Long userId) 
    {
        this.userId = userId;
    }

    public Long getUserId() 
    {
        return userId;
    }
    public void setInspectionName(String inspectionName) 
    {
        this.inspectionName = inspectionName;
    }

    public String getInspectionName() 
    {
        return inspectionName;
    }
    public void setMethod(String method) 
    {
        this.method = method;
    }

    public String getMethod() 
    {
        return method;
    }

    @Override
    public String toString() {
        return new ToStringBuilder(this,ToStringStyle.MULTI_LINE_STYLE)
            .append("id", getId())
            .append("deviceId", getDeviceId())
            .append("status", getStatus())
            .append("userId", getUserId())
            .append("inspectionName", getInspectionName())
            .append("method", getMethod())
            .append("createBy", getCreateBy())
            .append("createTime", getCreateTime())
            .append("updateBy", getUpdateBy())
            .append("updateTime", getUpdateTime())
            .append("remark", getRemark())
            .toString();
    }
}
