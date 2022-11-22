import cv2
import matplotlib.pyplot as plt

cap = cv2.VideoCapture("../video/test1.mkv")

if (cap.isOpened()== False):  
  print("error occured") 

count = 0
while(cap.isOpened()):

    ret, frame = cap.read()
    if ret == True:
        count += 1
    else:
        break
    if 75 < count < 90:
        plt.figure(figsize=(14,10))
        plt.imshow(frame)
        plt.show()
print(count)

cap.release()