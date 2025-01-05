#############################
# the code is taken from the examples section for the MIP package (https://docs.python-mip.com/en/latest/examples.html)
# supplement the example code with your own one below

from itertools import product
from sys import stdout as out
from mip import Model, xsum, minimize, BINARY, CBC
import numpy as np
import time

# names of places to visit
places = ['Antwerp', 'Bruges', 'C-Mine', 'Dinant', 'Ghent',
          'Grand-Place de Bruxelles', 'Hasselt', 'Leuven',
          'Mechelen', 'Mons', 'Montagne de Bueren', 'Namur',
          'Remouchamps', 'Waterloo']

# distances in an upper triangular matrix
dists = [[83, 81, 113, 52, 42, 73, 44, 23, 91, 105, 90, 124, 57],
         [161, 160, 39, 89, 151, 110, 90, 99, 177, 143, 193, 100],
         [90, 125, 82, 13, 57, 71, 123, 38, 72, 59, 82],
         [123, 77, 81, 71, 91, 72, 64, 24, 62, 63],
         [51, 114, 72, 54, 69, 139, 105, 155, 62],
         [70, 25, 22, 52, 90, 56, 105, 16],
         [45, 61, 111, 36, 61, 57, 70],
         [23, 71, 67, 48, 85, 29],
         [74, 89, 69, 107, 36],
         [117, 65, 125, 43],
         [54, 22, 84],
         [60, 44],
         [97],
         []]

# number of nodes and list of vertices
n, V = len(dists), set(range(len(dists)))

# distances matrix
c = [[0 if i == j
      else dists[i][j-i-1] if j > i
      else dists[j][i-j-1]
      for j in V] for i in V]

model = Model(solver_name=CBC)
model.verbose = False

# binary variables indicating if arc (i,j) is used on the route or not
x = [[model.add_var(var_type=BINARY) for j in V] for i in V]

# continuous variable to prevent subtours: each city will have a
# different sequential id in the planned route except the first one
y = [model.add_var() for i in V]

# objective function: minimize the distance
model.objective = minimize(xsum(c[i][j]*x[i][j] for i in V for j in V))

# constraint : leave each city only once
for i in V:
    model += xsum(x[i][j] for j in V - {i}) == 1

# constraint : enter each city only once
for i in V:
    model += xsum(x[j][i] for j in V - {i}) == 1

# subtour elimination
for (i, j) in product(V - {0}, V - {0}):
    if i != j:
        model += y[i] - (n+1)*x[i][j] >= y[j]-n

# optimizing
model.optimize()

# checking if a solution was found
if model.num_solutions:
    out.write('route with total distance %g found: %s'
              % (model.objective_value, places[0]))
    nc = 0
    while True:
        nc = [i for i in V if x[nc][i].x >= 0.99][0]
        out.write(' -> %s' % places[nc])
        if nc == 0:
            break
    out.write('\n')


#### YOUR CODE HERE VVVVVVVVVVVVVVVVVV

# TASK 1
def task_1(matr, vertexes):
    model = Model(solver_name=CBC)
    model.verbose = False

    x = [[model.add_var() for j in vertexes] for i in vertexes]
    y = [model.add_var() for i in vertexes]

    model.objective = minimize(xsum(matr[i][j]*x[i][j] for i in vertexes for j in vertexes))

    for i in V:
        model += xsum(x[i][j] for j in vertexes - {i}) == 1
    for i in V:
        model += xsum(x[j][i] for j in vertexes - {i}) == 1
    for (i, j) in product(vertexes - {0}, vertexes - {0}):
        if i != j:
            model += y[i] - (n+1)*x[i][j] >= y[j]-n

    model.optimize()

    if model.num_solutions:
        order = [0]
        nc = 0
        while True:
            nc = [i for i in V if x[nc][i].x >= 0.99][0]
            order.append(nc)
            if nc == 0:
                return model.objective_value, order
    return None

cont_value, cont_order = task_1(matr=c, vertexes=V)

print('//////////TASK_1/////////////')
print(f'Integer value: {model.objective_value:.2f}')
print(f'Continuous value: {cont_value:.2f}')
print(f'F_MIP - F_LP = {model.objective_value - cont_value:.2f}')
print('/////////////////////////////')
print()

# TASK 2
def task_2(matr, vertexes):
    model = Model(solver_name=CBC)
    model.verbose = False

    x = [[model.add_var(var_type=BINARY) for j in vertexes] for i in vertexes]

    model.objective = minimize(xsum(matr[i][j]*x[i][j] for i in vertexes for j in vertexes))

    for i in vertexes:
        model += xsum(x[i][j] for j in vertexes - {i}) == 1
    for j in vertexes:
        model += xsum(x[i][j] for i in vertexes - {j}) == 1

    model.optimize()

    if model.num_solutions:
        visited = set()
        cycles = []
        for i in range(len(matr)):
            if i in visited:
                continue
            curr_cycle = []
            curr_vert = i
            while True:
                curr_cycle.append(curr_vert)
                visited.add(curr_vert)
                next_node, *_ = [j for j in range(len(matr)) if x[curr_vert][j].x >= 0.99]
                curr_vert = next_node
                if curr_vert == i:
                    curr_cycle.append(curr_vert)
                    break
            cycles.append(curr_cycle)
        return model.objective_value, cycles
    return None

