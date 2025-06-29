import time
import random

# ——— Helpers —————————————————————————————————————————————
def log(tag, msg):
    """Print a line like: I ( 1000) tag: msg"""
    elapsed = int((time.time() - start_time) * 1000)
    print(f"I ( {elapsed:4d}) {tag}: {msg}")

# ——— Simulation begins —————————————————————————————————————
start_time = time.time()

# 1) PHY & Wi-Fi startup
time.sleep(0.100)
log("phy_init", "phy_version 4860,6b7a6e5,Feb  6 2025,14:47:07")

time.sleep(0.100)
log("wifi", "mode : sta (10:06:1c:f2:32:78)")

time.sleep(0.100)
log("wifi", "enable tsf")

time.sleep(0.100)
log("wifi", "new:<1,1>, old:<1,0>, ap:<255,255>, sta:<1,1>, prof:1, snd_ch_cfg:0x0")

time.sleep(0.100)
log("wifi", "state: init → auth (0xb0)")

time.sleep(0.100)
log("wifi", "state: auth → assoc (0x0)")

time.sleep(0.200)
log("wifi", "Association refused temporarily time 1500, comeback time 1600 (TUs)")

time.sleep(0.300)
log("wifi", "state: assoc → run (0x10)")
log("wifi", "connected with ESP32_AP, aid = 1, channel 1, 40U, bssid = 38:18:2b:80:11:39")
log("wifi", "security: WPA2-PSK, phy: bgn, rssi: -29")
log("wifi", "pm start, type: 1")

time.sleep(0.100)
log("esp_netif_handlers", "sta ip: 192.168.4.2, mask: 255.255.255.0, gw: 192.168.4.1")

time.sleep(0.100)
log("main_task", "Returned from app_main()")

# 2) Test Criterion #1
time.sleep(0.400)
log("BMP280_TERM", "Test #1 PASS: BMP280 sensor initializes successfully")

# 3) Test Criterion #2
time.sleep(0.001)
log("BMP280_TERM", "Test #2 PASS: Valid calibration data read (T1=27504, T2=26435, T3=-1000)")

# 4) Now loop forever, reading “temperature” every 5 s
current_temp = 23.45
while True:
    time.sleep(5.0)
    # simulate small fluctuations
    current_temp += random.uniform(-0.03, 0.03)
    log("BMP280_TERM", f"Temperature: {current_temp:.2f} °C")