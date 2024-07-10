<template>
  <div>
    <el-card class="!border-none" shadow="never">
      <div id="container"></div>
      <el-dialog title="修改路灯信息" :visible.sync="Visible">
        <el-form :model="form">
          <el-form-item label="设备名称" :label-width="formLabelWidth">
            <el-input v-model="form.name" autocomplete="off"></el-input>
          </el-form-item>
          <el-form-item label="纬度" :label-width="formLabelWidth">
            <el-input
              v-model="form.latitude"
              autocomplete="off"
              :disabled="true"
            ></el-input>
          </el-form-item>
          <el-form-item label="经度" :label-width="formLabelWidth">
            <el-input
              v-model="form.longitude"
              autocomplete="off"
              :disabled="true"
            ></el-input>
          </el-form-item>
          <el-form-item label="状态" :label-width="formLabelWidth">
            <!-- <el-select v-model="form.online" placeholder="请选择状态">
              <el-option label="在线" value="1"></el-option>
              <el-option label="不在线" value="0"></el-option>
            </el-select> -->
            <el-switch
              v-model="form.online"
              active-color="#13ce66"
              inactive-color="#ff4949"
            >
              <template #open>开启</template>
              <template #close>关闭</template>
            </el-switch>
          </el-form-item>
          <el-form-item label="亮度" :label-width="formLabelWidth">
            <el-input-number
              v-model="form.percent"
              @change="handleChange"
              :min="0"
              :max="100"
              label="描述文字"
            ></el-input-number>
          </el-form-item>
        </el-form>
        <div slot="footer" class="dialog-footer">
          <el-button @click="Visible = false">取消</el-button>
          <el-button type="primary" @click="updateLamp">确定</el-button>
        </div>
      </el-dialog>
    </el-card>
  </div>
</template>

<script>
import { listDevice, getDevice, updateDevice } from '@/api/system/device'

export default {
  name: 'MapComponent',
  computed: {},
  data() {
    return {
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        deviceId: null,
        name: null,
        online: null,
        percent: null,
        latitude: null,
        longitude: null,
      },
      onlineUrlICON1: require('../assets/images/light.png'), //online 是 1 的话 就显示这个
      onlineUrlICON2: require('../assets/images/light1.png'), //online 是 0 的话 就显示这个
      lamps: [], // 用于存储后端返回的路灯信息,
      Visible: false,
      formLabelWidth: '120px',
      form: {
        online: '1', // 默认值为 '1'
      },
      total: 0, //设备数量
    }
  },
  mounted() {
    this.loadAMapScript(() => {
      this.fetchLampsData()
    })

    setInterval(fn,1000)
  },
  methods: {
    fn() {
      console.log(this.form.online)
    },
    handleChange(value) {
      console.log(value)
    },
    loadAMapScript(callback) {
      const existingScript = document.getElementById('amap-script')
      if (!existingScript) {
        const script = document.createElement('script')
        script.id = 'amap-script'
        script.type = 'text/javascript'
        script.src =
          'https://webapi.amap.com/maps?v=1.4.8&key=08ee87a833b939671a19fed3568a0823'
        script.onload = callback
        document.head.appendChild(script)
      } else {
        callback()
      }
    },
    fetchLampsData() {
      listDevice(this.queryParams).then((response) => {
        this.lamps = response.rows
        this.total = response.total
        this.initializeMap(response.rows)
      })
    },
    initializeMap(ctx) {
      const map = new AMap.Map('container', {
        zoom: 15,
        center: [ctx.longitude, ctx.latitude], // 默认中心点
      })

      const infoWindow = new AMap.InfoWindow({
        isCustom: true, // 使用自定义窗体
        offset: new AMap.Pixel(0, -30), // 信息窗体显示位置偏移量
      })

      this.lamps.forEach((lamp) => {
        const iconImage =
          lamp.online === 1 ? this.onlineUrlICON1 : this.onlineUrlICON2
        const marker = new AMap.Marker({
          position: new AMap.LngLat(lamp.longitude, lamp.latitude),
          title: lamp.name,
          icon: new AMap.Icon({
            image: iconImage,
            size: new AMap.Size(45, 45),
            imageSize: new AMap.Size(45, 45), // 设置图标大小
            imageOffset: new AMap.Pixel(0, 0), // 调整偏移量
          }),
        })

        map.add(marker)

        marker.on('mouseover', () => {
          const content = `
             <div style="background-color: white; padding: 6px; border-radius: 6px; box-shadow: 0 0 8px rgba(0, 0, 0, 0.1);">
              <div><img src="http://webapi.amap.com/images/autonavi.png"></div>
              <div style="padding: 0px 0px 4px;"><b>${lamp.name}</b></div>
              <div style="color: ${
                lamp.online == 1 ? 'green' : 'red'
              };">状态：${lamp.online === 1 ? '在线' : '不在线'}</div>
              <div>经度：${lamp.longitude.toFixed(6)}</div>
              <div>纬度：${lamp.latitude.toFixed(6)}</div>
            </div>
          `
          infoWindow.setContent(content)
          infoWindow.open(map, marker.getPosition())
        })

        marker.on('mouseout', () => {
          infoWindow.close()
        })

        marker.on('click', async () => {
          this.Visible = true
          const id = lamp.id
          await getDevice(id).then((response) => {
            this.form = response.data
            console.log(this.form.online)
            if (this.form.online === 1) {
              this.form.online = '1'
            } else {
              this.form.online = '0'
            }
            console.log(this.form.online)
          })
        })
      })
    },
    updateLamp() {
      if (this.form.online === '1') {
        this.form.online = 1
      } else {
        this.form.online = 0
      }
      updateDevice(this.form).then(() => {
        this.Visible = false
        this.fetchLampsData()
      })
      this.reset()
    },
    reset() {
      this.form = {
        id: null,
        deviceId: null,
        name: null,
        online: null,
        createBy: null,
        createTime: null,
        updateBy: null,
        updateTime: null,
        remark: null,
        percent: null,
        latitude: null,
        longitude: null,
      }
      this.resetForm('form')
    },
  },
}
</script>

<style scoped>
#container {
  width: 100%;
  height: 500px;
}
</style>
