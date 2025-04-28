import tkinter as tk
from tkinter import ttk
import requests

ESP32_IP = 'http://192.168.1.11'
API_TOKEN = '123456'

def send_led_command(led_index, action):
    try:
        response = requests.get(
            f'{ESP32_IP}/{action}',
            params={'token': API_TOKEN, 'led': led_index},
            timeout=2
        )
        print(response.text)
        update_status()
    except:
        status_var.set("Không kết nối được ESP32")

def led_on(led_index):
    send_led_command(led_index, 'led_on')

def led_off(led_index):
    send_led_command(led_index, 'led_off')

def update_status():
    try:
        response = requests.get(
            f'{ESP32_IP}/status',
            params={'token': API_TOKEN},
            timeout=2
        )
        status_var.set(response.text)
    except:
        status_var.set("Không kết nối được ESP32")

root = tk.Tk()
root.title("Điều khiển LED ESP32 (Bảo mật)")

mainframe = ttk.Frame(root, padding="10")
mainframe.grid(row=0, column=0, sticky="nsew")

# Nút cho LED0, LED1, LED2
for i in range(3):
    led_frame = ttk.LabelFrame(mainframe, text=f"LED {i}")
    led_frame.grid(row=i, column=0, padx=10, pady=5, sticky="ew")
    on_button = ttk.Button(led_frame, text="BẬT", command=lambda i=i: led_on(i))
    on_button.grid(row=0, column=0, padx=5, pady=5)
    off_button = ttk.Button(led_frame, text="TẮT", command=lambda i=i: led_off(i))
    off_button.grid(row=0, column=1, padx=5, pady=5)

status_var = tk.StringVar()
status_label = ttk.Label(mainframe, textvariable=status_var)
status_label.grid(row=4, column=0, pady=10)

refresh_button = ttk.Button(mainframe, text="Cập nhật trạng thái", command=update_status)
refresh_button.grid(row=5, column=0, pady=5)

# Cấu hình layout
root.columnconfigure(0, weight=1)
root.rowconfigure(0, weight=1)

update_status()
root.mainloop()
