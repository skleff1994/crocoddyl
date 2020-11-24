import crocoddyl
from crocoddyl.utils import UnicycleModelDerived
import numpy as np
import os
import sys
import time
import subprocess

N = 200  # number of nodes
T = int(sys.argv[1]) if (len(sys.argv) > 1) else int(5e3)  # number of trials
MAXITER = 1
CALLBACKS = False


def createProblem(model):
    x0 = np.matrix([[1.], [0.], [0.]])
    runningModels = [model()] * N
    terminalModel = model()
    xs = [x0] * (N + 1)
    us = [np.matrix([[0.], [0.]])] * N

    problem = crocoddyl.ShootingProblem(x0, runningModels, terminalModel)
    return xs, us, problem


def runDDPSolveBenchmark(xs, us, problem):
    ddp = crocoddyl.SolverDDP(problem)
    if CALLBACKS:
        ddp.setCallbacks([crocoddyl.CallbackVerbose()])
    duration = []
    for i in range(T):
        c_start = time.time()
        ddp.solve(xs, us, MAXITER)
        c_end = time.time()
        duration.append(1e3 * (c_end - c_start))

    avrg_duration = sum(duration) / len(duration)
    min_duration = min(duration)
    max_duration = max(duration)
    return avrg_duration, min_duration, max_duration


def runShootingProblemCalcBenchmark(xs, us, problem):
    duration = []
    for i in range(T):
        c_start = time.time()
        problem.calc(xs, us)
        c_end = time.time()
        duration.append(1e3 * (c_end - c_start))

    avrg_duration = sum(duration) / len(duration)
    min_duration = min(duration)
    max_duration = max(duration)
    return avrg_duration, min_duration, max_duration


def runShootingProblemCalcDiffBenchmark(xs, us, problem):
    duration = []
    for i in range(T):
        c_start = time.time()
        problem.calcDiff(xs, us)
        c_end = time.time()
        duration.append(1e3 * (c_end - c_start))

    avrg_duration = sum(duration) / len(duration)
    min_duration = min(duration)
    max_duration = max(duration)
    return avrg_duration, min_duration, max_duration


print('\033[1m')
print('C++:')
popen = subprocess.check_call([os.path.dirname(os.path.abspath(__file__)) + "/unicycle-optctrl", str(T)])

print('Python bindings:')
xs, us, problem = createProblem(crocoddyl.ActionModelUnicycle)
avrg_duration, min_duration, max_duration = runDDPSolveBenchmark(xs, us, problem)
print('  DDP.solve [ms]: {0} ({1}, {2})'.format(avrg_duration, min_duration, max_duration))
avrg_duration, min_duration, max_duration = runShootingProblemCalcBenchmark(xs, us, problem)
print('  ShootingProblem.calc [ms]: {0} ({1}, {2})'.format(avrg_duration, min_duration, max_duration))
avrg_duration, min_duration, max_duration = runShootingProblemCalcDiffBenchmark(xs, us, problem)
print('  ShootingProblem.calcDiff [ms]: {0} ({1}, {2})'.format(avrg_duration, min_duration, max_duration))

print('Python:')
xs, us, problem = createProblem(UnicycleModelDerived)
avrg_duration, min_duration, max_duration = runDDPSolveBenchmark(xs, us, problem)
print('  DDP.solve [ms]: {0} ({1}, {2})'.format(avrg_duration, min_duration, max_duration))
avrg_duration, min_duration, max_duration = runShootingProblemCalcBenchmark(xs, us, problem)
print('  ShootingProblem.calc [ms]: {0} ({1}, {2})'.format(avrg_duration, min_duration, max_duration))
avrg_duration, min_duration, max_duration = runShootingProblemCalcDiffBenchmark(xs, us, problem)
print('  ShootingProblem.calcDiff [ms]: {0} ({1}, {2})'.format(avrg_duration, min_duration, max_duration))
print('\033[0m')
