#pragma once
#include <string>

const std::string uploadHTML=R"(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>HTML文件上传</title>
    <style>
        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }
        
        body {
            font-family: 'Arial', sans-serif;
            background-color: #f4f7f6;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            color: #333;
        }
        
        .container {
            background-color: white;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 4px 20px rgba(0, 0, 0, 0.1);
            width: 100%;
            max-width: 500px;
        }
        
        h1 {
            text-align: center;
            margin-bottom: 20px;
            color: #2c3e50;
        }
        
        .upload-area {
            border: 2px dashed #3498db;
            border-radius: 8px;
            padding: 40px 20px;
            text-align: center;
            margin-bottom: 20px;
            transition: all 0.3s ease;
            background-color: #f8f9fa;
        }
        
        .upload-area:hover {
            border-color: #2980b9;
            background-color: #e9ecef;
        }
        
        .upload-area.dragover {
            border-color: #27ae60;
            background-color: #e8f5e9;
        }
        
        #fileInput {
            display: none;
        }
        
        .upload-btn {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 12px 24px;
            border-radius: 5px;
            cursor: pointer;
            font-size: 16px;
            transition: background-color 0.3s ease;
            margin: 10px;
        }
        
        .upload-btn:hover {
            background-color: #2980b9;
        }
        
        .upload-btn:active {
            transform: translateY(1px);
        }
        
        #fileInfo {
            margin-top: 20px;
            padding: 15px;
            border-radius: 5px;
            background-color: #f8f9fa;
            border: 1px solid #dee2e6;
            display: none;
        }
        
        #status {
            margin-top: 20px;
            padding: 15px;
            border-radius: 5px;
            font-weight: bold;
            display: none;
        }
        
        .success {
            background-color: #d4edda;
            color: #155724;
            border: 1px solid #c3e6cb;
        }
        
        .error {
            background-color: #f8d7da;
            color: #721c24;
            border: 1px solid #f5c6cb;
        }
        
        .info {
            background-color: #d1ecf1;
            color: #0c5460;
            border: 1px solid #bee5eb;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>HTML文件上传</h1>
        
        <div class="upload-area" id="uploadArea">
            <p>点击按钮选择HTML文件，或直接拖放文件到此处</p>
            <input type="file" id="fileInput" accept=".html,.htm">
            <button class="upload-btn" id="browseBtn">浏览文件</button>
            <button class="upload-btn" id="uploadBtn" disabled>上传文件</button>
        </div>
        
        <div id="fileInfo">
            <p>已选择文件：<span id="fileName"></span></p>
            <p>文件大小：<span id="fileSize"></span></p>
        </div>
        
        <div id="status"></div>
    </div>

    <script>
        // 获取DOM元素
        const uploadArea = document.getElementById('uploadArea');
        const fileInput = document.getElementById('fileInput');
        const browseBtn = document.getElementById('browseBtn');
        const uploadBtn = document.getElementById('uploadBtn');
        const fileInfo = document.getElementById('fileInfo');
        const fileName = document.getElementById('fileName');
        const fileSize = document.getElementById('fileSize');
        const status = document.getElementById('status');

        // 选择文件
        browseBtn.addEventListener('click', () => {
            fileInput.click();
        });

        // 文件选择变化处理
        fileInput.addEventListener('change', (e) => {
            const file = e.target.files[0];
            if (file) {
                // 检查文件类型
                if (file.type === 'text/html' || file.name.endsWith('.html') || file.name.endsWith('.htm')) {
                    showFileInfo(file);
                    uploadBtn.disabled = false;
                    showStatus('文件已准备好上传', 'info');
                } else {
                    showStatus('请选择HTML文件(.html或.htm)', 'error');
                    resetForm();
                }
            }
        });

        // 拖放功能
        uploadArea.addEventListener('dragover', (e) => {
            e.preventDefault();
            uploadArea.classList.add('dragover');
        });

        uploadArea.addEventListener('dragleave', () => {
            uploadArea.classList.remove('dragover');
        });

        uploadArea.addEventListener('drop', (e) => {
            e.preventDefault();
            uploadArea.classList.remove('dragover');
            
            const file = e.dataTransfer.files[0];
            if (file) {
                // 检查文件类型
                if (file.type === 'text/html' || file.name.endsWith('.html') || file.name.endsWith('.htm')) {
                    fileInput.files = e.dataTransfer.files;
                    showFileInfo(file);
                    uploadBtn.disabled = false;
                    showStatus('文件已准备好上传', 'info');
                } else {
                    showStatus('请选择HTML文件(.html或.htm)', 'error');
                    resetForm();
                }
            }
        });

        // 上传文件
        uploadBtn.addEventListener('click', () => {
            const file = fileInput.files[0];
            if (file) {
                showStatus('正在上传...', 'info');
                
                // 创建FormData对象
                const formData = new FormData();
                formData.append('htmlFile', file);
                
                // 模拟上传过程（实际项目中应替换为真实的API请求）
                setTimeout(() => {
                    // 这里可以替换为真实的fetch或XMLHttpRequest请求
                    // 示例：
                    // fetch('/upload', {
                    //     method: 'POST',
                    //     body: formData
                    // })
                    // .then(response => response.json())
                    // .then(data => {
                    //     showStatus('文件上传成功！', 'success');
                    // })
                    // .catch(error => {
                    //     showStatus('上传失败：' + error.message, 'error');
                    // });
                    
                    showStatus('文件上传成功！', 'success');
                    uploadBtn.disabled = true;
                }, 1500);
            }
        });

        // 显示文件信息
        function showFileInfo(file) {
            fileName.textContent = file.name;
            fileSize.textContent = formatFileSize(file.size);
            fileInfo.style.display = 'block';
        }

        // 格式化文件大小
        function formatFileSize(bytes) {
            if (bytes < 1024) return bytes + ' B';
            else if (bytes < 1048576) return (bytes / 1024).toFixed(2) + ' KB';
            else return (bytes / 1048576).toFixed(2) + ' MB';
        }

        // 显示状态信息
        function showStatus(message, type) {
            status.textContent = message;
            status.className = type;
            status.style.display = 'block';
        }

        // 重置表单
        function resetForm() {
            fileInput.value = '';
            fileInfo.style.display = 'none';
            uploadBtn.disabled = true;
        }
    </script>
</body>
</html>)"; 
