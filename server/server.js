const express = require('express');
const cors = require('cors');
require('dotenv').config();

const app = express();
const PORT = process.env.PORT || 3000;

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static('public'));

// Mock data - Fake logs from November 10, 2025
const mockLogs = [
  {
    id: 1,
    timestamp: '2025-11-10T08:15:23.000Z',
    accessType: 'authorized',
    fingerID: 1,
    confidence: 95,
    deviceID: 'ESP32_DOOR_001',
    location: 'Front Door'
  },
  {
    id: 2,
    timestamp: '2025-11-10T09:32:45.000Z',
    accessType: 'authorized',
    fingerID: 3,
    confidence: 88,
    deviceID: 'ESP32_DOOR_001',
    location: 'Front Door'
  },
  {
    id: 3,
    timestamp: '2025-11-10T11:45:12.000Z',
    accessType: 'unauthorized',
    fingerID: -1,
    confidence: 0,
    deviceID: 'ESP32_DOOR_001',
    location: 'Front Door'
  },
  {
    id: 4,
    timestamp: '2025-11-10T14:22:18.000Z',
    accessType: 'authorized',
    fingerID: 2,
    confidence: 92,
    deviceID: 'ESP32_DOOR_001',
    location: 'Front Door'
  },
  {
    id: 5,
    timestamp: '2025-11-10T16:33:07.000Z',
    accessType: 'unauthorized',
    fingerID: -1,
    confidence: 0,
    deviceID: 'ESP32_DOOR_001',
    location: 'Front Door'
  },
  {
    id: 6,
    timestamp: '2025-11-10T18:45:33.000Z',
    accessType: 'unauthorized',
    fingerID: -1,
    confidence: 0,
    deviceID: 'ESP32_DOOR_001',
    location: 'Front Door'
  },
  {
    id: 7,
    timestamp: '2025-11-10T19:12:55.000Z',
    accessType: 'error',
    fingerID: -1,
    confidence: 0,
    deviceID: 'ESP32_DOOR_001',
    location: 'Front Door'
  }
];

// System status - simulating error after Nov 10
const systemStatus = {
  isOnline: false,
  lastSeen: '2025-11-10T19:12:55.000Z',
  errorMessage: 'Device connection lost - WiFi timeout after multiple unauthorized attempts'
};

// Routes

// Health check endpoint
app.get('/health', (req, res) => {
  res.json({
    status: systemStatus.isOnline ? 'OK' : 'ERROR',
    timestamp: new Date().toISOString(),
    uptime: process.uptime(),
    version: '1.0.0',
    deviceStatus: systemStatus
  });
});

// Get server status
app.get('/', (req, res) => {
  res.json({
    message: 'IoT Fingerprint Door Lock System',
    status: systemStatus.isOnline ? 'Device Online' : 'Device Offline',
    timestamp: new Date().toISOString(),
    lastActivity: systemStatus.lastSeen,
    error: systemStatus.isOnline ? null : systemStatus.errorMessage
  });
});

// Simulate receiving data (device is offline since Nov 10)
app.post('/api/fingerprint', (req, res) => {
  res.status(503).json({
    error: 'Device offline',
    message: 'Door lock device has been offline since November 10, 2025',
    lastSeen: systemStatus.lastSeen,
    errorDetails: systemStatus.errorMessage
  });
});

// Get all access logs (using mock data)
app.get('/api/logs', (req, res) => {
  res.json({
    logs: mockLogs.slice().reverse(), // Most recent first
    total: mockLogs.length,
    deviceStatus: systemStatus
  });
});

// Get statistics (based on mock data)
app.get('/api/stats', (req, res) => {
  const authorized = mockLogs.filter(log => log.accessType === 'authorized').length;
  const unauthorized = mockLogs.filter(log => log.accessType === 'unauthorized').length;
  
  res.json({
    nov10: {
      authorized: authorized,
      unauthorized: unauthorized,
      total: mockLogs.length,
      errors: 1
    },
    deviceStatus: systemStatus,
    lastActivity: systemStatus.lastSeen
  });
});

// Handle 404
app.use('*', (req, res) => {
  res.status(404).json({
    error: 'Endpoint not found'
  });
});

// Start server
app.listen(PORT, () => {
  console.log(`🚀 IoT Door Lock Demo Server running on port ${PORT}`);
  console.log(`🔗 Dashboard: http://localhost:${PORT}`);
  console.log(`⚠️  Device Status: OFFLINE since Nov 10, 2025`);
});