import numpy as np
#import numpy.typing as npt
#import typing as tp


class Solver:
    def __init__(self, n, arr_a, arr_c, arr_c_0) -> None:
        self.n = n
        self.arr_a = np.array(arr_a)
        self.arr_c = np.array(arr_c)
        self.arr_c_0 = np.array(arr_c_0)
        self.is_solved = False
        self.price_matrix = None
        self.solution = None

    def _price_to_price(self, idx_1, idx_2) -> int:
        """
        :param idx_1: left bound of segment in 'Nearest neighbor search' task
        :param idx_2: right bound of segment in 'Nearest neighbor search' task

        :return:
                price of service [idx_1, idx_2]
        """
        assert idx_1 < idx_2

        neccessary = np.sum(self.arr_a[idx_1: idx_2])

        if neccessary:
            return self.arr_c[idx_1] * neccessary + self.arr_c_0[idx_1]
        
        return 0


    def fit_transform(self) -> 'Solver':
        """
        transform data of base task to
        price_matrix: input of 'Nearest neighbor search' task

        :return: 
                self
                    transformed data
        """
        self.price_matrix = np.zeros(shape=(self.n + 1, self.n + 1))
        for i in range(0, self.n + 1):
            for j in range(i + 1, self.n + 1):
                self.price_matrix[i][j] = self._price_to_price(i, j)
        return self


    def solve(self) -> int:
        """
        implement O(n^2) solution of 'Nearest neighbor search' task by dp.
        dp[i] - minimal price of service [0, i]
        base:
        dp[0] = 0
        dp[1] = price_matrix[0, 1], price_matrix - input data in 'Nearest neighbor search' task
        dp[i] = min(dp(j) + matrix[j, i], 0 <= x <= y - 1

        :return:
                dp[-1] - least element in dp
        """
        if self.is_solved:
            return self.solution

        dp = np.full(fill_value=np.inf, shape=(self.n + 1,))
        dp[0] = 0
        dp[1] = self.price_matrix[0, 1]

        for i in range(0, self.n + 1):
            for j in range(0, i):
                dp[i] = min(dp[i], dp[j] + self.price_matrix[j, i])

        return int(dp[-1])
    

n, arr_a, arr_c, arr_c_0 = list(map(lambda s: list(map(int, s.split(' '))), input().split('; ')))
n = n[0]

solver = Solver(n, arr_a, arr_c, arr_c_0)
solver = solver.fit_transform()
solution = solver.solve()

print(solution)