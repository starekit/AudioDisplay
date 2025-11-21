#pragma once
#include <string>
namespace html{

const std::string wifiConfigHtml=R"(
<!DOCTYPE html>
<html>
	
<head>
	<title>ESP32 联网配置界面</title>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1"> 
	<style>
		body {
			font-family: Arial;
			text-align: center;
			margin: 0;
			padding: 0px;
			background-color: aliceblue;
		}

		.wifi-form {
			width: 80%;
			max-width: 500px;
			margin: 50px auto;
			padding: 20px;
			border-radius: 10px;
			box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);
			background-color: rgb(255, 252, 252);
		}

		.wifi-select {
			display: block;
			width: 80%;
			margin: 15px auto;
			padding: 8px;
			font-size: 16px;
		}

		.wifi-input {
			display: block;
			width: 76%;
			margin: 15px auto;
			padding: 8px;
			font-size: 16px;
		}

		.submit-btn {
			display: block;
			margin: 20px auto;
			padding: 10px 20px;
			font-size: 16px;
			background-color: #4CAF50;
			color: white;
			border: none;
			border-radius: 5px;
			cursor: pointer;
		}
	</style>
</head>

<body>
	<h1 style="text-align: center;">配置WiFi连接</h1>
	<form class="wifi-form" method="POST" action="/save">
		<label style="display: block; text-align: center;">选择WiFi网络 (SSID):</label>
		<select class="wifi-select" name="ssid" required>
			<option value="">-- 请选择WiFi --</option>
			%SSID_OPTIONS%
		</select>

		<label style="display: block; text-align: center;">WiFi密码:</label>
		<input class="wifi-input" type="password" name="password" placeholder="无密码则留空">

		<input class="submit-btn" type="submit" value="保存并连接">

	</form>
	<p id="status" style="text-align: center;">%STATUS%</p> 
</body>
</html>
)";


const std::string indexHtml=R"(

