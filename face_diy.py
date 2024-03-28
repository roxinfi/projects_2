import cv2

# Load the pre-trained face detection model
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Load the pre-trained gender classification model
gender_net = cv2.dnn.readNetFromCaffe('deploy_gender.prototxt', 'gender_net.caffemodel')

# Load the pre-trained age classification model
age_net = cv2.dnn.readNetFromCaffe('deploy_age.prototxt', 'age_net.caffemodel')

# Open the video capture device (camera)
cap = cv2.VideoCapture(0)

# Check if the camera opened successfully
if not cap.isOpened():
    print("Error: Unable to open camera.")
    exit()

# Loop to continuously capture frames from the camera
while True:
    # Read a frame from the camera
    ret, frame = cap.read()

    # Check if the frame was read successfully
    if not ret:
        print("Error: Unable to read frame from camera.")
        break

    # Convert the frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect faces in the frame
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    # Iterate over the detected faces
    for (x, y, w, h) in faces:
        # Extract the face region of interest
        face = frame[y:y+h, x:x+w]

        # Preprocess the face for gender classification
        blob = cv2.dnn.blobFromImage(face, 1.0, (227, 227), (78.4263377603, 87.7689143744, 114.895847746), swapRB=False)

        # Feed the face into the gender classification model
        gender_net.setInput(blob)
        gender_preds = gender_net.forward()
        gender = 'Male' if gender_preds[0][0] > gender_preds[0][1] else 'Female'

        # Preprocess the face for age classification
        blob = cv2.dnn.blobFromImage(face, 1.0, (227, 227), (78.4263377603, 87.7689143744, 114.895847746), swapRB=False)

        # Feed the face into the age classification model
        age_net.setInput(blob)
        age_preds = age_net.forward()
        age = int(age_preds[0][0] * 100)

        # Draw the gender and age labels on the frame
        label = f'{gender}, Age: {age}'
        cv2.putText(frame, label, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

    # Display the output frame
    cv2.imshow('Face Detection', frame)

    # Check for the "Esc" key press
    key = cv2.waitKey(1) & 0xFF
    if key == 27:  # Check if "Esc" key was pressed
        break

# Release the video capture device and close all windows
cap.release()
cv2.destroyAllWindows()
