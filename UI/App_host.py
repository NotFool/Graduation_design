from flask import Flask, request, jsonify, send_from_directory
import os
from werkzeug.utils import secure_filename

app = Flask(__name__)

# 定义文件存储目录
UPLOAD_FOLDER = 'uploads'
PROCESSED_FOLDER = 'processed'
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(PROCESSED_FOLDER, exist_ok=True)

@app.route('/api/upload', methods=['POST'])
def upload_file():
    file = request.files.get('file')
    cwe = request.form.get('cwe')
    if not file or not cwe:
        return jsonify({'code': 1, 'message': '文件或CWE信息缺失'}), 400
    filename = secure_filename(file.filename)
    file_path = os.path.join(UPLOAD_FOLDER, filename)
    file.save(file_path)
    # 可在此记录上传记录到数据库或内存中
    return jsonify({'code': 0, 'message': '上传成功', 'filename': filename})

@app.route('/api/process', methods=['POST'])
def process_file():
    data = request.get_json()
    if not data:
        return jsonify({'code': 1, 'message': '缺失数据'}), 400
    filename = data.get('filename')
    cwe = data.get('cwe')
    variables = data.get('variables')  # 变量列表
    if not filename or not cwe or variables is None:
        return jsonify({'code': 1, 'message': '缺失必要信息'}), 400
    upload_path = os.path.join(UPLOAD_FOLDER, filename)
    if not os.path.exists(upload_path):
        return jsonify({'code': 1, 'message': '上传文件不存在'}), 404
    with open(upload_path, 'r', encoding='utf-8') as f:
        content = f.read()
    # 进行简单处理：追加变量信息，可根据业务逻辑改写
    processed_content = content + "\n\n--变量信息--\n" + "\n".join(variables)
    # 生成处理后文件名
    processed_filename = filename.rsplit('.', 1)[0] + '_processed.txt'
    processed_path = os.path.join(PROCESSED_FOLDER, processed_filename)
    with open(processed_path, 'w', encoding='utf-8') as f:
        f.write(processed_content)
    return jsonify({
        'code': 0,
        'message': '处理成功',
        'processed_filename': processed_filename,
        'processed_content': processed_content
    })

@app.route('/api/file-content', methods=['GET'])
def get_file_content():
    file_type = request.args.get('type')  # "uploaded" 或 "processed"
    filename = request.args.get('filename')
    if file_type not in ['uploaded', 'processed'] or not filename:
        return jsonify({'code': 1, 'message': '参数错误'}), 400
    folder = UPLOAD_FOLDER if file_type == 'uploaded' else PROCESSED_FOLDER
    file_path = os.path.join(folder, filename)
    if not os.path.exists(file_path):
        return jsonify({'code': 1, 'message': '文件不存在'}), 404
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    return jsonify({'code': 0, 'message': '成功', 'content': content})

@app.route('/api/download', methods=['GET'])
def download_file():
    filename = request.args.get('filename')
    if not filename:
        return jsonify({'code': 1, 'message': '缺少文件名参数'}), 400
    # 这里默认下载处理后的文件
    if not os.path.exists(os.path.join(PROCESSED_FOLDER, filename)):
        return jsonify({'code': 1, 'message': '文件不存在'}), 404
    return send_from_directory(PROCESSED_FOLDER, filename, as_attachment=True)

if __name__ == '__main__':
    app.run(debug=True)
