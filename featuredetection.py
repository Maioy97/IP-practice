
import cv2
import argparse
from mtcnn.mtcnn  import MTCNN

def detectAndDisplay(frame):
    #frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faceDetection = MTCNN()
    faces = faceDetection.detect_faces(frame)

    for face in faces:
        bounding_box = face['box']
        keypoints = face["keypoints"]

        cv2.rectangle(frame, (bounding_box[0], bounding_box[1]),
                     (bounding_box[0]+bounding_box[2], bounding_box[1] + bounding_box[3]),
                     (0,155,255),  2)

        cv2.circle(frame,(keypoints['left_eye']), 2, (0,155,255), 2)
        cv2.circle(frame,(keypoints['right_eye']), 2, (0,155,255), 2)
            # for each box draw a rectangle in the original image 
    cv2.imshow('Capture - Face detection', frame)

parser = argparse.ArgumentParser(description='Code for Cascade Classifier tutorial.')
parser.add_argument('--image', help='Image path.', default="image-1.jpeg")
args = parser.parse_args()

image = cv2.imread(args.image)
if  image is  None:
    print('--(!)Error opening image')
    exit(0)
else:
    detectAndDisplay(image)
    cv2.waitKey(0)