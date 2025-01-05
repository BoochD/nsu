import numpy as np

class Solver:
    def __init__(self, T: int, N: int, D_t: list, B_t: list, d_0: int, b_0: int, d: int, b: int, t_j: list) -> None:
        self.T: int = T
        self.N: int = N
        self.D_t = np.array(D_t)
        self.B_t = np.array(B_t)
        self.d_0: int = d_0
        self.b_0: int = b_0
        self.d: int = d
        self.b: int = b
        self.t_j = np.array(t_j)

        self.is_solved: bool = False
        self.solution: int = None

    def _guys_award(self, start_hour: int, end_hour: int, name: str) -> int:
        """
        implement calc: (d_0 + d * n or b_0 + b* n)  + call price

        :param start_hour: person starts work at this hour
        :param end_hour: person finishes work at this hour
        :param name: 'Bob' or 'John'
        :return:
                payment for working hours from 'start_hour' to 'end_hour' for 'name' 
        """
        assert name in ['John', 'Bob']

        tasks_curr_hour: int = self.t_j[(self.t_j >= start_hour * 60)  & (self.t_j < end_hour * 60)].size

        if name == 'John':
            curr_call_price: int = self.D_t[start_hour]
            curr_base_price: int = self.d_0
            curr_task_price: int = self.d
        elif name == 'Bob':
            curr_call_price: int = self.B_t[start_hour]
            curr_base_price: int = self.b_0
            curr_task_price: int = self.b

        award: int = np.sum([curr_task_price * i + curr_base_price for i in range(1, tasks_curr_hour + 1)])

        return award + curr_call_price
    

    def solve(self) -> int:
        """
        implement O(n^2) solution
        dp_name[i] - minimum payment in the case when the person 'name' finishes work at hour i

        dp_name[0] = 0
        dp_name[1] = payment for 'name' for the first hour

        dp_name[i] = min(dp_name[i], name_award[0, i], dp_other_name[j] + name_awardp[j, i]), 0 <= j < i
        (dp_other_name[j] + name_awardp[j, i] implements the case when the best solution has already 
        been determined before moment j and 'other_name' has finished working at moment j. And from moment j to moment i 'name' works)
        :return:
                minimal price
        """ 
        if self.is_solved:
            return self.solution
        
        dp_bob = np.zeros(shape=(self.T + 1))
        dp_john = np.zeros(shape=(self.T + 1))

        dp_bob[1] = self._guys_award(start_hour=0, end_hour=1, name='Bob')
        dp_john[1] = self._guys_award(start_hour=0, end_hour=1, name='John')

        for i in range(2, self.T + 1):
            dp_bob[i] = self._guys_award(start_hour=0, end_hour=i, name='Bob')
            dp_john[i] = self._guys_award(start_hour=0, end_hour=i, name='John')
            for j in range(0, i):
                dp_bob[i] = min(dp_john[j] + self._guys_award(start_hour=j, end_hour=i, name='Bob'), 
                                                dp_bob[i])
                dp_john[i] = min(dp_bob[j] + self._guys_award(start_hour=j, end_hour=i, name='John'),
                                                dp_john[i])

        self.solution = int(min(dp_bob[-1], dp_john[-1]))

        return self.solution


splt_1 = input().split('; ')
T: int = int(splt_1[0])
N: int = int(splt_1[1])
D_t: list = list(map(int, splt_1[2].split(' ')))
B_t: list = list(map(int, splt_1[3].split(' ')))
d_0: int = int(splt_1[4])
b_0: int = int(splt_1[5])
d: int =  int(splt_1[6])
b: int = int(splt_1[7])
t_j: list = list(map(int, splt_1[8].split(' ')))


# T, N, D_t, B_t, d_0, b_0, d, b, t_j = list(map(lambda s: list(map(int, s.split(' '))), input().split('; ')))
# T, N, d_0, b_0, d, b = T[0], N[0], d_0[0], b_0[0], d[0], b[0]


solver = Solver(T, N, D_t, B_t, d_0, b_0, d, b, t_j)
solution = solver.solve()
print(int(solution))