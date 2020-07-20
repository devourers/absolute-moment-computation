import matplotlib.pyplot as plt
import numpy as np

results_treap = []

print("Please enter a number. 0 will plot the total time for orders 1-9, any other will plot comparasment for each order and sample size")
decision = int(input())

if decision == 0:
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
    
else:
    fig = plt.figure()
    plt.title('Comparasment of moments')
    plt.xlabel('Number of elements in A')
    plt.ylabel('Time to compute')
    frame1 = plt.gca()
    frame1.get_yaxis().set_visible(True)
    #frame1.set_yscale('log')    
    plot_list = []
    label_list = []
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
        axis_another = np.arange(1, len(target_results_treap)+1, 1)
        curr, = plt.plot(axis_another, target_results_treap[:, 0], label = 'Order ' + str(i))
        plot_list.append(curr)
        label_list.append("Order " + str(i))
        print(str(i) + "/" + str(9))
    plt.legend(plot_list, label_list)
    plt.show()