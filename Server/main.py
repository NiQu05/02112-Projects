from flask import Flask, request, jsonify
import openpyxl
import os
from datetime import datetime


app = Flask(__name__)

# Define the name of the Excel file
excel_file = "sensor_data.xlsx"

# Ensure the Excel file exists
if not os.path.exists(excel_file):
    # Create a new workbook and add a header row
    wb = openpyxl.Workbook()
    ws = wb.active
    ws.title = "Sensor Data"
    ws.append(["Timestamp", "Air Temperature", "Air Humidity", "Soil Temperature", "Soil Moisture", "Light Value"])
    wb.save(excel_file)

@app.route('/sensor-data', methods=['POST'])
def receive_sensor_data():
    try:
        data = request.json  # Expecting JSON data
        if not data or not all(key in data for key in ['airTemp', 'airHumidity', 'soilTemp', 'soilMoisture', 'light']):
            return jsonify({"error": "Invalid data, expected keys: 'airTemp', 'airHumidity', 'soilTemp', 'soilMoisture', 'light'"}), 400
        
        # Extract the sensor values
        values = [data['airTemp'], data['airHumidity'], data['soilTemp'], data['soilMoisture'], data['light']]

        # Generate the current timestamp
        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        # Open the workbook and append the data
        wb = openpyxl.load_workbook(excel_file)
        ws = wb.active
        ws.append([timestamp] + values)  # Append data with the current timestamp
        wb.save(excel_file)

        return jsonify({"message": "Sensor data received and saved"}), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5120)  # Run the server on port 5000
