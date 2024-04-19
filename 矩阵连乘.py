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


def generate_matrices_from_dims(dims):
    matrices = []
    for i in range(len(dims) - 1):
        rows, cols = dims[i], dims[i + 1]
        matrices.append(np.random.rand(rows, cols))
    return matrices





if __name__ == "__main__":
    N = 6
    # p = [[30, 35], [35, 15], [15, 5], [5, 10], [10, 20], [20, 25]]
    dims = np.random.randint(2, 50, size=N)  # 生成维度列表
    matrices = generate_matrices_from_dims(dims)  # 生成矩阵

    p = [[len(matrices[i]), len(matrices[i][1])] for i in range(len(matrices))]
    m = np.zeros([N, N], dtype=int)
    s = np.zeros([N, N], dtype=int)

    MatrixChain(p, N - 1, m, s)
    cout(0, N-1)




