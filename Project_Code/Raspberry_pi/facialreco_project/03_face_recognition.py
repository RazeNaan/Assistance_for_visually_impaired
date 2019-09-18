import cv2
import numpy as np
import os
from time import sleep
import speech_recognition as sr
from time import ctime
import time
import os
from gtts import gTTS


def speak(audioString):
    print(audioString)
    tts = gTTS(text=audioString, lang = 'en')
    tts.save("audio.mp3")
    os.system("mpg321 audio.mp3")

def recordAudio():
    r = sr.Recognizer()
    mic = sr.Microphone(2)
    mic = sr.Microphone(2)
    with sr.Microphone(device_index=2) as source:
        print("Say something!")
        audio = r.listen(source, timeout=5.0)

        data = ""
        try:
            print("hello")
            data = r.recognize_google(audio)
            print ("You said: " + data);
        except sr.UnknownValueError:
            print("Google Speech Recognition could not understand audio")
        except sr.RequestError as e:
            print("Could not request results from google speech recognition service")

        return data
    
def Jarvis(data):
    if "who is this" in data:
        facial_reco()

def facial_reco():
    recognizer = cv2.face.LBPHFaceRecognizer_create()
    recognizer.read('trainer/trainer.yml')
    cascadePath = "/home/pi/opencv-3.3.0/data/haarcascades/haarcascade_frontalface_default.xml"
    faceCascade = cv2.CascadeClassifier(cascadePath);
    font = cv2.FONT_HERSHEY_SIMPLEX
    name = ""

    def print_me(x_name, p_name):
        if x_name != p_name:
            return 1
        
#iniciate id counter
    id = 0
    # names related to ids: example ==> Marcelo: id=1,  etc
    names = ["None", "Pratyush", "Anubhav", "Vaishnavi","Mili"]# Initialize and start realtime video capture
    cam = cv2.VideoCapture(0)
    cam.set(3, 640) # set video widht
    cam.set(4, 480) # set video height
    # Define min window size to be recognized as a face
    minW = 0.1*cam.get(3)
    minH = 0.1*cam.get(4)
    while True:
        ret, img =cam.read()
        img = cv2.flip(img, 1) # Flip vertically
        gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    
        faces = faceCascade.detectMultiScale( 
            gray,
            scaleFactor = 1.2,
            minNeighbors = 5,
            minSize = (int(minW), int(minH)),
           )
        for(x,y,w,h) in faces:
            cv2.rectangle(img, (x,y), (x+w,y+h), (0,255,0), 2)
            id, confidence = recognizer.predict(gray[y:y+h,x:x+w])
            # Check if confidence is less them 100 ==> "0" is perfect match 
            if (confidence < 100):
                confidence1 = round(100-confidence)
                confidence = "  {0}%".format(round(100 - confidence))
                if(confidence1 > 45):
                    id = names[id]
                    f = open("1.txt", 'w')
                    f.write('1')
                    ahem = print_me(id, name) 
                    if(ahem == 1):
                        name = id
                        speak("this is " + id)
                else:
                    id = "unknown"
                    f = open("1.txt", 'w')
                    f.write('0')
                    ahem = print_me(id, name) 
                    if(ahem == 1):
                        name = id
                        speak("this is " + id)

            
            else:
                id = "unknown"
                confidence = "  {0}%".format(round(100 - confidence))
                f = open("1.txt", 'w')
                f.write('0')
                ahem = print_me(id, name) 
                if(ahem == 1):
                    name = id
                    speak("this is " + id)
        
            cv2.putText(img, str(id), (x+5,y-5), font, 1, (255,255,255), 2)
            cv2.putText(img, str(confidence), (x+5,y+h-5), font, 1, (255,255,0), 1)
    
        cv2.imshow('camera',img)
        k = cv2.waitKey(10) & 0xff # Press 'ESC' for exiting video
        if k == 27:
            break
    # Do a bit of cleanup
    print("\n [INFO] Exiting Program and cleanup stuff")
    os.system('lxterminal -e python call.py')
    cam.release()
    cv2.destroyAllWindows()

speak("Hello, What can I do for you?")
while(1):
   data = recordAudio()
   Jarvis(data)
