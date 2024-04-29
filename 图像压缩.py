import numpy as np
import random


def Compress(n, p, s, l, b):
    for i in range(1, n + 1):
        b[i] = int(np.log2(p[i - 1])) + 1
        bmax = b[i]
        s[i] = s[i - 1] + bmax
        l[i] = 1
        for j in range(2, min(i + 1, 256)):
            if bmax < b[i - j + 1]:
                bmax = b[i - j + 1]
            if s[i] > s[i - j] + j * bmax:
                s[i] = s[i - j] + j * bmax
                l[i] = j
        s[i] += 11


def traceback(b, l):
    back = []
    t = len(b) - 1
    while t != 0:
        back.append((l[t], b[t]))
        t -= l[t]
    k = 1
    for i in range(len(back) - 1, -1, -1):
        print('第', k, '段的元素个数为:', back[i][0], ',存储单元的位数为:', back[i][1])
        k += 1


if __name__ == "__main__":
    N = 100
    s = np.zeros(N + 1, dtype=int)
    p = np.random.randint(1, 256, size=100)
    # p = np.array([10, 12, 15, 255, 1, 2])
    l = np.zeros(N + 1, dtype=int)
    b = np.zeros(N + 1, dtype=int)
    Compress(N, p, s, l, b)
    traceback(b, l)
