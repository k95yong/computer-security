def exp(a, b, n):
    c = 0
    f = 1
    bin_b = int_to_bin(b)
    k = len(bin_b)
    for i in range(k):
        c = 2 * c
        f = (f * f) % n
        if bin_b[i] == '1':
            c = c + 1
            f = (f * a) % n
    return f

def int_to_bin(num):
    return list(bin(num))[2:]