<template>
	<view class="content">
		<map
			style="width: 100%; height: 90vh"
			:layer-style="5"
			:style="{ height: mapheight }"
			:show-location="true"
			:latitude="latitude"
			:longitude="longitude"
			:markers="markers"
			:scale="scale"
			@markertap="markertap"
			@callouttap="callouttap"
		></map>
	</view>
</template>

<script>
import { listDevice, getDevice,   } from '@/api/system/device.js';
export default {
	data() {
		return {
			latitude: 22.781724951481600000, //纬度
			longitude: 108.467640627786980000, //经度
			scale: 17, //缩放级别
			bottomData: false,
			markers: [], // 将根据数据生成的标记点列表
			eventHandled: false, // 标志位
			queryParams: {
				pageNum: 1,
				pageSize: 10,
				deviceId: null,
				name: null,
				online: null,
				percent: null,
				latitude: null,
				longitude: null
			},
			total: 0 //设备数量
		};
	},
	onLoad() {
		this.loadMarkers();
	},
	computed: {
		mapheight() {
			let data = '';
			if (this.bottomData) {
				data = this.upTop ? '50px' : '200px';
			} else {
				data = '90vh';
			}
			return data;
		},
		coverbottom() {
			return this.bottomData ? '20rpx' : '100rpx';
		}
	},
	methods: {
		async loadMarkers() {
			// 模拟从API获取的数据
			let data = [];
			await listDevice(this.queryParams).then((res) => {
				console.log(res);
				data = res.rows;
			});
			this.markers = data.map((lamp) => ({
				
				id: lamp.id,
				latitude: lamp.latitude,
				longitude: lamp.longitude,
				iconPath: lamp.online === 1 ? '../static/images/light.png' : '../static/images/light1.png', // 图标路径
				rotate: 0,
				width: 40,
				height: 36,
				alpha: 0.5,
				callout: {
					content: lamp.name, // 文本
					color: '#ffffff', // 文字颜色
					fontSize: 12, // 文本大小
					borderRadius: 15, // 边框圆角
					borderWidth: 1, // 边框宽度
					bgColor: lamp.online === 1 ? 'green' : 'red', // 背景颜色根据online状态设置
					display: 'ALWAYS' // 常显
				}
			}));
		},
		handleEvent(e) {
			// if (!this.eventHandled) {
			// 	this.eventHandled = true;
			// 	console.log('===你点击了标记点或气泡窗口===', e);
			// 	setTimeout(() => {
			// 		this.eventHandled = false;
			// 	}, 300);
		},
		markertap(e) {
			if (!this.eventHandled) {
				this.eventHandled = true;
				console.log('===你点击了标记点', e);
				setTimeout(() => {
					this.eventHandled = false;
				}, 300);
				}
		},
		//地图点击事件
		callouttap(e) {
			if (!this.eventHandled) {
				this.eventHandled = true;
				console.log('----------', e);
				setTimeout(() => {
					this.eventHandled = false;
				}, 300);
				}
		}
	}
};
</script>

<style scoped>
.content {
	width: 100%;
	height: 100vh;
}
</style>
