<template>
  <div class="app-container">
    <el-form
      :model="queryParams"
      ref="queryForm"
      size="small"
      :inline="true"
      v-show="showSearch"
      label-width="68px"
    >
      <el-form-item label="设备编号" prop="deviceId">
          <el-select v-model="queryParams.deviceId" placeholder="请选择设备编号">
             <el-option v-for="d in deviceIDS" :key="d" :label="d" :value="d">
          </el-option>
        </el-select>
      </el-form-item>
      <el-form-item label="照明状态" prop="status">
        <el-select v-model="queryParams.status" placeholder="请选择照明状态">
          <el-option label="在线" :value="1"> </el-option>
          <el-option label="不在线" :value="0"> </el-option>
        </el-select>
      </el-form-item>
      <el-form-item label="检查人员" prop="inspectionName">
        <el-select
          v-model="queryParams.inspectionName"
          placeholder="请选择检查人员"
        >
          <el-option v-for="v in user" :key="v" :label="v" :value="v">
          </el-option>
        </el-select>
      </el-form-item>
      <el-form-item label="检查方式" prop="method">
        <el-select v-model="queryParams.method" placeholder="请选择检查方式">
          <el-option label="外观检查" value="外观检查"> </el-option>
          <el-option label="运行情况" value="运行情况"> </el-option>
          <el-option label="部件检查" value="部件检查"> </el-option>
        </el-select>
      </el-form-item>
      <el-form-item label="上报时间">
        <el-date-picker
          v-model="daterangeCreateTime"
          style="width: 240px"
          value-format="yyyy-MM-dd"
          type="daterange"
          range-separator="-"
          start-placeholder="开始日期"
          end-placeholder="结束日期"
        ></el-date-picker>
      </el-form-item>

      <el-form-item>
        <el-button
          type="primary"
          icon="el-icon-search"
          size="mini"
          @click="handleQuery"
          >搜索</el-button
        >
        <el-button icon="el-icon-refresh" size="mini" @click="resetQuery"
          >重置</el-button
        >
      </el-form-item>
    </el-form>

    <el-row :gutter="10" class="mb8">
      <el-col :span="1.5">
        <el-button
          type="primary"
          plain
          icon="el-icon-plus"
          size="mini"
          @click="handleAdd"
          v-hasPermi="['system:inspection:add']"
          >新增</el-button
        >
      </el-col>
      <el-col :span="1.5">
        <el-button
          type="success"
          plain
          icon="el-icon-edit"
          size="mini"
          :disabled="single"
          @click="handleUpdate"
          v-hasPermi="['system:inspection:edit']"
          >修改</el-button
        >
      </el-col>
      <el-col :span="1.5">
        <el-button
          type="danger"
          plain
          icon="el-icon-delete"
          size="mini"
          :disabled="multiple"
          @click="handleDelete"
          v-hasPermi="['system:inspection:remove']"
          >删除</el-button
        >
      </el-col>
      <el-col :span="1.5">
        <el-button
          type="warning"
          plain
          icon="el-icon-download"
          size="mini"
          @click="handleExport"
          v-hasPermi="['system:inspection:export']"
          >导出</el-button
        >
      </el-col>
      <right-toolbar
        :showSearch.sync="showSearch"
        @queryTable="getList"
      ></right-toolbar>
    </el-row>

    <el-table
      v-loading="loading"
      :data="inspectionList"
      @selection-change="handleSelectionChange"
    >
      <el-table-column type="selection" width="55" align="center" />
      <el-table-column label="编号" align="center" type="index" />
      <el-table-column label="设备编号" align="center" prop="deviceId" />
      <el-table-column label="检查情况" align="center" prop="status" />

      <el-table-column label="检查人员" align="center" prop="inspectionName" />
      <el-table-column label="检查方式" align="center" prop="method" />
      <el-table-column
        label="上报时间"
        align="center"
        prop="createTime"
        width="180"
      >
        <template slot-scope="scope">
          <span>{{ parseTime(scope.row.createTime, '{y}-{m}-{d}') }}</span>
        </template>
      </el-table-column>
     
      <el-table-column label="备注" align="center" prop="remark" />
      <el-table-column
        label="操作"
        align="center"
        class-name="small-padding fixed-width"
      >
        <template slot-scope="scope">
          <el-button
            size="mini"
            type="text"
            icon="el-icon-edit"
            @click="handleUpdate(scope.row)"
            v-hasPermi="['system:inspection:edit']"
            >修改</el-button
          >
          <el-button
            size="mini"
            type="text"
            icon="el-icon-delete"
            @click="handleDelete(scope.row)"
            v-hasPermi="['system:inspection:remove']"
            >删除</el-button
          >
        </template>
      </el-table-column>
    </el-table>

    <pagination
      v-show="total > 0"
      :total="total"
      :page.sync="queryParams.pageNum"
      :limit.sync="queryParams.pageSize"
      @pagination="getList"
    />

    <!-- 添加或修改设备巡检对话框 -->
    <el-dialog :title="title" :visible.sync="open" width="500px" append-to-body>
      <el-form ref="form" :model="form" :rules="rules" label-width="80px">
        <el-form-item label="设备编号" prop="deviceId">
        <el-select  v-model="form.deviceId"
            placeholder="请选择设备编号"
            style="width: 90%">
          <el-option v-for="v in deviceIDS" :key="v" :label="v" :value="v">   </el-option>
        </el-select>
        </el-form-item>
        <el-form-item label="检查人员" prop="inspectionName">
          <el-select
            v-model="form.inspectionName"
            placeholder="请选择检查人员"
           style="width: 90%"
          >
            <el-option v-for="v in user" :key="v" :label="v" :value="v">
            </el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="检查方式" prop="method">
          <el-select v-model="form.method"   style="width: 90%" placeholder="请选择检查方式">
            <el-option label="外观检查" value="外观检查"> </el-option>
            <el-option label="运行情况" value="运行情况"> </el-option>
            <el-option label="部件检查" value="部件检查"> </el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="照明状态" prop="status">
          <el-select v-model="form.status" placeholder="请选择照明状态"    style="width: 90%" >
            <el-option label="开灯" :value="1"> </el-option>
            <el-option label="关灯" :value="0"> </el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="备注" prop="remark">
          <el-input
            v-model="form.remark"
            type="textarea"
            placeholder="请输入内容"
             style="width: 90%"
          />
        </el-form-item>
      </el-form>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="submitForm">确 定</el-button>
        <el-button @click="cancel">取 消</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import {
  listInspection,
  getInspection,
  delInspection,
  addInspection,
  updateInspection,
} from '@/api/system/inspection'
import {
  listDevice,
  
} from '@/api/system/device'

