from openai import OpenAI
import datetime

client = OpenAI(api_key="sk-83be9eaa312e4447a576f4182ca9cf6a", base_url="https://api.deepseek.com")

#file_path = input("Enter file path: ")
file_path = "../CWE190/CWE-190_android_security_CVE-2017-0576_1.txt"
# 读取本地文件内容
with open(file_path, "r") as file:  # 修改为你的文件路径
    file_content = file.read()

response = client.chat.completions.create(
    model="deepseek-reasoner",
    messages=[
        {"role": "system", "content": "你是一个做安全代码分析的辅助者"},
        {"role": "user", "content": f"这是一个代码切片CWE-190_android_security_CVE-2017-0576_1，如果我想要你提取出涉及到漏洞触发的关键变量，我想要的结果只是关键变量：\n{file_content}"}  # 将文件内容嵌入消息
    ],
    temperature=0.3,
    stream=False
)

response_text = response.choices[0].message.content

# 保存文件
timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
filename = f"poem_{timestamp}.txt"

with open(filename, "w", encoding="utf-8") as file:
    file.write(f"\n\n{response_text}")

print(f"诗歌已保存至 {filename}")