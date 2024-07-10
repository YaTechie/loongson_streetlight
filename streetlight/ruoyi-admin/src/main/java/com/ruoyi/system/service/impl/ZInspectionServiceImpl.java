package com.ruoyi.system.service.impl;

import java.util.List;
import com.ruoyi.common.utils.DateUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import com.ruoyi.system.mapper.ZInspectionMapper;
import com.ruoyi.system.domain.ZInspection;
import com.ruoyi.system.service.IZInspectionService;

/**
 * 设备巡检Service业务层处理
 * 
 * @author ruoyi
 * @date 2024-07-10
 */
@Service
public class ZInspectionServiceImpl implements IZInspectionService 
{
    @Autowired
    private ZInspectionMapper zInspectionMapper;

    /**
     * 查询设备巡检
     * 
     * @param id 设备巡检主键
     * @return 设备巡检
     */
    @Override
    public ZInspection selectZInspectionById(Long id)
    {
        return zInspectionMapper.selectZInspectionById(id);
    }

    /**
     * 查询设备巡检列表
     * 
     * @param zInspection 设备巡检
     * @return 设备巡检
     */
    @Override
    public List<ZInspection> selectZInspectionList(ZInspection zInspection)
    {
        return zInspectionMapper.selectZInspectionList(zInspection);
    }

    /**
     * 新增设备巡检
     * 
     * @param zInspection 设备巡检
     * @return 结果
     */
    @Override
    public int insertZInspection(ZInspection zInspection)
    {
        zInspection.setCreateTime(DateUtils.getNowDate());
        return zInspectionMapper.insertZInspection(zInspection);
    }

    /**
     * 修改设备巡检
     * 
     * @param zInspection 设备巡检
     * @return 结果
     */
    @Override
    public int updateZInspection(ZInspection zInspection)
    {
        zInspection.setUpdateTime(DateUtils.getNowDate());
        return zInspectionMapper.updateZInspection(zInspection);
    }

    /**
     * 批量删除设备巡检
     * 
     * @param ids 需要删除的设备巡检主键
     * @return 结果
     */
    @Override
    public int deleteZInspectionByIds(Long[] ids)
    {
        return zInspectionMapper.deleteZInspectionByIds(ids);
    }

    /**
     * 删除设备巡检信息
     * 
     * @param id 设备巡检主键
     * @return 结果
     */
    @Override
    public int deleteZInspectionById(Long id)
    {
        return zInspectionMapper.deleteZInspectionById(id);
    }
}
