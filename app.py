from flask import Flask, render_template, Response
import cv2
import datetime

app = Flask(__name__)
camera = cv2.VideoCapture(0) # Camera 1
camera2 = cv2.VideoCapture(1) # Camera 2

def gen_frames(camera, label):
    while True:
        success, frame = camera.read()
        if not success:
            break
        else:
            # Get the current date and time
            tdate = datetime.datetime.now().strftime("%d-%m-%Y")
            ttime = datetime.datetime.now().strftime("%H:%M:%S")

            # Define the positions and fonts for the text overlay
            date_position = (10, 30)
            time_position = (10, 60)
            label_position = (10, 90)
            font = cv2.FONT_HERSHEY_SIMPLEX
            font_scale = 1
            font_color = (255, 255, 255) # White color
            line_type = 2

            # Draw the date, time, and label on the frame
            cv2.putText(frame, f'Date: {tdate}', date_position, font, font_scale, font_color, line_type)
            cv2.putText(frame, f'Time: {ttime}', time_position, font, font_scale, font_color, line_type)
            cv2.putText(frame, label, label_position, font, font_scale, font_color, line_type)

            # Encode the frame to JPEG
            ret, buffer = cv2.imencode(".jpg", frame)
            frame = buffer.tobytes()

        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/video')
def video():
    return Response(gen_frames(camera, 'Camera 1'), mimetype="multipart/x-mixed-replace; boundary=frame")

@app.route('/video2')
def video2():
    return Response(gen_frames(camera2, 'Camera 2'), mimetype="multipart/x-mixed-replace; boundary=frame")

if __name__ == "__main__":
    app.run(debug=True)
