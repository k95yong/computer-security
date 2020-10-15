import random
from exponentian import exp


def dlog(b, a, m):
    cnt = 0
    cur = 1
    while cur != a:
        cur *= b
        cur %= m
        cnt += 1
    return cnt


if __name__ == "__main__":
    base = 3
    modulus = 65537
    expos = [
        1,
        2,
        65535
    ]

    try:
        for expo in expos:
            powed = exp(base, expo, modulus)
            logged = dlog(base, powed, modulus)
            print("({},{},{},{},{}),".format(expo, base, powed, logged, modulus))
            assert expo == logged
        print(">> All the tests passed <<")
    except:
        print("expo({}) and logged({}) is not equal".format(expo, logged))