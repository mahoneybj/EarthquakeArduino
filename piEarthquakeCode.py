import serial
import time
import requests

# Configuration
serial_port = '/dev/ttyACM0'  # Change to your serial port
baud_rate = 9600              # Change to your baud rate
calibration_index = 10
threshold = 0.3               # Deviation threshold to start capture period
capture_duration = 3          # Capture period in seconds
api_url = 'https://earthquake-web.azurewebsites.net/api/earthquake-alerts'
sensor_id = 1
buffer_time = 5               # Buffer time after calibration to ignore minor fluctuations

def read_serial_data(port, baud_rate, calibration_index, threshold, capture_duration, buffer_time):
    # Open the serial port
    ser = serial.Serial(port, baud_rate, timeout=1)
    print(f"Connected to {port} at {baud_rate} baud.")
    
    # Give some time for the connection to establish
    time.sleep(2)
    
    calibration_data = None
    count = 0
    p_wave = None
    s_wave = None
    
    try:
        while True:
            try:
                # Read a line from the serial port
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                
                if line:
                    count += 1
                    # Split the line into x, y, z values
                    try:
                        x, y, z = map(float, line.split(','))
                        print(f"Reading {count}: x={x}, y={y}, z={z}")
                        
                        if count == calibration_index:
                            calibration_data = (x, y, z)
                            print(f"Calibration data captured: {calibration_data}")
                            print(f"Entering buffer period for {buffer_time} seconds.")
                            time.sleep(buffer_time)
                            print("Buffer period ended. Starting to monitor for deviations.")
                        elif calibration_data:
                            # Check for deviation
                            dx = abs(x - calibration_data[0])
                            dy = abs(y - calibration_data[1])
                            dz = abs(z - calibration_data[2])
                            
                            if dx > threshold or dy > threshold or dz > threshold:
                                print("Significant deviation detected. Starting capture period.")
                                p_wave = capture_peak(ser, calibration_data, capture_duration)
                                print(f"P wave captured: {p_wave}")
                                
                                s_wave = wait_for_s_wave(ser, calibration_data, p_wave, threshold)
                                print(f"S wave captured: {s_wave}")

                                # Send data to API
                                send_to_api(p_wave, s_wave, sensor_id, api_url)
                                
                                break
                    except ValueError:
                        print(f"Invalid data format: {line}")
            except UnicodeDecodeError:
                print("Failed to decode line, skipping...")
                    
    except KeyboardInterrupt:
        print("Serial reading interrupted.")
    finally:
        # Close the serial port
        ser.close()
        print("Serial port closed.")
        
    return calibration_data, p_wave, s_wave

def capture_peak(ser, calibration_data, capture_duration):
    start_time = time.time()
    max_deviation = [0, 0, 0]
    
    while time.time() - start_time < capture_duration:
        try:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line:
                x, y, z = map(float, line.split(','))
                
                # Calculate deviations
                dx = abs(x - calibration_data[0])
                dy = abs(y - calibration_data[1])
                dz = abs(z - calibration_data[2])
                
                max_deviation[0] = max(max_deviation[0], dx)
                max_deviation[1] = max(max_deviation[1], dy)
                max_deviation[2] = max(max_deviation[2], dz)
                
        except (ValueError, UnicodeDecodeError):
            continue
    
    return max(max_deviation)

def wait_for_s_wave(ser, calibration_data, p_wave, threshold):
    while True:
        try:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line:
                x, y, z = map(float, line.split(','))
                
                # Calculate deviations
                dx = abs(x - calibration_data[0])
                dy = abs(y - calibration_data[1])
                dz = abs(z - calibration_data[2])
                
                max_deviation = max(dx, dy, dz)
                
                if max_deviation >= p_wave:
                    return max_deviation
                
        except (ValueError, UnicodeDecodeError):
            continue

def send_to_api(p_wave, s_wave, sensor_id, api_url):
    data = {
        'pwave': p_wave,
        'swave': s_wave,
        'sensorid': sensor_id
    }
    try:
        response = requests.post(api_url, json=data)
        if response.status_code == 201:
            print("Data successfully sent to API.")
        else:
            print(f"Failed to send data to API. Status code: {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"Error sending data to API: {e}")

# Call the function and capture the calibration, P wave, and S wave data
calibration_data, p_wave, s_wave = read_serial_data(serial_port, baud_rate, calibration_index, threshold, capture_duration, buffer_time)

# Save the data to a file
if calibration_data and p_wave is not None and s_wave is not None:
    with open('wave_data.txt', 'w') as file:
        file.write(f"Calibration data: {calibration_data[0]},{calibration_data[1]},{calibration_data[2]}\n")
        file.write(f"P wave: {p_wave}\n")
        file.write(f"S wave: {s_wave}\n")
    print("Wave data saved to wave_data.txt")
else:
    print("Wave data was not captured.")