import { listUser } from '@/api/system/user'

export default {
  name: 'Inspection',
  data() {
    return {
      // 遮罩层
      loading: true,
      // 选中数组
      ids: [],
      // 非单个禁用
      single: true,
      // 非多个禁用
      multiple: true,
      // 显示搜索条件
      showSearch: true,
      // 总条数
      total: 0,
      // 设备巡检表格数据
      inspectionList: [],
      // 弹出层标题
      title: '',
      // 是否显示弹出层
      open: false,
      // 备注时间范围
      daterangeCreateTime: [],
      queryPar: {
        pageNum: 1,
        pageSize: 10,
      },
      // 查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        deviceId: null,
        status: null,
        userId: null,
        inspectionName: null,
        method: null,
        createTime: null,
        updateBy: null,
      },
      // 表单参数
      form: {},
      // 表单校验
      rules: {
        deviceId: [
          { required: true, message: '设备id不能为空', trigger: 'blur' },
        ],
        status: [
          { required: true, message: '检查情况不能为空', trigger: 'change' },
        ],
        inspectionName: [
          { required: true, message: '检查人员不能为空', trigger: 'blur' },
        ],
        method: [
          { required: true, message: '检查方式不能为空', trigger: 'blur' },
        ],
      },
      user: [],
      postGroup: '',
      roleGroup: '',
      deviceIDS: [],
    }
  },
  created() {
    this.getList()
    this.getUser()
  },
  methods: {
    /** 查询设备巡检列表 */
    async getUser() {
      await listUser(this.addDateRange(this.queryPar)).then((response) => {
        this.user = response.rows.map((v) => v.userName)
        console.log(this.user)
      })
      listDevice(this.queryParams).then((response) => {
        this.deviceIDS = response.rows.map(v => v.deviceId)
        console.log(this.deviceIDS);
        
      })
    },
    /** 查询设备巡检列表 */
    getList() {
      this.loading = true
      this.queryParams.params = {}
      if (null != this.daterangeCreateTime && '' != this.daterangeCreateTime) {
        this.queryParams.params['beginCreateTime'] = this.daterangeCreateTime[0]
        this.queryParams.params['endCreateTime'] = this.daterangeCreateTime[1]
      }
      listInspection(this.queryParams).then((response) => {
        this.inspectionList = response.rows
        this.total = response.total
        this.loading = false
      })
    },
    // 取消按钮
    cancel() {
      this.open = false
      this.reset()
    },
    // 表单重置
    reset() {
      this.form = {
        id: null,
        deviceId: null,
        status: null,
        userId: null,
        inspectionName: null,
        method: null,
        createBy: null,
        createTime: null,
        updateBy: null,
        updateTime: null,
        remark: null,
      }
      this.resetForm('form')
    },
    /** 搜索按钮操作 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery() {
      this.daterangeCreateTime = []
      this.resetForm('queryForm')
      this.handleQuery()
    },
    // 多选框选中数据
    handleSelectionChange(selection) {
      this.ids = selection.map((item) => item.id)
      this.single = selection.length !== 1
      this.multiple = !selection.length
    },
    /** 新增按钮操作 */
    handleAdd() {
      this.reset()
      this.open = true
      this.title = '添加设备巡检'
    },
    /** 修改按钮操作 */
    handleUpdate(row) {
      this.reset()
      const id = row.id || this.ids
      getInspection(id).then((response) => {
        this.form = response.data
        this.open = true
        this.title = '修改设备巡检'
      })
    },
    /** 提交按钮 */
    submitForm() {
      this.$refs['form'].validate((valid) => {
        if (valid) {
          if (this.form.id != null) {
            updateInspection(this.form).then((response) => {
              this.$modal.msgSuccess('修改成功')
              this.open = false
              this.getList()
            })
          } else {
            console.log(this.form);
            addInspection(this.form).then((response) => {
              this.$modal.msgSuccess('新增成功')
              this.open = false
              this.getList()
            })
          }
        }
      })
    },
    /** 删除按钮操作 */
    handleDelete(row) {
      const ids = row.id || this.ids
      this.$modal
        .confirm('是否确认删除设备巡检编号为"' + ids + '"的数据项？')
        .then(function () {
          return delInspection(ids)
        })
        .then(() => {
          this.getList()
          this.$modal.msgSuccess('删除成功')
        })
        .catch(() => {})
    },
    /** 导出按钮操作 */
    handleExport() {
      this.download(
        'system/inspection/export',
        {
          ...this.queryParams,
        },
        `inspection_${new Date().getTime()}.xlsx`
      )
    },
  },
}
</script>
