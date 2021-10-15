import numpy as np

def safeNorm(vec):
    return vec / np.sqrt(np.sum(vec ** 2) + 1e-18)

def fibonacciSphere(samples, angle, direction):
    points = []
    phi = np.pi * (3. - np.sqrt(5.))  # golden angle in radians

    y_max = 1
    y_min = np.cos(angle / 2)

    for i in range(samples):
        y = y_max - (i / float(samples - 1)) * (y_max - y_min) # y goes from 1 to -1
        radius = np.sqrt(1 - y * y)  # radius at y

        theta = phi * i # golden angle increment

        x = np.cos(theta) * radius
        z = np.sin(theta) * radius

        points.append((x, y, z))

    return points
