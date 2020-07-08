import matplotlib.pyplot
import numpy
import os
import glob

passline = "results/2pass_results0.txt"
results_2pass= numpy.loadtxt(passline)


treap_line = "results/treap_results_new0.txt"
results_treap = numpy.loadtxt(treap_line)


for i in range(1, 250):
   passline = "results/2pass_results"
   passline += str(i)
   passline += ".txt"
   results_2pass += numpy.loadtxt(passline)
   results_2pass_curr = numpy.loadtxt(passline)
   '''
   for j in range (len(results_2pass)):
      if results_2pass_curr[j][0] < results_2pass[j][0]:
         results_2pass[j][0] = results_2pass_curr[j][0]
   '''
   treap_line = "results/treap_results_new"
   treap_line += str(i)
   treap_line += ".txt"
   results_treap += numpy.loadtxt(treap_line)
   '''
   results_treap_curr = numpy.loadtxt(treap_line)
   for j in range (len(results_2pass)):
      if results_treap_curr[j][0] < results_treap[j][0]:
         results_treap[j][0] = results_treap_curr[j][0]
   '''


results_treap /= 250
results_2pass /= 250
   
fig = matplotlib.pyplot.figure()
matplotlib.pyplot.title('Moment of order 10')
matplotlib.pyplot.xlabel('Number of elements in A')
matplotlib.pyplot.ylabel('Time to compute')
axis1 = numpy.arange(0, 10000, 1)
axis2 = numpy.arange(0, 1000, 1)
frame1 = matplotlib.pyplot.gca()
frame1.get_yaxis().set_visible(True)
matplotlib.pyplot.plot(axis1, results_treap[:,0], 'r')
matplotlib.pyplot.plot(axis1, results_2pass[:,0], 'k')
matplotlib.pyplot.show()
matplotlib.pyplot.plot(axis2, results_treap[:1000,0], 'r')
matplotlib.pyplot.plot(axis2, results_2pass[:1000,0], 'k')
matplotlib.pyplot.show()