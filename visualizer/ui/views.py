import os
import subprocess

from django.shortcuts import render
from django.conf import settings

from solver import (
    random_solver,
    heuristic_solver,
    utils,
)


INPUT = {
    0: 'tsp_51_1',
    1: 'vlsi_131',
    2: 'tsp_200_2',
    3: 'vlsi_237',
    4: 'vlsi_662',
    5: 'vlsi_1083',
}
OPTIMAL = {
    0: 428,
    1: 566,
    2: 29440,
    3: 1019,
    4: 2513,
    5: 3558,
}


def list(request):
    return render(request, 'tsp/list.html', {
        'N': [
            (0, 51),
            (1, 131),
            (2, 200),
            (3, 237),
            (4, 662),
            (5, 1083)
        ],
        'algo': [
            ('random', 'Random'),
            ('nearest', 'Heuristic - Nearest Neighbor'),
            ('2opt', 'Local Search - 2opt'),
            ('sa', 'Simulated Annealing'),
            ('gurobi', 'Mixed Integer Programming - Gurobi'),
        ],
    })


def _get_input_path(id):
    return os.path.join(settings.BASE_DIR, 'data', INPUT[id])


def _read_input(input_path):
    pts = []
    print 'in:', input_path

    with open(input_path) as f:
        n = int(f.readline())
        for i in xrange(n):
            x, y = map(float, f.readline().split())
            pts.append((x, y))
    return pts


def _read_output(output_path, n):
    path = []
    l = 0.0
    print 'out:', output_path

    with open(output_path) as f:
        l, _ = map(float, f.readline().split())
        path = map(int, f.readline().split())

    return l, path


def index(request):
    # Read input
    input_id = int(request.GET['in'])
    input_path = _get_input_path(input_id)
    pts = _read_input(input_path)

    # Read output

    algo = request.GET.get('algo', '')
    if algo == 'random':
        l, path = random_solver.process(pts)
    elif algo == 'nearest':
        l, path = heuristic_solver.nearest_neighbor(pts)
    elif algo == '2opt':
        subprocess.call(['./solver/cpp/local_search_2opt', input_path, '2opt.txt'])
        l, path = _read_output('2opt.txt', len(pts))
    elif algo == 'sa':
        subprocess.call(['./solver/cpp/simulated_annealing', input_path, 'sa.txt'])
        l, path = _read_output('sa.txt', len(pts))
    elif algo == 'gurobi':
        subprocess.call(['javac', '-classpath', '/opt/gurobi652/linux64/lib/gurobi.jar:.', './solver/java/SolutionGurobi2.java'])
        subprocess.call(['mv', './solver/java/SolutionGurobi2.class', '.'])
        subprocess.call(['java', '-classpath', '/opt/gurobi652/linux64/lib/gurobi.jar:.', 'SolutionGurobi2', _get_input_path(input_id), 'gurobi.txt'])

        l, path = _read_output('gurobi.txt', len(pts))
    else:
        try:
            l, path = _read_output(os.path.join(settings.BASE_DIR, 'data', str(input_id) + '.out'), len(pts))
        except:
            l = 0.0
            path = []

    segs = []
    actual = 0.0
    print 'n =', len(path)
    for i in xrange(len(path)):
        u = path[i]
        v = path[(i+1) % len(path)]

        segs.append((pts[u], pts[v]))
        actual += utils.get_point_distance(pts[u], pts[v])

    # Calculate stuff & return
    minx = min(p[0] for p in pts)
    maxx = max(p[0] for p in pts)
    miny = min(p[1] for p in pts)
    maxy = max(p[1] for p in pts)

    return render(request, 'tsp/index.html', {
        'n': len(pts),
        'optimal': OPTIMAL[input_id],
        'algo': algo,
        'minx': min([minx, 0]) - (maxx - minx) / 20,
        'maxx': maxx + (maxx - minx) / 20,
        'miny': min([miny, 0]) - (maxy - miny) / 15,
        'maxy': maxy + (maxx - minx) / 15,
        'pts': pts,
        'segs': segs,
        'reported_length': l,
        'actual_length': actual,
    })
