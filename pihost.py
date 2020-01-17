import picamera
import io
import time
import socket

camera = picamera.PiCamera(resolution="1280x720")
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
            camera.capture(stream, "jpeg")
            print("about to send...")
            connection.sendall(stream.getvalue())
            time.sleep(0.2)

