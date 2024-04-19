import numpy as np


def MatrixChain(p, n, m, s):
    for r in range(1, n):
        for i in range(n-r):
            j = i+r
            m[i][j], s[i][j] = min((m[i][k] + m[k + 1][j] + p[i][0] * p[k][1] * p[j][1], k) for k in range(i, j))

def cout(i, j):
    if i == j:
        print('A[', i, ']', end=' ')
        return
    print('(', end=' ')
    cout(i, s[i][j])
    cout(s[i][j] + 1, j)  # 递归1到s[1][j]
    print(')', end=' ')


if __name__ == "__main__":
    N = 6
    p = [[30, 35], [35, 15], [15, 5], [5, 10], [10, 20], [20, 25]]
    m = np.zeros([N, N], dtype=int)
    s = np.zeros([N, N], dtype=int)
    MatrixChain(p, N, m, s)
    cout(0, N-1)
