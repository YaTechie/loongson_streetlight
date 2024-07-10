package com.ruoyi.system.service;

import java.util.List;
import com.ruoyi.system.domain.ZDevice;

/**
 * 设备管理Service接口
 * 
 * @author ruoyi
 * @date 2024-07-10
 */
public interface IZDeviceService 
{
    /**
     * 查询设备管理
     * 
     * @param id 设备管理主键
     * @return 设备管理
     */
    public ZDevice selectZDeviceById(Long id);

    /**
     * 查询设备管理列表
     * 
     * @param zDevice 设备管理
     * @return 设备管理集合
     */
    public List<ZDevice> selectZDeviceList(ZDevice zDevice);

    /**
     * 新增设备管理
     * 
     * @param zDevice 设备管理
     * @return 结果
     */
    public int insertZDevice(ZDevice zDevice);

    /**
     * 修改设备管理
     * 
     * @param zDevice 设备管理
     * @return 结果
     */
    public int updateZDevice(ZDevice zDevice);

    /**
     * 批量删除设备管理
     * 
     * @param ids 需要删除的设备管理主键集合
     * @return 结果
     */
    public int deleteZDeviceByIds(Long[] ids);

    /**
     * 删除设备管理信息
     * 
     * @param id 设备管理主键
     * @return 结果
     */
    public int deleteZDeviceById(Long id);
}
