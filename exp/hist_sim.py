import numpy as np
import matplotlib.pyplot as plt

def sim(i,rank):
    j = i- 0.5
    sgn = np.sign(j)
    j = np.abs(j) * 2
    res = 0
    for r in range(1, rank+1):
        res += j**r / r
    return res * sgn

ranks = [3, 5, 10]

for rank in ranks:
    rand = np.random.rand(int(1e7))
    res = sim(rand, rank)
    counts, bins = np.histogram(res, bins=300, range=(-3, 3))
    plt.stairs(counts, bins, label=f'$k={rank}$')

plt.legend()

# plt.show()

import tikzplotlib
tikzplotlib.save('../../graduate-thesis/figures/hist-sim.tex',
        extra_tikzpicture_parameters=['font=\\footnotesize'],
        axis_width='.7\\textwidth',
                 )
