import matplotlib.pyplot
import numpy
import os
import glob

passline = "results/2pass_results0.txt"
results_2pass= numpy.loadtxt(passline)


treap_line = "results/treap_results_new0.txt"
results_treap = numpy.loadtxt(treap_line)



target_results_2pass = numpy.empty([len(results_2pass), 5])
target_results_treap = numpy.empty([len(results_treap), 5])


for i in range(len(results_2pass)):
   target_results_2pass[i, 0] = results_2pass[i, 0]
   target_results_treap[i, 0] = results_treap[i, 0]

for i in range(1,5):
   passline = "results/2pass_results"
   passline += str(i)
   passline += ".txt"
   results_2pass_curr = numpy.loadtxt(passline)
   
   treap_line = "results/treap_results_new"
   treap_line += str(i)
   treap_line += ".txt"
   results_treap_curr = numpy.loadtxt(treap_line)   
   
   for j in range(len(results_2pass_curr)):
      target_results_2pass[j, i] = results_2pass_curr[j, 0]
      target_results_treap[j, i] = results_treap_curr[j, 0]


for i in range(len(target_results_2pass)):
   target_results_2pass[i] = numpy.sort(target_results_2pass[i])
   target_results_treap[i] = numpy.sort(target_results_treap[i])



fig = matplotlib.pyplot.figure()
matplotlib.pyplot.title('Moment of order 4')
matplotlib.pyplot.xlabel('Number of elements in A')
matplotlib.pyplot.ylabel('Time to compute')
axis1 = numpy.arange(0, 99999, 1)
frame1 = matplotlib.pyplot.gca()
frame1.get_yaxis().set_visible(True)
matplotlib.pyplot.plot(axis1, target_results_treap[:, 2], 'r')
matplotlib.pyplot.plot(axis1, target_results_2pass[:, 2], 'k')
matplotlib.pyplot.show()