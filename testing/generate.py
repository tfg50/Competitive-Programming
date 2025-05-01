import random

def generate_array(n, L, R):
    test = ''
    for _ in range(n):
        test += str(random.randint(L, R)) + ' '
    test = test[:-1] + '\n'
    return test

def multitest(single_test_function, tests):
    test = str(tests) + '\n'
    for _ in range(tests):
        test += single_test_function()
    return test

# from here you should implement your test if necessary
# maybe put the code from above in a seperate file

def single_test_gen_array():
    n = 30
    k = 3
    return str(n) + ' ' + str(k) + '\n' + generate_array(n, 0, 2 ** k - 1)

def single_test():
    INF = 100
    n = random.randint(3, INF)
    x = random.randint(1, n-1)
    y = random.randint(1, n-1)
    vx = random.randint(1, INF)
    vy = random.randint(1, INF)
    return str(n) + ' ' + str(x) + ' ' + str(y) + ' ' + str(vx) + ' ' + str(vy) + '\n'

print(multitest(single_test, 1000))
exit(0)