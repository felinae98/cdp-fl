parties = [3, 10, 20, 30, 40, 50, 60, 70]
parties2 = [3, 10, 20, 25, 40, 50, 60, 70]
import re

data2 = []
data3=[]
data4 = []
for p in parties:
    with open(f'./data/res-2-{p}') as f:
        raw_data = f.read()
    m = re.search(f'onchain time (.+)', raw_data) 
    data2.append(float(m.group(1)))

for p in parties:
    with open(f'./data/res-4-{p}') as f:
        raw_data = f.read()
    m = re.search(f'onchain time (.+)', raw_data) 
    data4.append(float(m.group(1)))

for p in parties2:
    with open(f'./data/res-{p}') as f:
        raw_data = f.read()
    m = re.search(f'onchain time (.+)', raw_data) 
    data3.append(float(m.group(1)))

import matplotlib.pyplot as plt

plt.plot(parties, data2, label='$k=2$')
plt.plot(parties2, data3, label='$k=3$')
plt.plot(parties, data4, label='$k=4$')
plt.legend()
plt.xlabel("参与者数量")
plt.ylabel('使用时间/s')

# plt.show()

import tikzplotlib
tikzplotlib.save(
        '../../graduate-thesis/figures/rank-time.tex',
        extra_tikzpicture_parameters=['font=\\footnotesize'],
        # extra_groupstyle_parameters=['horizontal sep=2cm'],
        axis_width='.6\\textwidth',
        )
