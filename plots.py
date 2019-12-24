import matplotlib.pyplot
import numpy
import os
import glob

results = [[] for f in glob.glob('*.txt')]
i = 0
for filename in glob.glob('*.txt'):
   results = numpy.loadtxt(filename)
   i += 1
   
print(results)
   

axis1 = numpy.arange(0, len(results), 1)
fig = matplotlib.pyplot.figure()



matplotlib.pyplot.plot(axis1, results[:,0])
matplotlib.pyplot.title('Moment of order 10 via 2 pass algorithm')
matplotlib.pyplot.xlabel('Number of elements in A')
matplotlib.pyplot.ylabel('Time to compute')

#for i in range(len(results)):
#   matplotlib.pyplot.subplot(1, 1, i+1)
#   matplotlib.pyplot.plot(axis1, results[i])


frame1 = matplotlib.pyplot.gca()
frame1.get_yaxis().set_visible(True)
matplotlib.pyplot.show()
    
    
