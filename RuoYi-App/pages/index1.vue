<template>
	<view class="all">
		<view class="ip-app">
			<!--页头-->
			<view class="ip-head">
				<text>资产管理</text>
				<text>></text>
				<text>二维码生成</text>
			</view>
			<view class="QR_switchover">
				<text>二维码生成</text>
			</view>
			<view class="QRCode_seek">
				<view>请输入内容：</view>
				<input v-model="txt" placeholder="请输入内容" />
				<button @click="create" type="primary">生成</button>
			</view>
			<view class="qr_center">
				<canvas canvas-id="canvass" style="height: 300px; width: 300px"></canvas>
			</view>
		</view>
	</view>
</template>

<script>
import QRCode from 'qrcode';
import { getUserProfile } from '@/api/system/user';

export default {
	data() {
		return {
			msg: '江南无所有、聊赠一枝春',
			txt: ''
		};
	},
	mounted() {
		this.useqrcode(this.msg);
	},
	methods: {
		useqrcode(txt) {
			QRCode.toDataURL(txt, { width: 300, height: 300 }, (error, url) => {
				if (error) {
					console.error('生成二维码失败:', error);
					return;
				}
				const canvas = uni.createCanvasContext('canvass');
				uni.getImageInfo({
					src: url,
					success: (image) => {
						canvas.drawImage(image.path, 0, 0, 300, 300);
						canvas.draw();
					}
				});
			});
		},
		create() {
			if (!this.txt && this.txt !== '0') {
				uni.showToast({
					title: '请输入生成值',
					icon: 'none'
				});
			} else {
				this.useqrcode(this.txt);
				uni.showToast({
					title: '恭喜你，生成成功',
					icon: 'success'
				});
			}
		},
		// 上传
		updataXJ() {
			getUserProfile().then((response) => {
				this.user = response.data;
				this.roleGroup = response.roleGroup;
				this.postGroup = response.postGroup;
			});
		}
	}
};
</script>

<style>
.all {
	height: 100%;
	width: 100%;
	padding: 1% 2%;
}
.QR_switchover {
	height: 80px;
	background-color: #ffffff;
	line-height: 80px;
	padding-left: 2%;
	font-size: 1.8em;
}
.qr_center {
	background-color: #fff;
	margin-top: 2%;
	text-align: center;
}
.QRCode_seek {
	padding: 2% 3%;
	background-color: #fff;
}
.QRCode_seek > view {
	display: inline-block;
}
.QRCode_seek > view:last-child {
	margin-left: 2%;
}
</style>
