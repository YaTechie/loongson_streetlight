package com.ruoyi.system.controller;

import java.util.List;
import javax.servlet.http.HttpServletResponse;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import com.ruoyi.common.annotation.Log;
import com.ruoyi.common.core.controller.BaseController;
import com.ruoyi.common.core.domain.AjaxResult;
import com.ruoyi.common.enums.BusinessType;
import com.ruoyi.system.domain.ZInspection;
import com.ruoyi.system.service.IZInspectionService;
import com.ruoyi.common.utils.poi.ExcelUtil;
import com.ruoyi.common.core.page.TableDataInfo;

/**
 * 设备巡检Controller
 * 
 * @author ruoyi
 * @date 2024-07-10
 */
@RestController
@RequestMapping("/system/inspection")
public class ZInspectionController extends BaseController
{
    @Autowired
    private IZInspectionService zInspectionService;

    /**
     * 查询设备巡检列表
     */
    @PreAuthorize("@ss.hasPermi('system:inspection:list')")
    @GetMapping("/list")
    public TableDataInfo list(ZInspection zInspection)
    {
        startPage();
        List<ZInspection> list = zInspectionService.selectZInspectionList(zInspection);
        return getDataTable(list);
    }

    /**
     * 导出设备巡检列表
     */
    @PreAuthorize("@ss.hasPermi('system:inspection:export')")
    @Log(title = "设备巡检", businessType = BusinessType.EXPORT)
    @PostMapping("/export")
    public void export(HttpServletResponse response, ZInspection zInspection)
    {
        List<ZInspection> list = zInspectionService.selectZInspectionList(zInspection);
        ExcelUtil<ZInspection> util = new ExcelUtil<ZInspection>(ZInspection.class);
        util.exportExcel(response, list, "设备巡检数据");
    }

    /**
     * 获取设备巡检详细信息
     */
    @PreAuthorize("@ss.hasPermi('system:inspection:query')")
    @GetMapping(value = "/{id}")
    public AjaxResult getInfo(@PathVariable("id") Long id)
    {
        return success(zInspectionService.selectZInspectionById(id));
    }

    /**
     * 新增设备巡检
     */
    @PreAuthorize("@ss.hasPermi('system:inspection:add')")
    @Log(title = "设备巡检", businessType = BusinessType.INSERT)
    @PostMapping
    public AjaxResult add(@RequestBody ZInspection zInspection)
    {
        return toAjax(zInspectionService.insertZInspection(zInspection));
    }

    /**
     * 修改设备巡检
     */
    @PreAuthorize("@ss.hasPermi('system:inspection:edit')")
    @Log(title = "设备巡检", businessType = BusinessType.UPDATE)
    @PutMapping
    public AjaxResult edit(@RequestBody ZInspection zInspection)
    {
        return toAjax(zInspectionService.updateZInspection(zInspection));
    }

    /**
     * 删除设备巡检
     */
    @PreAuthorize("@ss.hasPermi('system:inspection:remove')")
    @Log(title = "设备巡检", businessType = BusinessType.DELETE)
	@DeleteMapping("/{ids}")
    public AjaxResult remove(@PathVariable Long[] ids)
    {
        return toAjax(zInspectionService.deleteZInspectionByIds(ids));
    }
}
