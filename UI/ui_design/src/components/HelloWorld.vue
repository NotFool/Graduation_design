<template>
  <div class="container py-4">
    <h2 class="mb-4 text-center">文件上传与处理</h2>

    <!-- 上传区域 -->
    <div class="card mb-4">
      <div class="card-header">文件上传</div>
      <div class="card-body">
        <div class="mb-3">
          <label for="fileInput" class="form-label">选择 TXT 文件</label>
          <input type="file" id="fileInput" class="form-control" @change="handleFileChange" accept=".txt" />
        </div>
        <div class="mb-3">
          <label for="cweSelect" class="form-label">选择 CWE 序号</label>
          <select v-model="selectedCwe" id="cweSelect" class="form-select">
            <option disabled value="">请选择CWE序号</option>
            <option value="CWE190">CWE190</option>
            <option value="CWE123">CWE123</option>
          </select>
        </div>
        <button class="btn btn-primary" @click="uploadFile">上传文件</button>
      </div>
    </div>

    <!-- 变量输入区域 -->
    <div class="card mb-4" v-if="uploadedFile">
      <div class="card-header">输入变量信息</div>
      <div class="card-body">
        <div v-for="(variable, index) in variables" :key="index" class="input-group mb-2">
          <input type="text" class="form-control" v-model="variables[index]" placeholder="变量信息" />
          <button class="btn btn-danger" @click="removeVariable(index)">删除</button>
        </div>
        <div class="mb-3">
          <button class="btn btn-secondary" @click="addVariable">添加变量</button>
        </div>
        <button class="btn btn-success" @click="processFile">提交处理</button>
      </div>
    </div>

    <!-- 清理区域 -->
    <div class="mb-4 text-center">
      <button class="btn btn-warning" @click="clearAll">清理所有记录和文件</button>
    </div>

    <!-- 展示区域：上传记录与回传文件列表 -->
    <div class="row">
      <!-- 上传记录 -->
      <div class="col-md-6 mb-4">
        <div class="card h-100">
          <div class="card-header">上传记录（点击可对比展示）</div>
          <ul class="list-group list-group-flush">
            <li v-for="file in uploadHistory" :key="file" class="list-group-item">
              <span @click="displayComparison(file)" class="file-link">{{ file }}</span>
            </li>
          </ul>
        </div>
      </div>
      <!-- 回传文件 -->
      <div class="col-md-6 mb-4">
        <div class="card h-100">
          <div class="card-header">回传文件</div>
          <ul class="list-group list-group-flush">
            <li v-for="file in processedHistory" :key="file" class="list-group-item d-flex justify-content-between align-items-center">
              <span class="file-link">{{ file }}</span>
              <button class="btn btn-outline-primary btn-sm" @click.stop="downloadFile(file)">下载</button>
            </li>
          </ul>
        </div>
      </div>
    </div>

    <!-- 对比展示区域 -->
    <div class="row">
      <!-- 处理前文件内容 -->
      <div class="col-md-6">
        <div class="card">
          <div class="card-header">处理前文件内容</div>
          <div class="card-body">
            <pre class="file-content">{{ originalContent }}</pre>
          </div>
        </div>
      </div>
      <!-- 处理后文件内容 -->
      <div class="col-md-6">
        <div class="card">
          <div class="card-header">处理后文件内容</div>
          <div class="card-body">
            <pre class="file-content">{{ processedContent }}</pre>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
export default {
  data() {
    return {
      selectedFile: null,
      selectedCwe: '',
      uploadedFile: null,
      variables: [],
      uploadHistory: [],
      processedHistory: [],
      // 新增两个字段用于对比展示
      originalContent: '',
      processedContent: ''
    };
  },
  methods: {
    handleFileChange(e) {
      this.selectedFile = e.target.files[0];
    },
    uploadFile() {
      if (!this.selectedFile || !this.selectedCwe) {
        alert('请选择文件和CWE序号');
        return;
      }
      let formData = new FormData();
      formData.append('file', this.selectedFile);
      formData.append('cwe', this.selectedCwe);
      axios.post('/api/upload', formData)
        .then(response => {
          if (response.data.code === 0) {
            this.uploadedFile = response.data.filename;
            this.uploadHistory.push(response.data.filename);
            alert('上传成功，请输入变量信息');
          } else {
            alert(response.data.message);
          }
        })
        .catch(err => {
          console.error(err);
          alert('上传失败');
        });
    },
    addVariable() {
      this.variables.push('');
    },
    removeVariable(index) {
      this.variables.splice(index, 1);
    },
    processFile() {
      if (!this.uploadedFile) {
        alert('请先上传文件');
        return;
      }
      axios.post('/api/process', {
        filename: this.uploadedFile,
        cwe: this.selectedCwe,
        variables: this.variables
      }).then(response => {
        if (response.data.code === 0) {
          this.processedHistory.push(response.data.processed_filename);
          // 处理成功后同时刷新对比展示区域
          this.displayComparison(this.uploadedFile);
          alert('处理成功');
        } else {
          alert(response.data.message);
        }
      }).catch(err => {
        console.error(err);
        alert('处理失败');
      });
    },
    // 新增方法：同时获取上传和处理后的文件内容
    displayComparison(uploadedFilename) {
      // 获取上传前的内容
      axios.get('/api/file-content', {
        params: {
          type: 'uploaded',
          filename: uploadedFilename
        }
      }).then(response => {
        if (response.data.code === 0) {
          this.originalContent = response.data.content;
        } else {
          this.originalContent = '获取原始内容失败';
        }
      }).catch(err => {
        console.error(err);
        this.originalContent = '获取原始内容失败';
      });

      // 计算处理后文件名（需与后端生成规则一致）
      const processedFilename = uploadedFilename.replace(/\.(txt)$/i, '') + '_processed.txt';
      axios.get('/api/file-content', {
        params: {
          type: 'processed',
          filename: processedFilename
        }
      }).then(response => {
        if (response.data.code === 0) {
          this.processedContent = response.data.content;
        } else {
          this.processedContent = '暂无处理结果';
        }
      }).catch(err => {
        console.error(err);
        this.processedContent = '暂无处理结果';
      });
    },
    downloadFile(filename) {
      window.location.href = `/api/download?filename=${filename}`;
    },
    clearAll() {
      if (!confirm('确定要清理所有记录和文件吗？此操作不可恢复。')) {
        return;
      }
      axios.post('/api/clear')
        .then(response => {
          if (response.data.code === 0) {
            this.uploadHistory = [];
            this.processedHistory = [];
            this.uploadedFile = null;
            this.variables = [];
            this.originalContent = '';
            this.processedContent = '';
            alert('清理成功');
          } else {
            alert(response.data.message);
          }
        })
        .catch(err => {
          console.error(err);
          alert('清理失败');
        });
    }
  }
};
</script>

<style scoped>
.file-link {
  cursor: pointer;
  color: #0d6efd;
}
.file-link:hover {
  text-decoration: underline;
}
.file-content {
  background-color: #f8f9fa;
  padding: 15px;
  border-radius: 5px;
  max-height: 400px;
  overflow-y: auto;
}
</style>
