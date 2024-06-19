import requests
from nacl.secret import SecretBox
from nacl.utils import random

# URL của API server
url = "http://localhost:5000/api/decrypt"

# Khóa bí mật (32 bytes), giống với khóa bí mật đã được sử dụng trong API server
secret_key = b'\x86\xd9#\xdf\x81\xb2\x90\xf3\x97\xe8\xc7\x8d<\xc3\x16DW9~wj\xa9\xf5\xbc\xcfG\x1bn\x8d\x12\x9c\x9c'

# Dữ liệu cần mã hóa
plaintext = "FFF THINGSBOARD"

# Tạo nonce ngẫu nhiên (24 bytes)
nonce = random(24)

# Mã hóa dữ liệu
box = SecretBox(secret_key)
ciphertext = box.encrypt(plaintext.encode('utf-8'), nonce)

# Chuẩn bị dữ liệu để gửi
data = {
    "ciphertext": ciphertext.ciphertext.hex(),
    "nonce": nonce.hex()
}

# Gửi yêu cầu HTTP POST tới API server
try:
    response = requests.post(url, json=data)
    response_data = response.json()

    if response.status_code == 200:
        print("Decrypted data:", response_data['decrypted_data'])
    else:
        print("Error:", response_data['message'])

except requests.exceptions.RequestException as e:
    print("Error sending request:", e)
