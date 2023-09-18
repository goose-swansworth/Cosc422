import numpy as np
from matplotlib import image
import matplotlib.pyplot as plt

img = image.imread('Bays.png')
image.imsave("bays-normalised.png", img / np.max(img))