target_value_2, cycles_2 = task_2(matr=c, vertexes=V)

print('//////////TASK_2/////////////')
print(f'Value: {target_value_2:.2f}')
for i, cycle in enumerate(cycles_2):
    for j, vert in enumerate(cycle):
        print(places[vert], end='')
        if j != len(cycle) - 1:
            print(' -> ', end='')
    print()   
print('/////////////////////////////')
print()

# TASK 3
def check_cycle(matr):
    cycle = [0]
    visited = {0}
    stack = [0]
    while stack:
        curr_vert = stack.pop()
        neighbour = [i for i in range(0, len(matr)) if matr[curr_vert][i] >= 0.99][0]
        if neighbour in visited:
            return cycle
        visited.add(neighbour)
        cycle.append(neighbour)
        stack.append(neighbour)
    return []

def task_3(matr, vertexes):
    model = Model(solver_name=CBC)
    model.verbose = False

    x = [[model.add_var(var_type=BINARY) for j in vertexes] for i in vertexes]

    model.objective = minimize(xsum(matr[i][j]*x[i][j] for i in vertexes for j in vertexes))

    for i in vertexes:
        model += xsum(x[i][j] for j in vertexes - {i}) == 1
    for j in vertexes:
        model += xsum(x[i][j] for i in vertexes - {j}) == 1

    model.optimize()
    inner_opt = [model.objective_value]
    is_cycle = True
    res_cycle = []
    iterations = 0
    while is_cycle:
        iterations += 1
        curr_matrix = [[item.x for item in line] for line in x]
        curr_cycle = check_cycle(curr_matrix)
        if len(curr_cycle) < len(curr_matrix):
            diff = list(set(vertexes) - set(curr_cycle))

            model += xsum(x[i][j] for i in curr_cycle for j in diff) >= 1
            model.optimize()
            inner_opt.append(model.objective_value)
        else:
            is_cycle = False
            res_cycle = curr_cycle.copy()
            res_cycle.append(res_cycle[0])
            break
    return model.objective_value, res_cycle, iterations, inner_opt

def template_solver(matr, vertexes):
    model = Model(solver_name=CBC)
    model.verbose = False

    x = [[model.add_var(var_type=BINARY) for j in vertexes] for i in vertexes]
    y = [model.add_var() for i in vertexes]

    model.objective = minimize(xsum(matr[i][j]*x[i][j] for i in vertexes for j in vertexes))

    for i in vertexes:
        model += xsum(x[i][j] for j in vertexes - {i}) == 1
    for i in vertexes:
        model += xsum(x[j][i] for j in vertexes - {i}) == 1
    for (i, j) in product(vertexes - {0}, vertexes - {0}):
        if i != j:
            model += y[i] - (n+1)*x[i][j] >= y[j]-n

    model.optimize()
    if model.num_solutions:
        order = [0]
        nc = 0
        while True:
            nc = [i for i in vertexes if x[nc][i].x >= 0.99][0]
            order.append(nc)
            if nc == 0:
                return model.objective_value, order
    return None
    
new_c = np.array(c)[:8, :8]
new_places = places[:8]
new_V = set(range(len(new_c)))

_, cycles = task_2(matr=new_c, vertexes=new_V)

# действительно несколько подциклов
assert len(cycles) > 1

start_time_1 = time.time()
target_value_3, cycle_3, iterations, inner_opt = task_3(matr=new_c, vertexes=new_V)
my_solver_time = time.time() - start_time_1

start_time_2 = time.time()
template_value, template_cycle = template_solver(matr=new_c, vertexes=new_V)
template_solver_time = time.time() - start_time_2

assert target_value_3 == template_value
assert len(cycle_3) ==  len(template_cycle)

print('//////////TASK_3/////////////')
print(f'My solver value: {target_value_3:.2f}')
for i, vert in enumerate(cycle_3):
        print(new_places[vert], end='')
        if i != len(cycle_3) - 1:
            print(' -> ', end='')
print()
print(f'Optimal values for each iteration: {inner_opt}')
print(f'My solver iterations: {iterations}')
print(f'My solver execution time: {my_solver_time:.2f} sec')
print()

print(f'Template solver value: {template_value:.2f}')
for i, vert in enumerate(template_cycle):
        print(new_places[vert], end='')
        if i != len(template_cycle) - 1:
            print(' -> ', end='')
print()
print(f'Template solver execution time: {template_solver_time:.2f} sec')
print('/////////////////////////////')