<!DOCTYPE html>
<html lang="en">
<head>
<title>Esp32控制台</title>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<style>
	* {
		margin: 0;
		padding: 0;
		box-sizing: border-box;
		font-family: Arial, sans-serif;
	}
	body{
		padding: 0;
	}
	.menu-bar {
		position: fixed;
		width: 100%;
		height: 50px;
		display: flex;
		justify-content:center;
		background-color: #2c3e50;
		flex-wrap: wrap; /* 支持小屏幕换行 */
		z-index: 1000;
	}
	.button{
		display: inline-block;
		padding: 8px 20px;
		font-size: 1em;
		text-decoration: none;
		color: white;
		background-color: lightgray;
		border-radius: 5px;
		cursor: pointer;
		transition:background-color 0.3s;
		box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
	}
	.button:hover{
		background-color: lightgray;
	}

	.menu-item {
		padding: 2px 25px;
		color: white;
		border: none;
		background: none;
		cursor: pointer;
		font-size: 16px;
		border-radius: 5px;
		transition: background-color 0.3s;
	}
	.menu-item:hover {
		background-color: #34495e;
	}
	.menu-item.active {
		background-color: #4CAF50; /* 选中项高亮 */
	}

	.content {
		text-align: center;
		margin-top: 60px;
		width: 100%;
		height:calc(100vh - 60px);
		padding: 10px;
		display: none; /* 默认隐藏所有内容 */
	}
	.content.active {
		display: block; /* 只显示当前激活的内容 */
	}
	.container{
		margin: 0 auto;
		position: relative;
		overflow: hidden;
		width: 100%;
		height: 100vh;
		background: linear-gradient(90deg, #ebbd92 0,#a96aa2 58%);
	}
	.upload-box {
		margin-top: 20px;
	}


</style>
<style>
	.home-box{
		display: flex;
		height: 100%;
		width: 100%;
		gap: 20px;
	}
	.left-content {
		flex: 1;
		display: flex;
		flex-direction: column;
		gap: 20px;
	}
	.rightView {
		width: 300px;
		border-radius: 10px;
		background-color: rgba(255, 255, 255, 0.29);
		-webkit-backdrop-filter: blur(5px);
		backdrop-filter: blur(50px);
		padding: 20px;
		display: flex;
		flex-direction: column;
		align-items: center;
	}



	.status-cards {
		display: flex;
		justify-content: center;
		gap: 20px;
		width: 100%;
		margin-bottom: 20px;
	}

	.humidity, .temperature, .LED,.cpuUsage,.color {
		display: flex;
		flex-direction: column;
		align-items: center;
		justify-content: center;
		font-size: 1.2em;
		width: 200px;
		height: 200px;
		background-color: rgba(255, 255, 255, 0.4);
		border-radius: 10px;
		-webkit-backdrop-filter: blur(5px);
		backdrop-filter: blur(5px);
		transition: transform 0.3s ease,box-shadow 0.3s ease;
		padding: 15px;
	}
	.humidity:hover,.temperature:hover, .LED:hover,.cpuUsage:hover,.color:hover {
		transform: scale(1.1);
		box-shadow: 0 8px 16px rgba(0,0,0,0.2); /* 放大时阴影加深 */
	}
	.humidity-text, .temp-text, .led-text,.cpuUsage-text,.color-text {
		font-size: 1.1rem;
		font-weight: 700;
		color: rgba(0, 0, 0, 0.76);
		margin-bottom: 5px;
	}
	.humidity-show {
		position: relative;
		width: 120px;
		height: 120px;
		margin: 0 auto;
	}
	.cpuUsage-show{
		position: relative;
		width: 120px;
		height: 120px;
		margin: 0 auto;
	}

	.humidity-percent-text,.cpu-percent-text {
		position: absolute;
		top: 50%;
		left: 50%;
		transform: translate(-50%, -50%);
		font-size: 28px;
		font-weight: bold;
		color: #333;
		opacity: 0;
		animation: fadeIn 1s 0.3s forwards;
	}
	/* 进度环动画过渡 */
	#cpu-progress-ring,#hum-progress-ring{
		/* 核心：stroke-dashoffset 变化时平滑过渡（1.5秒动画） */
		transition: stroke-dashoffset 1.5s cubic-bezier(0.34, 1.56, 0.64, 1);
		/* 贝塞尔曲线让动画先快后慢，更自然 */
	}
	/* 文本淡入动画 */
	@keyframes fadeIn {
		to { opacity: 1; }
	}


	.temp-number {
		font-size: 3.5rem;
		font-weight: 700;
		color: #4a90e2;
		transition: color 1s ease;
		line-height: 1;
		margin: 10px 0;
	}
	.led-controls {
		display: flex;
		flex-direction: column;
		gap: 10px;
		width: 100%;
	}
	.button.on{
		padding: 10px 10px;
		background: #4CAF50;
		margin: 0;
	}
	.button.on:hover{
		background: #45a049;
	}
	.button.on:active{
		background: #073709;
	}
	.button.off{
		padding: 10px;
		background: #f44336;
		margin: 0;
	}
	.button.off:hover{
		background: #d32f2f;
	}
	.button.off:active{
		background: #5a0606;
	}

	.show{
		flex: 1;
		border-radius: 10px;
		background-color: rgba(255, 255, 255, 0.4);
		-webkit-backdrop-filter: blur(5px);
		backdrop-filter: blur(5px);
		padding: 20px;
		display: flex;
		align-items: center;
		justify-content: center;
		font-size: 1.5rem;
		color: rgba(0, 0, 0, 0.7);
		flex-direction: column;
	}
