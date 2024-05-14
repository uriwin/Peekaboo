import cv2
import mediapipe as mp
import numpy as np
import time

mp_face_mesh = mp.solutions.face_mesh
face_mesh = mp_face_mesh.FaceMesh(min_detection_confidence=0.5, min_tracking_confidence=0.5)

mp_drawing = mp.solutions.drawing_utils
drawing_spec = mp_drawing.DrawingSpec(thickness=1, circle_radius=1)

cap = cv2.VideoCapture(1)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)

looking_mirror_start = None  # Timer start for looking at the mirror
detecting_displayed = False  # To manage the "Detecting" text display

while cap.isOpened():
    success, image = cap.read()

    if not success:
        continue

    image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
    image.flags.writeable = False
    results = face_mesh.process(image)
    image.flags.writeable = True
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    img_h, img_w, img_c = image.shape
    face_3d = []
    face_2d = []

    current_text = "Looking The mirror"  # Default assumption

    if results.multi_face_landmarks:
        for face_landmarks in results.multi_face_landmarks:
            for idx, lm in enumerate(face_landmarks.landmark):
                if idx in [33, 263, 1, 61, 291, 199]:
                    x, y = int(lm.x * img_w), int(lm.y * img_h)
                    face_2d.append([x, y])
                    face_3d.append([x, y, lm.z])

            face_2d = np.array(face_2d, dtype=np.float64)
            face_3d = np.array(face_3d, dtype=np.float64)
            cam_matrix = np.array([[img_w, 0, img_h / 2],
                                   [0, img_w, img_w / 2],
                                   [0, 0, 1]])
            dist_matrix = np.zeros((4, 1), dtype=np.float64)

            success, rot_vec, trans_vec = cv2.solvePnP(face_3d, face_2d, cam_matrix, dist_matrix)
            rmat, jac = cv2.Rodrigues(rot_vec)
            angles, mtxR, mtxQ, Qx, Qy, Qz = cv2.RQDecomp3x3(rmat)

            x, y, z = [angle * 360 for angle in angles]

            if y < -10:
                current_text = "Looking Left"
            elif y > 10:
                current_text = "Looking Right"
            elif x < -10:
                current_text = "Looking Down"
            elif x > 10:
                current_text = "Looking Up"

            if current_text == "Looking The mirror":
                if looking_mirror_start is None:
                    looking_mirror_start = time.time()
                elif time.time() - looking_mirror_start > 3:
                    detecting_displayed = True
            else:
                looking_mirror_start = None  # Reset the timer if the head pose changes
                detecting_displayed = False  # Hide "Detecting" when the pose changes

            if detecting_displayed:
                cv2.putText(image, "Detecting", (450, 100), cv2.FONT_HERSHEY_DUPLEX, 2, (0, 0, 255), 2)

            cv2.putText(image, current_text, (450, 50), cv2.FONT_HERSHEY_DUPLEX, 2, (0, 255, 0), 2)

        mp_drawing.draw_landmarks(
            image=image,
            landmark_list=face_landmarks,
            landmark_drawing_spec=drawing_spec,
            connection_drawing_spec=drawing_spec)

    cv2.imshow('Head Pose Estimation', image)
    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
