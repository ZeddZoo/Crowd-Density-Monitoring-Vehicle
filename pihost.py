import picamera
import io
import time
import socket

camera = picamera.PiCamera(resolution="1280x720", framerate=5)
stream = io.BytesIO()
time.sleep(1.5)

HOST = ""
PORT = 8000

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.bind((HOST, PORT))
    sock.listen()
    connection, address = sock.accept()
    with connection:
        while True:
            data = connection.recv(1024)
            if not data:
                break
            camera.capture(stream, "jpeg")
            print("about to send...")
            connection.sendall(b"test")
