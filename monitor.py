import time
from datetime import datetime
import psutil

print("=" * 40)
print(" LIVE SYSTEM TELEMETRY MONITOR ")
print("=" * 40)
print("Press Ctrl + C to stop.\n")

try:
    while True:
        cpu = psutil.cpu_percent(interval=1)
        ram = psutil.virtual_memory().percent
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        print(f"[{timestamp}] CPU Usage: {cpu:5.1f}% | RAM Usage: {ram:5.1f}%")
        time.sleep(1)
except KeyboardInterrupt:
    print("\n[!] Monitor stopped by user.")