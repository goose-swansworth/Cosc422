
import matplotlib.pyplot as plt
import numpy as np

def R(theta):
    return np.array([
        [np.cos(theta), -np.sin(theta)],
        [np.sin(theta), np.cos(theta)]
    ])
    

def ccd(joints, target):
    axes = plt.axes()
    axes.plot(*zip(*joints), marker="o")
    axes.grid(True)
    axes.set_xlim((-0.1, 8))
    axes.set_ylim((-0.1, 8))
    plt.show()

joints = np.array([[i, 0] for i in range(7)])

ccd(joints, [1, 1])