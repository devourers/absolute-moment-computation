import matplotlib.pyplot
import numpy
import os
import glob

results = [[] for f in glob.glob('*.txt')]
i = 0
for filename in glob.glob('*.txt'):
   results[i] = numpy.loadtxt(filename)
   i += 1
   
   

axis1 = numpy.arange(0, len(results[0]), 1)
fig = matplotlib.pyplot.figure()



for i in range(len(results)):
   matplotlib.pyplot.subplot(1, 1, i+1)
   matplotlib.pyplot.plot(axis1, results[i])
   matplotlib.pyplot.title('Moment of order ' + str(i))

frame1 = matplotlib.pyplot.gca()
frame1.get_yaxis().set_visible(True)
matplotlib.pyplot.show()
    
    
