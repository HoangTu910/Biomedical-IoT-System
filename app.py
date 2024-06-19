from flask import Flask, request, jsonify
from pymongo import MongoClient
from bson import ObjectId  # Import ObjectId from bson
from nacl.secret import SecretBox
from nacl.utils import random

app = Flask(__name__)

# MongoDB connection
client = MongoClient('mongodb://mongodb:27017/')
db = client.iot_data
collection = db.sensor_data

# Secret key for decryption (assuming you have this setup)
secret_key = b'\x86\xd9#\xdf\x81\xb2\x90\xf3\x97\xe8\xc7\x8d<\xc3\x16DW9~wj\xa9\xf5\xbc\xcfG\x1bn\x8d\x12\x9c\x9c'

# Decrypt function (assuming you have this setup)
def decrypt_data(ciphertext, nonce):
    box = SecretBox(secret_key)
    return box.decrypt(ciphertext, nonce)

# Routes
@app.route('/', methods=['GET'])
def index():
    return 'Welcome to the API server!'

@app.route('/api/decrypt', methods=['POST'])
def decrypt_endpoint():
    try:
        data = request.json
        ciphertext = bytes.fromhex(data['ciphertext'])
        nonce = bytes.fromhex(data['nonce'])
        decrypted_data = decrypt_data(ciphertext, nonce)
        
        # Insert decrypted data into MongoDB
        inserted_id = collection.insert_one({'data': decrypted_data.decode('utf-8')}).inserted_id
        
        return jsonify({'status': 'success', 'decrypted_data': decrypted_data.decode('utf-8'), 'id': str(inserted_id)}), 200
    except KeyError as e:
        return jsonify({'status': 'error', 'message': f'Missing key in JSON: {str(e)}'}), 400
    except Exception as e:
        return jsonify({'status': 'error', 'message': str(e)}), 400

# Retrieve data route
@app.route('/api/retrieve', methods=['GET'])
def retrieve_data():
    try:
        # Query all documents in the sensor_data collection
        cursor = collection.find({})
        
        # Prepare a list to store results
        results = []
        for document in cursor:
            # Convert ObjectId to string
            document['_id'] = str(document['_id'])
            results.append(document)
        
        return jsonify({'status': 'success', 'data': results}), 200
    except Exception as e:
        return jsonify({'status': 'error', 'message': str(e)}), 400

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
