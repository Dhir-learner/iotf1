# IoT Fingerprint Door Lock System

A complete IoT solution for a fingerprint-controlled door lock using ESP32/Arduino and a Node.js server with real-time notifications.

## 🎯 Features

- **Fingerprint Authentication**: Secure access using fingerprint sensor
- **Real-time Notifications**: Email alerts for unauthorized access attempts
- **Web Dashboard**: Monitor access logs and statistics
- **Cloud Deployment**: Ready for deployment on Render
- **LED & Audio Feedback**: Visual and audio status indicators
- **Auto-locking**: Door automatically locks after 5 seconds
- **WiFi Connectivity**: Send data to cloud server

## 📋 Hardware Requirements

### ESP32/Arduino Components:
- ESP32 or Arduino board (ESP32 recommended for WiFi)
- Fingerprint sensor (AS608 or similar)
- Servo motor or electronic lock
- LEDs (Red and Green)
- Buzzer
- Resistors (220Ω for LEDs)
- Breadboard and jumper wires

### Pin Configuration:
```
Fingerprint Sensor:
- VCC → 3.3V
- GND → GND  
- RX → Pin 2
- TX → Pin 3

Door Lock (Servo/Relay):
- Signal → Pin 4
- VCC → 5V
- GND → GND

LEDs:
- Green LED → Pin 5 (with 220Ω resistor)
- Red LED → Pin 6 (with 220Ω resistor)

Buzzer:
- Positive → Pin 7
- Negative → GND
```

## 🚀 Quick Start

### 1. Hardware Setup

1. Connect all components according to the pin configuration above
2. Upload the fingerprint enrollment sketch first to register fingerprints
3. Upload the main door lock sketch

### 2. Arduino Code Setup

1. Install required libraries:
   ```
   - Adafruit Fingerprint Sensor Library
   - ArduinoJson
   - WiFi (ESP32) or WiFiEsp (Arduino)
   ```

2. Update WiFi credentials in `fingerprint_door_lock.ino`:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

3. Update server URL (after deploying to Render):
   ```cpp
   const char* serverURL = "https://your-app-name.onrender.com/api/fingerprint";
   ```

### 3. Enroll Fingerprints

1. Upload `enroll_fingerprint.ino` to your ESP32
2. Open Serial Monitor
3. Follow prompts to enroll fingerprints (ID 1-127)
4. Remember the IDs for authorized users

### 4. Server Deployment on Render

1. **Create Render Account**: Sign up at [render.com](https://render.com)

2. **Deploy the Server**:
   - Connect your GitHub repository
   - Choose "Web Service"
   - Set build command: `npm install`
   - Set start command: `npm start`
   - Choose free tier or paid plan

3. **Configure Environment Variables** in Render Dashboard:
   ```
   NODE_ENV=production
   EMAIL_USER=your-gmail@gmail.com
   EMAIL_PASS=your-app-password
   NOTIFICATION_EMAIL=recipient@gmail.com
   ADMIN_KEY=your-secret-admin-key
   ```

4. **Gmail Setup for Notifications**:
   - Enable 2-factor authentication on Gmail
   - Generate an App Password (not your regular password)
   - Use the App Password in EMAIL_PASS

### 5. Update Arduino Code with Server URL

After deployment, update the Arduino code:
```cpp
const char* serverURL = "https://your-app-name.onrender.com/api/fingerprint";
```

## 📊 API Endpoints

- `GET /` - Server status and available endpoints
- `POST /api/fingerprint` - Receive fingerprint data from ESP32
- `GET /api/logs` - Get access logs
- `GET /api/logs/recent` - Get recent unauthorized attempts
- `GET /api/stats` - Get access statistics
- `GET /health` - Health check

## 🔧 Configuration

### Email Notifications
To enable email notifications, configure these environment variables:
- `EMAIL_USER`: Your Gmail address
- `EMAIL_PASS`: Gmail App Password (not regular password)
- `NOTIFICATION_EMAIL`: Where to send notifications

### Security
- Change the `ADMIN_KEY` to a secure random string
- Use HTTPS in production (Render provides this automatically)

## 📱 Web Dashboard

Access your dashboard at: `https://your-app-name.onrender.com`

Features:
- Real-time access statistics
- Recent access logs
- Authorized vs unauthorized attempts
- Auto-refresh every 30 seconds

## 🔐 Security Features

1. **Fingerprint Authentication**: Only enrolled fingerprints can unlock
2. **Auto-lock**: Door locks automatically after 5 seconds
3. **Unauthorized Alerts**: Instant email notifications for failed attempts
4. **Access Logging**: All attempts are logged with timestamps
5. **Visual/Audio Feedback**: LEDs and buzzer indicate status

## 🛠️ Troubleshooting

### Arduino Issues:
- **Fingerprint sensor not detected**: Check wiring and power supply
- **WiFi connection failed**: Verify credentials and signal strength
- **Server communication error**: Check URL and internet connection

### Server Issues:
- **Email not sending**: Verify Gmail settings and App Password
- **Deployment failed**: Check logs in Render dashboard
- **API not responding**: Verify environment variables

### Common Fixes:
1. **Power Supply**: Ensure adequate power for all components
2. **Serial Monitor**: Use for debugging connection issues
3. **Pin Conflicts**: Verify no pin conflicts between components
4. **Library Versions**: Use latest stable versions of libraries

## 📝 Customization

### Adding More Features:
- **SMS Notifications**: Integrate Twilio API
- **Multiple Locations**: Add location tracking
- **Database**: Replace in-memory storage with MongoDB
- **Mobile App**: Create React Native companion app
- **Facial Recognition**: Add camera module

### Hardware Upgrades:
- **OLED Display**: Show status and messages
- **Keypad Backup**: Secondary unlock method
- **Motion Sensor**: Detect presence before scanning
- **Camera Module**: Capture photos during access

## 📄 License

MIT License - feel free to modify and distribute

## 🆘 Support

For issues and questions:
1. Check the troubleshooting section
2. Review Arduino Serial Monitor output
3. Check Render deployment logs
4. Verify all connections and configurations

## 🔄 Updates

Keep your system updated:
- Arduino libraries
- Node.js dependencies (`npm update`)
- Monitor Render for platform updates

---

**Happy Building! 🔨**