</style>
<style>
	.info-grid {
		margin: 20px auto;
		text-align: center;
		display: grid;
		grid-template-columns: repeat(auto-fit, minmax(100px, 200px));
		gap: 10px;
		max-width: 700px;
		max-height: 700px;
		transform: translate(70px, 0);

	}
	.info-card {
		width: 150px;
		height: 90px;
		padding: 5px;
		border: 1px solid #eee;
		border-radius: 6px;
		background-color: rgba(255, 255, 255, 0.4);
		-webkit-backdrop-filter: blur(5px);
		backdrop-filter: blur(5px);
	}
	.info-label {
		font-size: 14px;
		color: #666;
		margin-bottom: 5px;
	}
	.info-value {
		font-size: 18px;
		font-weight: bold;
		color: #2c3e50;
	}

</style>
<style>

	.progress {
		margin-top: 20px;
		height: 20px;
		background-color: #eee;
		border-radius: 10px;
		overflow: hidden;
		display: none;
	}
	.progress-bar {
		height: 100%;
		background-color: #4CAF50;
		width: 50%;
		transition: width 0.3s;
	}
</style>
<style>
	.sensor-chart {
		width: 80%;
		height: 60%;
		border: 1px solid #eee;
		border-radius: 6px;
		margin: 100px auto;
		display: flex;
		align-items: center;
		justify-content: center;
		color: #404c85;
		background-color: rgba(255, 255, 255, 0.3);
		-webkit-backdrop-filter: blur(5px);
		backdrop-filter: blur(5px);
	}

	/* 响应式调整（小屏幕适配） */
	@media (max-width: 600px) {
		.menu-item {
			flex-basis: 50%; /* 小屏幕每行显示2个菜单 */
			text-align: center;
		}
		.content {
			padding: 15px;
		}
	}
