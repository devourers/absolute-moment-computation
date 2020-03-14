import matplotlib.pyplot
import numpy
import os
import glob

results_treap = numpy.loadtxt("treap_results.txt")
results_2pass = numpy.loadtxt("2pass_results.txt")

axis1 = numpy.arange(0, len(results_treap), 1)

#print(results_treap)
print(len(results_2pass))

fig = matplotlib.pyplot.figure()

matplotlib.pyplot.plot(axis1, results_treap[:,0])
matplotlib.pyplot.plot(axis1, results_2pass[:,0])
matplotlib.pyplot.title('Moment of order 2')
matplotlib.pyplot.xlabel('Number of elements in A')
matplotlib.pyplot.ylabel('Time to compute')

frame1 = matplotlib.pyplot.gca()
frame1.get_yaxis().set_visible(True)
matplotlib.pyplot.show()
