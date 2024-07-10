<template>
  <div>
    <div class="info-boxes">
      <div class="info-box info-box-1">
        <div class="info-box-header">设备数量</div>
        <div class="info-box-icon">
          <i class="el-icon-coin"></i>
        </div>
        <div class="info-box-value">{{ total }}</div>
      </div>
      <div class="info-box info-box-2">
        <div class="info-box-header">在线数量</div>
        <div class="info-box-icon">
          <i class="el-icon-coin"></i>
        </div>
        <div class="info-box-value">{{ onlineCount }}</div>
      </div>
      <div class="info-box info-box-3">
        <div class="info-box-header">离线数量</div>
        <div class="info-box-icon">
          <i class="el-icon-coin"></i>
        </div>
        <div class="info-box-value">{{ offlineCount }}</div>
      </div>
      <div class="info-box info-box-4">
        <div class="info-box-header">今日数量</div>
        <div class="info-box-icon">
          <i class="el-icon-coin"></i>
        </div>
        <div class="info-box-value">{{ total }}</div>
      </div>
    </div>

    <el-card class="!border-none" shadow="never">
      <div id="container" style="border-radius: 6px"></div>
      <el-dialog title="修改路灯信息" :visible.sync="Visible" width="34%">
        <el-form :model="form" label-width="100px">
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
          <el-form-item label="照明状态" :label-width="formLabelWidth">
            <el-switch
              v-model="form.online"
              active-color="#13ce66"
              inactive-color="#ff4949"
              :active-value="1"
              :inactive-value="0"
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
      onlineUrlICON1: require('../assets/images/light.png'), // online 是 1 的话 就显示这个
      onlineUrlICON2: require('../assets/images/light1.png'), // online 是 0 的话 就显示这个
      lamps: [], // 用于存储后端返回的路灯信息
      Visible: false,
      formLabelWidth: '100px',
      form: {},
      total: 0, // 设备数量
      onlineCount: 0,
      offlineCount: 0,
      todayCount: 0,
    }
  },
  mounted() {
    this.loadAMapScript(() => {
      this.fetchLampsData()
    })
  },
  methods: {
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
          "https://webapi.amap.com/maps?v=1.4.8&key=08ee87a833b939671a19fed3568a0823"
        script.onload = callback
        document.head.appendChild(script)
      } else {
        callback()
      }
    },
   async fetchLampsData() {
     await listDevice(this.queryParams).then((response) => {
        this.lamps = response.rows
        this.total = response.total
        this.calculateCounts()
        console.log(this.lamps);
        this.initializeMap(response.rows)
      })
    },
    calculateCounts() {
      this.onlineCount = this.lamps.filter((lamp) => lamp.online === 1).length
      this.offlineCount = this.lamps.filter((lamp) => lamp.online === 0).length

      // Assuming 'todayCount' is the number of devices added today
      const today = new Date().toISOString().slice(0, 10)
      this.todayCount = this.lamps.filter(
        (lamp) => lamp.createTime && lamp.createTime.slice(0, 10) === today
      ).length
    },
    initializeMap(ctx) {
      if (!ctx || !ctx.length || !ctx[0].longitude || !ctx[0].latitude) {
        console.error('Invalid coordinates for map initialization')
        return
      }

      const map = new AMap.Map('container', {
        zoom: 18,
        center: [ctx[0].longitude, ctx[0].latitude], // 默认中心点
      })
 
      const infoWindow = new AMap.InfoWindow({
        isCustom: true, // 使用自定义窗体
        offset: new AMap.Pixel(0, -30), // 信息窗体显示位置偏移量
      })
    

      this.lamps.forEach((lamp) => {
        if (!lamp.longitude || !lamp.latitude) {
          console.error(`Invalid coordinates for lamp name // 请求没有 ${lamp.name}`);
          return;
        }
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
              <div >状态：<span style="color: ${
                lamp.online == 1 ? 'green' : 'block'
              };">${lamp.online === 1 ? '开灯' : '关灯'}</span></div>
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
          const id = lamp.id
          await getDevice(id).then((response) => {
            this.form = response.data
            if (response.code === 200) {
              this.Visible = true
            }
          })
        })
      })
    },
    updateLamp() {
      updateDevice(this.form).then((res) => {
        if (res.code === 200) {
          this.Visible = false
          this.fetchLampsData()
          this.$notify({
            title: '成功',
            message: '数据修改成功 ！',
            type: 'success',
          })
        }
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
  height: 70vh;
}
.info-boxes {
  display: flex;
  justify-content: space-around;
  margin-bottom: 10px;
  margin-top: 15px;
}
.info-box {
  border-radius: 10px;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
  padding: 10px 20px;
  color: white;
  font-size: 18px;
  font-family: Arial, sans-serif;
  position: relative;
  width: 22%;
  height: 100px;
}
.info-box-header {
  position: absolute;
  top: 10px;
  left: 10px;
  font-size: 14px;
  font-weight: bold;
}
.info-box-icon {
  position: absolute;
  bottom: 15px;
  left: 15px;
  width: 24px;
  height: 24px;
}
.info-box-value {
  position: absolute;
  bottom: 15px;
  right: 15px;
  font-size: 24px;
}
.info-box-1 {
  background: linear-gradient(45deg, #ec4786, #b955a4);
}
.info-box-2 {
  background: linear-gradient(45deg, #ffb347, #ffcc33);
}
.info-box-3 {
  background: linear-gradient(45deg, #00c6ff, #0072ff);
}
.info-box-4 {
  background: linear-gradient(45deg, #24c6dc, #514a9d);
}
</style>
