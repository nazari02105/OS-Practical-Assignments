import numpy as np
import matplotlib.pyplot as plt

fig, axs = plt.subplots(4, 1)
for i in range(1, 5):
    to_draw = []
    with open(f"result_{i}.txt", "r") as f:
        while True:
            line = f.readline()
            if not line:
                break
            if "length" in line:
                length = line.split(" ")[-1]
                length = int(length[1:len(length)-2])
                to_draw.append(length)
    to_draw[0] = 4
    to_draw[-1] = 4
    axs[i-1].plot(np.array(to_draw))
    axs[i-1].set_title(f"Result {i}")
    fig.subplots_adjust(hspace=1)
plt.show()