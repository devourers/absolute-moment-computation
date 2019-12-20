import matplotlib.pyplot
import numpy

results = []

with open("results.txt") as file:
    results = [line.strip() for line in file]
    
    
axis1 = numpy.arange(0, len(results), 1)
fig = matplotlib.pyplot.figure()
frame1 = matplotlib.pyplot.gca()
frame1.get_yaxis().set_visible(False)
matplotlib.pyplot.scatter(axis1, results)
matplotlib.pyplot.show()
    
    