</style>
</head>
<body>
<div class="container">
	<div class="menu-bar">
		<button class="menu-item active" data-target="home">主页</button>
		<button class="menu-item" data-target="device-info">设备信息</button>
		<button class="menu-item" data-target="sensor-data">传感器数据</button>
		<button class="menu-item" data-target="firmware-update">固件更新</button>
		<button class="menu-item" data-target="system-settings">系统设置</button>


	</div>

	<div class="content active" id="home">
		<div class="home-box">
			<div class="left-content">
				<div class="status-cards">
					<div class="cpuUsage">
						<div class="cpuUsage-text">CPU利用率</div>
						<div class="cpuUsage-show" id="showCpuUsage">
							<svg width="100%" height="100%" viewBox="0 0 100 100">
								<circle
										cx="50" cy="50" r="40"
										fill="none"
										stroke="#eee"
										stroke-width="8"
								/>
								<circle
										id="cpu-progress-ring"
										cx="50" cy="50" r="40"
										fill="none"
										stroke="#3498db"
										stroke-width="8"
										stroke-linecap="round"
										transform="rotate(-90 50 50)\n"
										stroke-dasharray="251.2"
										stroke-dashoffset="251.2"
								/>
							</svg>
							<div class="cpu-percent-text">0%</div>
						</div>

					</div>
					<div class="humidity">
						<div class="humidity-text">湿度</div>
						<div class="humidity-show" id="showHumidity">
							<svg width="100%" height="100%" viewBox="0 0 100 100">
								<circle
										cx="50" cy="50" r="40"
										fill="none"
										stroke="#eee"
										stroke-width="8"
								/>
								<circle
										id="hum-progress-ring"
										cx="50" cy="50" r="40"
										fill="none"
										stroke="#3498db"
										stroke-width="8"
										stroke-linecap="round"
										transform="rotate(-90 50 50)\n"
										stroke-dasharray="251.2"
										stroke-dashoffset="251.2"
								/>
							</svg>
							<div class="humidity-percent-text">0%</div>
						</div>
					</div>

					<div class="temperature">
						<div class="temp-text">温度</div>
						<div class="temp-number" id="tempDisplay">10°C</div>
					</div>

					<div class="LED">
						<div class="led-text">LED控制</div>
						<div class="led-controls">
							<div class="button on" id="btnOn">打开LED</div>
							<div class="button off" id="btnOff">关闭LED</div>
						</div>
					</div>

					<div class="color">
						<div class="color-text">颜色控制</div>


					</div>
				</div>
				<div class="show">
						数据可视化区域
				</div>
			</div>
			<div class="rightView">
				<h3>设备状态</h3>
			</div>
		</div>
	</div>

	<div class="content" id="device-info">
		<h2>设备信息</h2>
		<p>当前设备的基本状态和配置</p>
		<div class="info-grid">
			<div class="info-card">
				<div class="info-label">设备名称</div>
				<div class="info-value" id="device-name">ESP32-DevKitC</div>
			</div>
			<div class="info-card">
				<div class="info-label">IP地址</div>
				<div class="info-value" id="device-ip">加载中...</div>
			</div>
			<div class="info-card">
				<div class="info-label">固件版本</div>
				<div class="info-value" id="firmware-version">v1.0.0</div>
			</div>
		</div>
		<div class="info-grid">
			<div class="info-card">
				<div class="info-label">运行时间</div>
				<div class="info-value" id="uptime">0小时</div>
			</div>
			<div class="info-card">
				<div class="info-label">WiFi信号</div>
				<div class="info-value" id="wifi-signal">-- dBm</div>
			</div>
			<div class="info-card">
				<div class="info-label">存储空间</div>
				<div class="info-value" id="storage">-- MB / -- MB</div>
			</div>

		</div>

	</div>

	<div class="content" id="sensor-data">
		<h2>传感器实时数据</h2>
		<p>环境传感器监测数据（每3秒更新一次）</p>
		<div class="info-grid">
			<div class="info-card">
				<div class="info-label">温度</div>
				<div class="info-value" id="sensor-temp">-- °C</div>
			</div>
			<div class="info-card">
				<div class="info-label">湿度</div>
				<div class="info-value" id="sensor-humi">-- %</div>
			</div>
			<div class="info-card">
				<div class="info-label">光照</div>
				<div class="info-value" id="sensor-light">-- lux</div>
			</div>
		</div>
		<div class="sensor-chart">
			温度趋势图（示例区域）
		</div>
	</div>

	<div class="content" id="system-settings">
		<h2>系统设置</h2>
		<p>配置设备的基本参数</p>
		<form id="settings-form">
			<div style="margin: 15px 40%;">
				<label for="wifi-ssid">WiFi名称</label><br>
				<input type="text" id="wifi-ssid" style="width: 100%; padding: 8px; margin-top: 5px;" placeholder="输入WiFi名称">
			</div>
			<div style="margin: 15px 40%;">
				<label for="wifi-password">WiFi密码</label><br>
				<input type="password" id="wifi-password" style="width: 100%; padding: 8px; margin-top: 5px;" placeholder="输入WiFi密码">
			</div>
			<div style="margin: 15px 40%;">
				<label for="update-interval">数据更新间隔（秒）</label><br>
				<input type="number" id="update-interval" min="1" max="60" value="3" style="width: 100%; padding: 8px; margin-top: 5px;">
			</div>
			<button type="submit" >保存设置</button>
			<div class="status" id="settings-status"></div>
		</form>
	</div>

	<div class="content" id="firmware-update">
		<h2>ESP32 固件更新</h2>
		<p>选择.bin格式的固件文件,上传并更新设备</p>

		<div class="upload-box">
			<form method="POST" action="/update" enctype="multipart/form-data">
				<input type="file" name="firmware" accept=".bin" required>
				<br>
				<button type="submit">开始更新</button>
			</form>
			<div class="progress">
				<div class="progress-bar" id="progressBar"></div>
			</div>
			<div class="status" id="updateStatus"></div>
		</div>
	</div>

</div>

