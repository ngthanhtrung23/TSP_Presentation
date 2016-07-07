import math


def get_point_distance(a, b):
    """
    Return distance between 2 points
    """
    dx = a[0] - b[0]
    dy = a[1] - b[1]
    return math.sqrt(dx * dx + dy * dy)


def get_path_length(path, pts):
    """
    Return length of a path
    Params:
    - path: permutation representing path
    - pts: points
    """
    n = len(path)
    res = 0.0
    for i in xrange(n):
        u = pts[path[i]]
        v = pts[path[(i + 1) % n]]
        res += get_point_distance(u, v)
    return res
