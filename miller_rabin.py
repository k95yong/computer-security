import random
from exponentian import exp
Prime = 0
Composite = 1


def miller_rabin(n, s):
    if n == 2:
        return Prime
    elif n % 2 == 0:
        return Composite
    for _ in range(s):
        a = random.randint(1, n-1)
        if test(a, n):
            return Composite
        return Prime


def get_tu(n):
    nn = n-1
    t = 0
    u = 0
    while nn % 2 == 0:
        nn = int(nn / 2)
        t += 1
    u = int((n-1) / (1 << t))
    return int(t), int(u)


def test(a, n):
    t, u = get_tu(n)
    arr = [exp(a, u, n)]
    for i in range(1, t+1):
        arr.append(exp(arr[i-1], 2, n))
        if arr[i] == 1 and arr[i-1] != 1 and arr[i-1] != n-1:
            return True
    if arr[t] != 1:
        return True
    return False;


if __name__ == "__main__":

    primes = [7879, 7883, 7901, 7907, 7919, 7927, 7933, 7937, 7949, 7951,
              7963, 7993, 8009, 8011, 8017, 8039, 8053, 8059, 8069, 8081,
              8087, 8089, 8093, 8101, 8111, 8117, 8123, 8147, 8161, 8167]
    for p in primes:
        result = miller_rabin(p, 20)
        if result == Prime:
            print("Prime")
        elif result == Composite:
            print("Composite")
        else:
            print("Undefined")