<!--初始化-->
<script>
	let chartInstance;

	const updateInterval = 60;
	window.onload = function() {
		ControlInit();
		animateToPercentHum(0);

		loadDeviceInfo(); // 加载设备信息

		setInterval(fetchSensorData, 3000);
	};
	function ControlInit(){
		// updateStatus();
		// document.getElementById("Reboot").addEventListener("click", function() {
		//     sendCommand("/reboot");
		// })
			document.getElementById("btnOn").addEventListener("click", function() {
			sendCommand("/on");
		})

		document.getElementById("btnOff").addEventListener("click", function() {
			sendCommand("/off");
		});

	}


</script>
<!--指令发送-->
<script>

	function sendCommand(command) {
		fetch(command, {
			method: 'POST'
		})
			.then(response => response.text())
			// .then(data => {
			//     updateStatus(); // 恢复状态更新
			// })
			.catch(error => {
				alert("控制失败: " + error); // 恢复错误提示
			});

		fetch(command)
			.then(response => response.text()) // 接收ESP32返回的响应
			.then(data => {
				// updateStatus(); // 指令发送后，更新状态显示
			})
			.catch(error => {
				// alert("控制失败: " + error); // 出错时提示
			});
	}
</script>
<!--湿度显示-->
<script>
	function animateToPercentHum(targetPercentHum) {
		animateToPercent(targetPercentHum,'hum-progress-ring','.humidity-percent-text');
	}

	function animateToPercentCpuUsage(targetPercentCpuUsage) {
		animateToPercent(targetPercentCpuUsage,'cpu-progress-ring','.cpu-percent-text');
	}
	// 动画到目标百分比
	function animateToPercent(targetPercent,id,id_text) {
		const circle = document.getElementById(id);
		const text = document.querySelector(id_text);
		const r = 40;
		const circumference = 2 * Math.PI * r; // 周长 ≈ 251.2

		// 计算目标偏移值（周长 - 进度长度）
		const targetOffset = circumference - (circumference * targetPercent / 100);

		// 动画过程中实时更新文本（让数字也跟着动）

		text.textContent = `${Math.round(targetPercent)}%`;

		// 触发进度环动画（修改offset）
		circle.style.strokeDashoffset =   targetOffset;
	}
</script>
<!--温度显示-->
<script>

	function setTemp(targetTemp) {

		// 计算当前温度（取整）
		const current = Math.round(startTemp + (targetTemp - startTemp) * easeProgress);
		// 更新显示
		tempDisplay.textContent = `${current}°C`;
		updateColor(current);
	}

	// 根据温度更新颜色
	function updateColor(temp) {
		let color;
		if (temp < 10) {
			// 低温：蓝色系（温度越低越蓝）
			const blue = 255;
			const green = Math.round(100 + (temp + 10) * 5);
			color = `rgb(74, ${green}, ${blue})`;
		} else if (temp < 25) {
			// 中温：绿色系
			const green = 200;
			const red = Math.round(74 + (temp - 10) * 8);
			color = `rgb(${red}, ${green}, 100)`;
		} else {
			// 高温：红色系（温度越高越红）
			const red = 255;
			const green = Math.round(200 - (temp - 25) * 6);
			color = `rgb(${red}, ${green}, 74)`;
		}
		cpuTempDisplay.style.color = color;
	}

</script>
<!--菜单切换-->
<script>
	// 菜单切换功能
	document.querySelectorAll('.menu-item').forEach(item => {
		item.addEventListener('click', () => {
			// 移除所有菜单的active类
			document.querySelectorAll('.menu-item').forEach(menu => {
				menu.classList.remove('active');
			});
			// 给当前点击的菜单添加active类
			item.classList.add('active');

			// 隐藏所有内容
			document.querySelectorAll('.content').forEach(content => {
				content.classList.remove('active');
			});
			// 显示对应内容（通过data-target关联ID）
			const targetId = item.getAttribute('data-target');
			document.getElementById(targetId).classList.add('active');
		});
	});
