import matplotlib.pyplot as plt
import numpy as np

results_treap = []

for i in range (1, 10):
    target_results_treap = np.empty([100000, 21])
    curr_number = 0
    for k in range(1, 21):
        passline = "txt_output/"
        passline += str(i)
        passline += "/treap_results_order"
        passline += str(i)
        passline += "_"
        passline += str(k)
        passline += ".txt"
        curr = np.loadtxt(passline)
        for j in range(len(curr)):
            target_results_treap[j, k] = curr[j, 0]
    for l in range(len(target_results_treap)):
        target_results_treap[l] = np.sort(target_results_treap[l])
        curr_number += target_results_treap[l, 10]
    results_treap.append([i, curr_number])
     
results_treap = np.array(results_treap)
print(results_treap)

plt.plot(results_treap[:, 0], results_treap[:, 1])
plt.show()

