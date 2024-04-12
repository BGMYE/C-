import random
import tkinter as tk

# 全局变量
Board = None  # 用于存储棋盘的列表
mark = 0  # 计数器，用于标记不同区域

def ChessBoard(tr, tc, dr, dc, size):
    """
    递归函数，用于划分棋盘
    :param tr: 左上角的行
    :param tc: 左上角的列
    :param dr: 特殊方格的行
    :param dc: 特殊方格的列
    :param size: 矩阵大小
    """
    global Board
    global mark
    mark += 1
    count = mark
    if size == 1:
        return
    s = size // 2
    # 覆盖左上角子棋盘
    if dr < tr + s and dc < tc + s:
        ChessBoard(tr, tc, dr, dc, s)
    else:
        Board[tr + s - 1][tc + s - 1] = count
        ChessBoard(tr, tc, tr + s - 1, tc + s - 1, s)
    # 覆盖右上角子棋盘
    if dr < tr + s and dc >= tc + s:
        ChessBoard(tr, tc + s, dr, dc, s)
    else:
        Board[tr + s - 1][tc + s] = count
        ChessBoard(tr, tc + s, tr + s - 1, tc + s, s)
    # 覆盖左下角子棋盘
    if dr >= tr + s and dc < tc + s:
        ChessBoard(tr + s, tc, dr, dc, s)
    else:
        Board[tr + s][tc + s - 1] = count
        ChessBoard(tr + s, tc, tr + s, tc + s - 1, s)
    # 覆盖右下角子棋盘
    if dr >= tr + s and dc >= tc + s:
        ChessBoard(tr + s, tc + s, dr, dc, s)
    else:
        Board[tr + s][tc + s] = count
        ChessBoard(tr + s, tc + s, tr + s, tc + s, s)


def drawboard(canvas1, board, x=40, y=40, cell_width=40):
    """
    用于绘制棋盘
    :param canvas1: tkinter 的画布对象
    :param board: 棋盘列表
    :param x: 棋盘左上角的 x 坐标
    :param y: 棋盘左上角的 y 坐标
    :param cell_width: 每个单元格的宽度
    """
    color_map = {}  # 创建颜色映射字典
    width = 2 * x + len(board) * cell_width
    height = 2 * y + len(board) * cell_width
    canvas1.config(width=width, height=height)

    for i in range(len(board)):
        for j in range(len(board)):
            if board[i][j] != -1 :
                if board[i][j] not in color_map:  # 如果该值尚未在颜色映射中
                    # 生成随机颜色或者根据具体需求选取颜色
                    r = random.randint(0, 255)
                    g = random.randint(0, 255)
                    b = random.randint(0, 255)
                    color = f"#{r:02x}{g:02x}{b:02x}"
                    color_map[board[i][j]] = color  # 将该值映射到生成的颜色
                else:
                    color = color_map[board[i][j]]  # 若已经存在颜色映射，则使用之前的颜色
            else:
                color = 'white'
            cell_x = x + i * cell_width
            cell_y = y + j * cell_width
            canvas1.create_rectangle(cell_x, cell_y, cell_x + cell_width, cell_y + cell_width,
                                     fill=color, outline="black")


def Input():
    """
    从输入框获取用户输入，并生成棋盘
    """
    global Board

    n = entry_board_size.get()
    x = entry_board_x.get()
    y = entry_board_y.get()
    n = 2 ** int(n)
    Board = [[-1 for x in range(n)] for y in range(n)]
    ChessBoard(0, 0, int(x), int(y), n)

    window_chessboard = tk.Toplevel(root)
    window_chessboard.title('Chessboard')

    canvas1 = tk.Canvas(window_chessboard, bg="white")
    canvas1.pack()
    drawboard(canvas1, Board)

if __name__ == "__main__":
    mark = 0
    root = tk.Tk()

    width = root.winfo_screenwidth() // 4
    height = root.winfo_screenheight() // 4
    root.title('Chessboard')
    root.geometry(f"{width}x{height}")
    root.resizable(width=False, height=False)
    tk.Label(root, text='Chessboard Coverage', font=('Times New Roman', 25)).place(x=width // 3, y=height // 8)

    tk.Label(root, text="输入棋盘大小（2的幂指数）：", font=('Arial', 15)).place(x=width // 5, y=height // 3)
    var_board_size = tk.StringVar()
    entry_board_size = tk.Entry(root, textvariable=var_board_size, font=('Arial', 15))
    entry_board_size.place(x=width // 2, y=height // 3)

    tk.Label(root, text="输入特殊方格位置：", font=('Arial', 15)).place(x=width / 2.5, y=height // 2)

    tk.Label(root, text="横坐标x:", font=('Arial', 15)).place(x=width // 6, y=height // 1.5)
    var_board_x = tk.StringVar()
    entry_board_x = tk.Entry(root, textvariable=var_board_x, font=('Arial', 15))
    entry_board_x.place(x=width // 4, y=height // 1.5)

    tk.Label(root, text="纵坐标y:", font=('Arial', 15)).place(x=width // 2, y=height // 1.5)
    var_board_y = tk.StringVar()
    entry_board_y = tk.Entry(root, textvariable=var_board_y, font=('Arial', 15))
    entry_board_y.place(x=int(width / 1.7), y=height // 1.5)

    btn = tk.Button(root, text="Run", font=('Arial', 20), command=Input).place(x=width // 2.2, y=height // 1.3)

    root.mainloop()
