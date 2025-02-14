import dlib
from flask import Flask, render_template, Response
import cv2

app = Flask(__name__)
camera = cv2.VideoCapture(0)#def
camera2 = cv2.VideoCapture(1)#2nd cam
# gen frames 
def gen_frames(camera):
    while True:
        success,frames = camera.read()
        if not success:
            break
        else:
            ret,buffer = cv2.imencode(".jpg", frames)
            frames = buffer.tobytes()

        yield(b'--frame\r\n' b'Content-Type: image/jpeg\r\n\r\n' + 
			frames + b'\r\n')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/video')
def video():
    return Response(gen_frames(camera), mimetype = "multipart/x-mixed-replace; boundary=frame")

@app.route('/video2')
def video2():
    return Response(gen_frames(camera2), mimetype = "multipart/x-mixed-replace; boundary=frame")


if __name__ == "__main__":
    app.run(debug=True)