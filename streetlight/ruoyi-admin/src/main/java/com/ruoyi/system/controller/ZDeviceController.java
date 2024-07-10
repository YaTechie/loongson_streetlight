package com.ruoyi.system.controller;

import java.math.BigDecimal;
import java.util.List;
import javax.servlet.http.HttpServletResponse;

import com.ruoyi.server.ClientMgr;
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
import com.ruoyi.system.domain.ZDevice;
import com.ruoyi.system.service.IZDeviceService;
import com.ruoyi.common.utils.poi.ExcelUtil;
import com.ruoyi.common.core.page.TableDataInfo;

/**
 * 设备管理Controller
 *
 * @author ruoyi
 * @date 2024-07-10
 */
@RestController
@RequestMapping("/system/device")
public class ZDeviceController extends BaseController
{
    @Autowired
    private IZDeviceService zDeviceService;
    @Autowired
    private ClientMgr clientMgr;

    static int test = 500;

    /**
     * 查询设备管理列表
     */
//    @PreAuthorize("@ss.hasPermi('system:device:list')")
    @GetMapping("/list")
    public TableDataInfo list(ZDevice zDevice)
    {
        startPage();
        List<ZDevice> list = zDeviceService.selectZDeviceList(zDevice);
        return getDataTable(list);
    }

    /**
     * 导出设备管理列表
     */
    @PreAuthorize("@ss.hasPermi('system:device:export')")
    @Log(title = "设备管理", businessType = BusinessType.EXPORT)
    @PostMapping("/export")
    public void export(HttpServletResponse response, ZDevice zDevice)
    {
        List<ZDevice> list = zDeviceService.selectZDeviceList(zDevice);
        ExcelUtil<ZDevice> util = new ExcelUtil<ZDevice>(ZDevice.class);
        util.exportExcel(response, list, "设备管理数据");
    }

    /**
     * 获取设备管理详细信息
     */
    @PreAuthorize("@ss.hasPermi('system:device:query')")
    @GetMapping(value = "/{id}")
    public AjaxResult getInfo(@PathVariable("id") Long id)
    {
        return success(zDeviceService.selectZDeviceById(id));
    }

    /**
     * 新增设备管理
     */
//    @PreAuthorize("@ss.hasPermi('system:device:add')")
    @Log(title = "设备管理", businessType = BusinessType.INSERT)
    @PostMapping
    public AjaxResult add(@RequestBody ZDevice zDevice)
    {
        Long zDevicePWM = zDevice.getPercent();
        if(zDevicePWM != null) {
            int mappedPWM = 400 + zDevicePWM.intValue() * 6;

            // 设置PWM占空比
            clientMgr.pwmSetDutyCycle("001", mappedPWM);

            // 输出映射后的值
            System.out.println(mappedPWM);
            System.out.println("1231232131231123");
        }
        return toAjax(zDeviceService.insertZDevice(zDevice));
    }

    /**
     * 修改设备管理
     */
    @PreAuthorize("@ss.hasPermi('system:device:edit')")
    @Log(title = "设备管理", businessType = BusinessType.UPDATE)
    @PutMapping
    public AjaxResult edit(@RequestBody ZDevice zDevice)
    {
        Long zDevicePWM = zDevice.getPercent();
        if(zDevicePWM != null) {
            int mappedPWM = 400 + zDevicePWM.intValue() * 6;

            // 设置PWM占空比
            clientMgr.pwmSetDutyCycle("001", mappedPWM);

            // 输出映射后的值
            zDevice.setSensorPwm(BigDecimal.valueOf(mappedPWM));

        }
        return toAjax(zDeviceService.updateZDevice(zDevice));
    }

    /**
     * 删除设备管理
     */
    @PreAuthorize("@ss.hasPermi('system:device:remove')")
    @Log(title = "设备管理", businessType = BusinessType.DELETE)
	@DeleteMapping("/{ids}")
    public AjaxResult remove(@PathVariable Long[] ids)
    {
        return toAjax(zDeviceService.deleteZDeviceByIds(ids));
    }
}
