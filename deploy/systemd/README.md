# Rover Gateway — systemd Service

This directory contains the systemd service configuration for automatically starting the C++ Rover Gateway when the Rock 5A boots.

Assuming the Git repository is already checked out on the SBC (Rock 5A in this case), follow these steps.

## 1. Install the service

From the repository root:

```bash
sudo cp deploy/systemd/rover-gateway.service /etc/systemd/system/
```

Make sure the `ExecStart` path in the service file points to the compiled `rover_gateway` executable.

## 2. Enable and start the service

Reload systemd to recognize the new service:

```bash
sudo systemctl daemon-reload
```

Enable automatic startup on boot and start the service immediately:

```bash
sudo systemctl enable --now rover-gateway
```

## 3. Verify

Check the service status:

```bash
systemctl status rover-gateway
```

View application logs:

```bash
journalctl -u rover-gateway -f
```

## 4. Common commands

```bash
# Restart the server
sudo systemctl restart rover-gateway

# Stop the server
sudo systemctl stop rover-gateway

# Start the server
sudo systemctl start rover-gateway

# Disable automatic startup
sudo systemctl disable --now rover-gateway
```

## 5. Updating the service

After modifying `deploy/systemd/rover-gateway.service` and pulling the latest changes on the Rock 5A, copy the updated file again:

```bash
sudo cp deploy/systemd/rover-gateway.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl restart rover-gateway
```

## Notes

- The service runs as the `ubuntu` user, which must have permission to access the STM32 serial device.
- `WorkingDirectory` points to `software/rover-gateway/` so the server can access relative paths such as `./web/index.html`.
- The C++ application must already be built on the Rock 5A.
- The service restarts automatically if the process exits with a failure.
- Network connectivity is not required to start the service, but is required to access the web UI remotely.
- Before testing automatic startup with motor power connected, ensure the STM32 initializes with motors stopped and its command timeout is enabled.