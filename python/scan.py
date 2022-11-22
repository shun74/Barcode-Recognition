import numpy as np
import cv2
import matplotlib.pyplot as plt
from tqdm import tqdm
from utils import get_width_list, decode_line, binary_line_to_number, get_best_barcode

def scan_line(line, length):
    w_width_list, w_x_list = get_width_list(line, 255, 0)
    b_width_list, b_x_list = get_width_list(line, 0, 255)

    w_start_idx_list = []
    b_start_idx_list = []
    if len(w_width_list)<length or len(b_width_list)<(length+1):
        return (0, 0, 0)
    else:
        for i in range(len(w_width_list)-length):
             row = w_width_list[i:i+length]
             if np.std(row) < row[0] * 0.1:
                w_start_idx_list.append(i)
                if w_x_list[0] > b_x_list[0]:
                    b_start_idx_list.append(i)
                else:
                    b_start_idx_list.append(i-1)
    for w_start_idx, b_start_idx in zip(w_start_idx_list, b_start_idx_list):
        w_width_mean = np.mean(w_width_list[w_start_idx:w_start_idx+length])
        decoded_line = decode_line(b_width_list[b_start_idx:b_start_idx+length+1], w_width_mean)
        if decoded_line[0] != -1:
            number = binary_line_to_number(decoded_line)
            return (number, b_x_list[b_start_idx], b_x_list[b_start_idx+length])
    return (0, 0, 0)

def scan_barcode(img, barcode_length=9, skip=1, fx=1):
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img_gray = cv2.resize(img_gray, dsize=None, fx=fx, fy=1.0, interpolation=cv2.INTER_LINEAR)
    # ret, img_otsu = cv2.threshold(img_gray, 0, 255, cv2.THRESH_OTSU)
    # img_otsu = cv2.adaptiveThreshold(img_gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 31, 10)
    # plt.imshow(img_gray, "gray")
    # plt.show()
    number_list = []
    x_list = []
    img_binary_lines = np.zeros((img_gray.shape[0], img_gray.shape[1], 1), dtype=np.uint8)
    for i in tqdm(range(img_gray.shape[0])):
        if i%skip != 0:
            continue
        line = img_gray[i, :]
        ret, line = cv2.threshold(line, 0, 255, cv2.THRESH_OTSU)
        # line = cv2.adaptiveThreshold(line, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 51, 10)
        # img_binary_lines[i, :] = line
        num, x1, x2 = scan_line(line, length=barcode_length-1)
        x1 = x1 // fx
        x2 = x2 // fx
        number_list.append(num)
        x_list.append((x1, x2))
    # print(number_list)
    # plt.imshow(img_binary_lines, "gray")
    # plt.show()
    number, y1, y2 = get_best_barcode(number_list, min_streak_thresh=15//skip)
    return number, (x_list[y1][0], y1*skip), (x_list[y2][1], y2*skip)



if __name__ == "__main__":
    img = cv2.imread("../images/test_blur.jpg")
    number, p1, p2 = scan_barcode(img, barcode_length=9, fx=3)
    img = cv2.rectangle(img, pt1=p1, pt2=p2, color=(0, 255, 0), thickness=2)
    plt.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    plt.axis("off")
    plt.title(number)
    plt.show()
