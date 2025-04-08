<template>
  <div class="container">
    <h2>文件上传与处理</h2>
    <!-- 上传区域 -->
    <div class="upload-section">
      <input type="file" ref="fileInput" @change="handleFileChange" accept=".txt" />
      <select v-model="selectedCwe">
        <option disabled value="">请选择CWE序号</option>
        <option value="CWE190">CWE190</option>
        <option value="CWE123">CWE123</option>
      </select>
      <button @click="uploadFile">上传文件</button>
    </div>

    <!-- 变量输入区域（上传成功后显示） -->
    <div class="variable-section" v-if="uploadedFile">
      <h3>输入变量信息</h3>
      <div v-for="(variable, index) in variables" :key="index">
        <input type="text" v-model="variables[index]" placeholder="变量信息" />
        <button @click="removeVariable(index)">删除</button>
      </div>
      <button @click="addVariable">添加变量</button>
      <button @click="processFile">提交处理</button>
    </div>

    <!-- 展示区域 -->
    <div class="display-section">
      <div class="uploaded-files">
        <h3>上传记录</h3>
        <ul>
          <li v-for="file in uploadHistory" :key="file" @click="displayFile('uploaded', file)">
            {{ file }}
          </li>
        </ul>
      </div>
      <div class="processed-files">
        <h3>回传文件</h3>
        <ul>
          <li v-for="file in processedHistory" :key="file" @click="displayFile('processed', file)">
            {{ file }}
            <button @click.stop="downloadFile(file)">下载</button>
          </li>
        </ul>
      </div>
    </div>

    <!-- 文件内容展示 -->
    <div class="file-content">
      <h3>文件内容展示</h3>
      <pre>{{ fileContent }}</pre>
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
      fileContent: '',
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
          // 可选择在处理后直接显示处理结果
          this.fileContent = response.data.processed_content;
          alert('处理成功');
        } else {
          alert(response.data.message);
        }
      }).catch(err => {
        console.error(err);
        alert('处理失败');
      });
    },
    displayFile(type, filename) {
      axios.get('/api/file-content', {
        params: {
          type: type,
          filename: filename
        }
      }).then(response => {
        if (response.data.code === 0) {
          this.fileContent = response.data.content;
        } else {
          alert(response.data.message);
        }
      }).catch(err => {
        console.error(err);
        alert('获取文件内容失败');
      });
    },
    downloadFile(filename) {
      // 直接通过地址跳转实现文件下载
      window.location.href = `/api/download?filename=${filename}`;
    }
  }
};
</script>

<style scoped>
.container {
  margin: 20px;
}
.upload-section,
.variable-section,
.display-section,
.file-content {
  margin-bottom: 20px;
}
.display-section {
  display: flex;
  justify-content: space-between;
}
.display-section > div {
  width: 45%;
}
li {
  cursor: pointer;
  margin-bottom: 5px;
}
</style>
