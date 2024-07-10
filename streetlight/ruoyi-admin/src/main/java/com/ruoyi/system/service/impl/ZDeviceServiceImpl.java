package com.ruoyi.system.service.impl;

import java.util.List;
import com.ruoyi.common.utils.DateUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import com.ruoyi.system.mapper.ZDeviceMapper;
import com.ruoyi.system.domain.ZDevice;
import com.ruoyi.system.service.IZDeviceService;

/**
 * 设备管理Service业务层处理
 * 
 * @author ruoyi
 * @date 2024-07-10
 */
@Service
public class ZDeviceServiceImpl implements IZDeviceService 
{
    @Autowired
    private ZDeviceMapper zDeviceMapper;

    /**
     * 查询设备管理
     * 
     * @param id 设备管理主键
     * @return 设备管理
     */
    @Override
    public ZDevice selectZDeviceById(Long id)
    {
        return zDeviceMapper.selectZDeviceById(id);
    }

    /**
     * 查询设备管理列表
     * 
     * @param zDevice 设备管理
     * @return 设备管理
     */
    @Override
    public List<ZDevice> selectZDeviceList(ZDevice zDevice)
    {
        return zDeviceMapper.selectZDeviceList(zDevice);
    }

    /**
     * 新增设备管理
     * 
     * @param zDevice 设备管理
     * @return 结果
     */
    @Override
    public int insertZDevice(ZDevice zDevice)
    {
        zDevice.setCreateTime(DateUtils.getNowDate());
        return zDeviceMapper.insertZDevice(zDevice);
    }

    /**
     * 修改设备管理
     * 
     * @param zDevice 设备管理
     * @return 结果
     */
    @Override
    public int updateZDevice(ZDevice zDevice)
    {
        zDevice.setUpdateTime(DateUtils.getNowDate());
        return zDeviceMapper.updateZDevice(zDevice);
    }

    /**
     * 批量删除设备管理
     * 
     * @param ids 需要删除的设备管理主键
     * @return 结果
     */
    @Override
    public int deleteZDeviceByIds(Long[] ids)
    {
        return zDeviceMapper.deleteZDeviceByIds(ids);
    }

    /**
     * 删除设备管理信息
     * 
     * @param id 设备管理主键
     * @return 结果
     */
    @Override
    public int deleteZDeviceById(Long id)
    {
        return zDeviceMapper.deleteZDeviceById(id);
    }
}
