import os
import json


def extract_and_save(json_file):
    folder = os.path.dirname(json_file)
    with open(json_file, 'r', encoding='utf-8') as f:
        data = json.load(f)

    for idx, item in enumerate(data):
        # 仅提取 "slices" 部分内容，多个字符串按换行拼接
        slices_text = "\n".join(item.get("slices", []))

        # 构造输出的 txt 文件名，例如：原文件名_索引.txt
        base_name = os.path.splitext(os.path.basename(json_file))[0]
        output_filename = os.path.join(folder, f"{base_name}_{idx}.txt")
        with open(output_filename, 'w', encoding='utf-8') as out:
            out.write(slices_text)
        print(f"已保存：{output_filename}")


def process_folder(folder_path):
    for file in os.listdir(folder_path):
        if file.endswith(".json"):
            json_file = os.path.join(folder_path, file)
            extract_and_save(json_file)


if __name__ == "__main__":
    folder_path = input("请输入包含 JSON 文件的文件夹路径：")
    process_folder(folder_path)
