package com.ruoyi.system.mapper;

import java.util.List;
import com.ruoyi.system.domain.ZInspection;

/**
 * 设备巡检Mapper接口
 * 
 * @author ruoyi
 * @date 2024-07-10
 */
public interface ZInspectionMapper 
{
    /**
     * 查询设备巡检
     * 
     * @param id 设备巡检主键
     * @return 设备巡检
     */
    public ZInspection selectZInspectionById(Long id);

    /**
     * 查询设备巡检列表
     * 
     * @param zInspection 设备巡检
     * @return 设备巡检集合
     */
    public List<ZInspection> selectZInspectionList(ZInspection zInspection);

    /**
     * 新增设备巡检
     * 
     * @param zInspection 设备巡检
     * @return 结果
     */
    public int insertZInspection(ZInspection zInspection);

    /**
     * 修改设备巡检
     * 
     * @param zInspection 设备巡检
     * @return 结果
     */
    public int updateZInspection(ZInspection zInspection);

    /**
     * 删除设备巡检
     * 
     * @param id 设备巡检主键
     * @return 结果
     */
    public int deleteZInspectionById(Long id);

    /**
     * 批量删除设备巡检
     * 
     * @param ids 需要删除的数据主键集合
     * @return 结果
     */
    public int deleteZInspectionByIds(Long[] ids);
}
