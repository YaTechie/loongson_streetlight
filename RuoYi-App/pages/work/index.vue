<template>
	  <div class="page-container">
	    
	   <!-- <div class="device-dropdown">
	      <el-dropdown @command="handleDeviceChange" trigger="click">
	        <span class="el-dropdown-link">
	          切换设备 <i class="el-icon-arrow-down el-icon--right"></i>
	        </span>
	        <el-dropdown-menu slot="dropdown">
	          <el-dropdown-item v-for="device in deviceList" :key="device.id" :command="device.id">
	            {{ device.name }}
	          </el-dropdown-item>
	        </el-dropdown-menu>
	      </el-dropdown>
	    </div> -->
	<div class="info-boxes">
		
		<div class="info-box info-box-1 info-box-R">
			<div class="info-box-header">风速</div>
			<div class="info-box-icon">
				<!-- <i class="el-icon-wind"></i> -->
				<uni-icons class="el-icon-wind" type="fire" size="30"></uni-icons>
			</div>
			<div class="info-box-value">{{ data.windspeed }}</div>
		</div>

		<div class="info-box info-box-2">
			<div class="info-box-header">风向</div>
			<div class="info-box-icon">
				<uni-icons class="el-icon-wind" type="fire" size="30"></uni-icons>
			</div>
			<div class="info-box-value">{{ data.direction }}</div>
		</div>

		<div class="info-box info-box-3">
			<div class="info-box-header">角度</div>
			<div class="info-box-icon">
				<uni-icons class="el-icon-wind" type="fire" size="30"></uni-icons>
			</div>
			<div class="info-box-value">{{ data.angle }}</div>
		</div>

		<div class="info-box info-box-4">
			<div class="info-box-header">光照</div>
			<div class="info-box-icon">
				<uni-icons class="el-icon-wind" type="fire" size="30"></uni-icons>
			</div>
			<div class="info-box-value">{{ data.sensorLight }}</div>
		</div>

		<div class="info-box info-box-5">
			<div class="info-box-header">温度</div>
			<div class="info-box-icon">
				<uni-icons class="el-icon-wind" type="fire" size="30"></uni-icons>
			</div>
			<div class="info-box-value">{{ data.sensorHumidity }}</div>
		</div>

		<div class="info-box info-box-6">
			<div class="info-box-header">湿度</div>
			<div class="info-box-icon">
				<uni-icons class="el-icon-wind" type="fire" size="30"></uni-icons>
			</div>
			<div class="info-box-value">{{ data.sensorTemperature }}</div>
		</div>

		<div class="info-box info-box-7">
			<div class="info-box-header">亮度</div>
			<div class="info-box-icon">
				<uni-icons class="el-icon-wind" type="fire" size="30"></uni-icons>
			</div>
			<div class="info-box-value">{{ data.percent }}</div>
		</div>

		<div class="info-box info-box-8">
			<div class="info-box-header">空气</div>
			<div class="info-box-icon">
				<uni-icons class="el-icon-wind" type="fire" size="30"></uni-icons>
			</div>
			<div class="info-box-value">{{ data.sensorPm2 }}</div>
		</div>
	</div>
	    </div>
</template>
<script>
import { getDevice } from '@/api/system/device.js';

export default {
  data() {
    return {
      data: {
        windspeed: '1.5', // 米每秒
        direction: '东南风',
        angle: '1°',
        sensorLight: '1',
        sensorHumidity: '25.3 ℃',
        sensorTemperature: '28.7',
        sensorPwm: '1 %',
        sensorPm2: '15.2'
      },
      ids: '001',
      timer: null,
      throttleTime: 3000 ,// 节流时间设置为3秒
			deviceList: []
    };
  },
  created() {
    this.getList();
  },
  mounted() {
    this.startPolling();
  },
  beforeDestroy() {
    this.clearTimer();
  },
  methods: {
    getList() {
      getDevice(this.ids).then((response) => {
        console.log(response);
        if (response.data.sensorPwm === null) {
          this.data.sensorPwm = '0';
        } else {
          this.data = response.data;
        }
        // 请求完成后再次启动定时器
        this.startPolling();
      });
    },
    startPolling() {
      // 清除现有定时器
      this.clearTimer();
      // 设置新的定时器
      this.timer = setTimeout(() => {
        this.getList();
      }, this.throttleTime);
    },
    clearTimer() {
      clearTimeout(this.timer);
    }
  }
};
</script>

<style lang="scss">
/* 省略部分样式 */
</style>

<style lang="scss">
	.page-container {
	  position: relative;  
	}
	
	.device-dropdown {
	  position: absolute;
	  top: 20px;
	  right: 20px;
	}
.info-boxes {
	display: flex;
	flex-wrap: wrap;
	justify-content: space-around;
	margin-bottom: 10px;
	margin-top: 15px;
}

.info-box {
	border-radius: 10px;
	box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
	padding: 10px 10px;
	color: white;
	font-size: 18px;
	font-family: Arial, sans-serif;
	position: relative;
	width: 42%;
	height: 100px;
	margin-bottom: 20px;
}

.info-box-header {
	position: absolute;
	top: 10px;
	left: 10px;
	font-size: 14px;
	font-weight: bold;
}
.info-box-R {
	position: relative;
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

.info-box-5 {
	background: linear-gradient(45deg, #ad5389, #3c1053);
}

.info-box-6 {
	background: linear-gradient(45deg, #ff7eb3, #ff6542);
}

.info-box-7 {
	background: linear-gradient(45deg, #d4fc79, #96e6a1);
}

.info-box-8 {
	background: linear-gradient(45deg, #ffafbd, #ffc3a0);
}
</style>
