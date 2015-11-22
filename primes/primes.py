#!/usr/bin/python

# Generate primes

def primes(N):
    pr = list(range(N))
    pr[0] = 0
    pr[1] = 0
    i = 2
    while i < len(pr):
        if pr[i] != 0:
            j = i + i
            while j < len(pr):
                pr[j] = 0
                j += i
        i += 1
    return list(filter(lambda x : x != 0, pr))

primes = primes(100000)

# Calculate

def factor(x):
    if x <= 1:
        return [x]
    f = list()
    while x > 1:
        found = False
        for p in primes:
            if x % p == 0:
                f.append(p)
                x //= p
                found = True
                break
        if not found:
            f.append(x)
            break
    return f

# Main

if __name__ == "__main__":
    import sys
    for line in sys.stdin:
        try:
            x = int(line)
        except:
            continue
        print(" x ".join([str(p) for p in factor(x)]))
