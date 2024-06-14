from flask import Flask, request, jsonify

app = Flask(__name__)

# Variabel global untuk menyimpan data terakhir
last_data = {}

@app.route('/api/data', methods=['POST'])
def receive_data():
    global last_data
    data = request.json
    if not data:
        return jsonify({'error': 'No data received'}), 400

    temp = data.get('temp')
    humidity = data.get('humidity')

    if temp is None or humidity is None:
        return jsonify({'error': 'Invalid data'}), 400

    last_data = data  # Simpan data terakhir yang diterima
    print(f"Received temperature: {temp} and humidity: {humidity}")

    return jsonify({'message': 'Data received successfully'}), 200

@app.route('/api/last_data', methods=['GET'])
def get_last_data():
    if not last_data:
        return jsonify({'error': 'No data available'}), 404

    return jsonify(last_data), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
    