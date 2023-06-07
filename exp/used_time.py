party_numbers = [3, 5, 10, 15, 20, 25, 40, 50, 60, 70, 80, 90, 100]

import re
from statistics import mean
def read_data(party_number: int) -> tuple[float, float, float, float]:
    with open(f'./data/res-{party_number}', 'r') as f:
        raw_data = f.read()
        res = re.findall(r'onchain: (.+)', raw_data) 
        gate_res = re.search(r'onchain gate ([\d\.]+)', raw_data)
    res = list(map(float, res))
    return mean(res), max(res), min(res), float(gate_res.group(1))

mean_data = []
max_data = []
min_data = []
gate_data = []
for pn in party_numbers:
    mean_, max_, min_, gate = read_data(pn)
    mean_data.append(mean_)
    max_data.append(max_ - mean_)
    min_data.append(mean_ - min_)
    gate_data.append(gate)

import matplotlib.pyplot as plt
from mplfonts import use_font
use_font('WenQuanYi Micro Hei')


fig, (ax1, ax2) = plt.subplots(1,2)
ax1.errorbar(party_numbers, mean_data, yerr=[max_data, min_data])
ax1.set_ylabel('执行时间/s')
ax1.set_xlabel('参与者数量')
ax1.set_title('参与者数量与执行时间的关系')

# ax2 = ax1.twinx()
ax2.plot(party_numbers, gate_data)
ax2.set_ylabel('同态门数量')
ax2.set_xlabel('参与者数量')
ax2.set_title('参与者数量与同态门数量的关系')

# plt.show()

import tikzplotlib
tikzplotlib.save(
        filepath='../../graduate-thesis/figures/c4-paties-time.tex',
        extra_tikzpicture_parameters=['font=\\footnotesize'],
        extra_groupstyle_parameters=['horizontal sep=2cm'],
        axis_width='.47\\textwidth',
        )
