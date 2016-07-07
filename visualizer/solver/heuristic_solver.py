from .utils import get_point_distance, get_path_length


INFINITY = 1e100


def nearest_neighbor(pts):
    n = len(pts)
    seen = [False for i in xrange(n)]

    path = [0]
    seen[0] = True

    for i in xrange(1, n):
        last_pt = pts[path[i-1]]
        best_dist = INFINITY
        best_neighbor = -1

        for j in xrange(n):
            if not seen[j]:
                cur_dist = get_point_distance(last_pt, pts[j])
                if cur_dist < best_dist:
                    best_dist = cur_dist
                    best_neighbor = j
        path.append(best_neighbor)
        seen[best_neighbor] = True

    return get_path_length(path, pts), path