</script>
<!--固件更新-->
<script>
	document.querySelector('form[action="/update"]').addEventListener('submit', function(e) {
		e.preventDefault();
		const formData = new FormData(this);
		const xhr = new XMLHttpRequest();

		document.querySelector('.progress').style.display = 'block';
		document.getElementById('updateStatus').className = 'status';
		document.getElementById('updateStatus').style.display = 'none';

		xhr.upload.addEventListener('progress', function(e) {
			if (e.lengthComputable) {
				const percent = (e.loaded / e.total) * 100;
				document.getElementById('progressBar').style.width = percent + '%';
			}
		});

		xhr.onload = function() {
			if (xhr.status === 200) {
				document.getElementById('updateStatus').textContent = '更新成功！设备正在重启...';
				document.getElementById('updateStatus').className = 'status success';
				document.getElementById('updateStatus').style.display = 'block';
				location.reload();
			} else {
				document.getElementById('updateStatus').textContent = '更新失败：' + xhr.responseText;
				document.getElementById('updateStatus').className = 'status error';
				document.getElementById('updateStatus').style.display = 'block';
			}
		};

		xhr.onerror = function() {
			document.getElementById('updateStatus').textContent = '网络错误，更新中断';
			document.getElementById('updateStatus').className = 'status error';
			document.getElementById('updateStatus').style.display = 'block';
		};

		xhr.open('POST', '/update', true);
		xhr.send(formData);
	});
</script>
<!--设备信息加载-->
<script>
	function loadDeviceInfo() {
		// 示例：通过AJAX请求设备信息
		const xhr = new XMLHttpRequest();
		xhr.open('GET', '/device-info', true);
		xhr.onload = function() {
			// if (xhr.status === 200) {
				const info = JSON.parse(xhr.responseText);

				document.getElementById("firmware-version").textContent=info.version;
				document.getElementById('device-ip').textContent = info.ip;
				document.getElementById('wifi-signal').textContent = info.rssi + ' dBm';
				document.getElementById('uptime').textContent = info.uptime + ' 小时';
				document.getElementById('storage').textContent = info.usedStorage + ' MB / ' + info.totalStorage + ' MB';
			// }
		};
		xhr.send();
	}

</script>
<!--传感器数据获取-->
<script>
	function fetchSensorData(){
		const xhr=new XMLHttpRequest();
		
		xhr.open('GET','/data',true);
		xhr.onload=function(){
			// if(xhr.status===200){
			const jsonData=JSON.parse(xhr.responseText);
			console.log('收到传感器数据:',jsonData);
			
			if(jsonData.type==='sensor_data'){
				document.getElementById('tempDisplay').textContent=jsonData.cpuTemp.toFixed(1);
				animateToPercentCpuUsage(jsonData.cpuUsage.toFixed(1));
			}
			// }   
		}
		xhr.send();

	}


</script>
<!--系统设置表单提交-->
<script>
	document.getElementById('settings-form').addEventListener('submit', function(e) {
		e.preventDefault();
		const settings = {
			ssid: document.getElementById('wifi-ssid').value,
			password: document.getElementById('wifi-password').value,
			interval: document.getElementById('update-interval').value
		};

		const xhr = new XMLHttpRequest();
		xhr.open('POST', '/system-settings', true);
		xhr.setRequestHeader('Content-Type', 'application/json');
		xhr.onload = function() {
			if (xhr.status === 200) {
				document.getElementById('settings-status').textContent = '设置保存成功！';
				document.getElementById('settings-status').className = 'status success';
				document.getElementById('settings-status').style.display = 'block';
			} else {
				document.getElementById('settings-status').textContent = '保存失败：' + xhr.responseText;
				document.getElementById('settings-status').className = 'status error';
				document.getElementById('settings-status').style.display = 'block';
			}
			// 3秒后隐藏状态提示
			setTimeout(() => {
				document.getElementById('settings-status').style.display = 'none';
			}, 3000);
		};
		xhr.send(JSON.stringify(settings));
	});
</script>
</body>
</html>
)";



}
