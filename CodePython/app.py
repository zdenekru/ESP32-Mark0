from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit
import json

app = Flask(__name__)
socketio = SocketIO(app)

@app.route('/')
def index():
    return render_template('dashboard.html')

@app.route('/data', methods=['POST'])
def data():
    sensor_data = request.get_json()
    print("📡 Received:", sensor_data)

    # Map `temp_inside` to `temp` for frontend compatibility
    if 'temp_inside' in sensor_data:
        sensor_data['temp'] = sensor_data['temp_inside']

    socketio.emit('sensor_update', sensor_data)
    return {'status': 'ok'}


if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000)
