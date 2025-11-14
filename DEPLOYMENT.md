# Deployment Guide for Render

## Step-by-Step Render Deployment

### 1. Prepare Your Code
- Ensure all files are in the `server` directory
- Verify `package.json` has correct dependencies
- Test locally with `npm install && npm start`

### 2. Create Render Account
- Go to [render.com](https://render.com)
- Sign up with GitHub account (recommended)

### 3. Deploy Web Service

1. **Connect Repository**:
   - Click "New +" → "Web Service"
   - Connect GitHub and select your repository
   - Choose the repository with your server code

2. **Configure Service**:
   - **Name**: `iot-door-lock-demo` (or your choice)
   - **Root Directory**: Leave empty (files are in root now)
   - **Environment**: `Node`
   - **Build Command**: `npm install`
   - **Start Command**: `npm start`
   - **Plan**: Free (or paid for better performance)

3. **Environment Variables**:
   Click "Advanced" and add these variables:
   ```
   NODE_ENV=production
   EMAIL_USER=your-email@gmail.com
   EMAIL_PASS=your-gmail-app-password
   NOTIFICATION_EMAIL=alerts@yourdomain.com
   ADMIN_KEY=your-secret-admin-key-123
   ```

### 4. Gmail Setup for Notifications

1. **Enable 2FA on Gmail**:
   - Google Account → Security → 2-Step Verification → Turn On

2. **Create App Password**:
   - Google Account → Security → 2-Step Verification
   - Scroll to "App passwords"
   - Generate password for "Mail"
   - Use this password (not your regular Gmail password) in `EMAIL_PASS`

### 5. Deploy and Test

1. **Deploy**: Click "Create Web Service"
2. **Wait**: Deployment takes 2-5 minutes
3. **Get URL**: Your app will be at `https://your-app-name.onrender.com`
4. **Test**: Visit `/health` endpoint to verify

### 6. Update Arduino Code

Replace the server URL in your Arduino code:
```cpp
const char* serverURL = "https://your-app-name.onrender.com/api/fingerprint";
```

### 7. Monitor Deployment

- **Logs**: Check deployment logs for errors
- **Events**: Monitor for crashes or issues
- **Metrics**: View performance data

## Free Tier Limitations

- **Sleep**: Free services sleep after 15 min inactivity
- **Build Minutes**: 500 minutes/month
- **Bandwidth**: 100GB/month
- **Storage**: No persistent disk storage

## Upgrading to Paid

For production use, consider paid plans:
- **Starter**: $7/month - No sleep, more resources
- **Standard**: $25/month - Better performance
- **Pro**: $85/month - High availability

## Troubleshooting

### Common Issues:

1. **Build Fails**:
   ```bash
   # Check package.json syntax
   # Verify Node.js version compatibility
   # Check for missing dependencies
   ```

2. **Environment Variables**:
   - Ensure no typos in variable names
   - Gmail App Password, not regular password
   - Restart service after changing variables

3. **Email Not Working**:
   - Verify Gmail 2FA is enabled
   - Use App Password, not regular password
   - Check spam folder for test emails

4. **Service Won't Start**:
   - Check logs for error messages
   - Verify start command: `npm start`
   - Ensure port binding: `process.env.PORT`

### Testing Commands:

```bash
# Test locally first
npm install
npm start

# Test endpoints
curl http://localhost:3000/health
curl http://localhost:3000/
```

## Custom Domain (Optional)

1. **Add Domain** in Render Dashboard
2. **Update DNS** records at your domain provider
3. **SSL Certificate** automatically provisioned

## Monitoring

- Set up health checks
- Monitor logs regularly  
- Use Render's built-in metrics
- Consider external monitoring (UptimeRobot, etc.)

## Backup Strategy

- Code: Keep in Git repository
- Data: Export logs periodically (API endpoint)
- Configuration: Document environment variables

## Security Checklist

- [x] Use environment variables for secrets
- [x] Enable HTTPS (automatic on Render)  
- [x] Strong admin key
- [x] Gmail App Password
- [x] CORS properly configured
- [x] Input validation on endpoints