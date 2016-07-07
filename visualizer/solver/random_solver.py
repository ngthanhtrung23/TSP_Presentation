from random import shuffle
from .utils import get_path_length


def random_permutation(n):
    res = range(n)
    shuffle(res)
    return res


def process(pts):
    n = len(pts)
    path = random_permutation(n)
    length = get_path_length(path, pts)
    print 'Init length =', length

    MAX_TURN = 10 ** 4
    for turn in xrange(MAX_TURN):
        new_path = random_permutation(n)
        new_length = get_path_length(new_path, pts)

        if new_length < length:
            print 'Improved: ', length, ' --> ', new_length
            length = new_length
            path = new_path

    return length, path
