import numpy as np

def get_width_list(line, val1, val2):
    width_list = []
    x_list = []
    width = 0
    for x, val in enumerate(line):
        if val == val1:
            if width == 0:
                x_list.append(x)
            width += 1
        if val == val2:
            if width > 0:
                width_list.append(width)
                width = 0
    if width > 0:
        width_list.append(width)
    return np.asarray(width_list), np.asarray(x_list)

def decode_line(line, mean):
    decoded_line = []
    for val in line:
        if mean*0.15 < val < mean:
            decoded_line.append(0)
        elif mean <= val:
            decoded_line.append(1)
        else:
            return [-1]
    return np.asarray(decoded_line)[::-1]

def binary_line_to_number(line):
    sum = 0
    for i, b in enumerate(line):
        sum += 2**(i+b)
    return sum

def get_best_barcode(line, min_streak_thresh=10):
    max_streak = 0
    max_streak_num = 0
    num = 0
    streak = 0
    start_y= 0
    end_y= 0
    for y, val in enumerate(line):
        if val != num:
            if streak > max_streak and num!=0:
                max_streak = streak
                max_streak_num = num
                start_y = y - streak
                end_y = y - 1
            if val == 0:
                num = 0
                streak = 0
            else:
                num = val
                streak = 1
        else:
            streak += 1
    if max_streak > min_streak_thresh:
        return (max_streak_num, start_y, end_y)
    else:
        return (0, 0 ,0)