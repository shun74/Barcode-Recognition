import cv2
import matplotlib.pyplot as plt
from scan import scan_barcode

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
        number, p1, p2 = scan_barcode(frame, barcode_length=9, skip=4, fx=1)
        frame = cv2.rectangle(frame, pt1=p1, pt2=p2, color=(0, 255, 0), thickness=2)
        plt.imshow(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
        plt.axis("off")
        plt.title(number)
        plt.show()

cap.release()