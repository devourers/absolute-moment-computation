import matplotlib.pyplot as plt
import numpy
import os
import glob

#passline = "txt_output/10/2pass_results_order10_1.txt"
#results_2pass= numpy.loadtxt(passline)


treap_line_1 = "txt_output/1/treap_results_order1_1.txt"
results_treap_1 = numpy.loadtxt(treap_line_1)
treap_line_3 = "txt_output/3/treap_results_order3_1.txt"
results_treap_3 = numpy.loadtxt(treap_line_3)
treap_line_5 = "txt_output/5/treap_results_order5_1.txt"
results_treap_5 = numpy.loadtxt(treap_line_5)
treap_line_7 = "txt_output/7/treap_results_order7_1.txt"
results_treap_7 = numpy.loadtxt(treap_line_7)

pass_line_1 = "txt_output/1/2pass_results_order1_1.txt"
results_2pass_1 = numpy.loadtxt(pass_line_1)
pass_line_3 = "txt_output/3/2pass_results_order3_1.txt"
results_2pass_3 = numpy.loadtxt(pass_line_3)
pass_line_5 = "txt_output/5/2pass_results_order5_1.txt"
results_2pass_5 = numpy.loadtxt(pass_line_5)
pass_line_7 = "txt_output/7/2pass_results_order7_1.txt"
results_2pass_7 = numpy.loadtxt(pass_line_7)



#target_results_2pass = numpy.empty([len(results_2pass), 21])
target_results_treap_1 = numpy.empty([len(results_treap_1), 21])
target_results_treap_3 = numpy.empty([len(results_treap_3), 21])
target_results_treap_5 = numpy.empty([len(results_treap_5), 21])
target_results_treap_7 = numpy.empty([len(results_treap_7), 21])

target_results_2pass_1 = numpy.empty([len(results_2pass_1), 21])
target_results_2pass_3 = numpy.empty([len(results_2pass_3), 21])
target_results_2pass_5 = numpy.empty([len(results_2pass_5), 21])
target_results_2pass_7 = numpy.empty([len(results_2pass_7), 21])



for i in range(len(results_treap_1)):
   #target_results_2pass[i, 0] = results_2pass[i, 0]
   target_results_treap_1[i, 0] = results_treap_1[i, 0]
   target_results_treap_3[i, 0] = results_treap_3[i, 0]
   target_results_treap_5[i, 0] = results_treap_5[i, 0]
   target_results_treap_7[i, 0] = results_treap_7[i, 0]
   
   target_results_2pass_1[i, 0] = results_2pass_1[i, 0]
   target_results_2pass_3[i, 0] = results_2pass_3[i, 0]
   target_results_2pass_5[i, 0] = results_2pass_5[i, 0]
   target_results_2pass_7[i, 0] = results_2pass_7[i, 0]
   
for i in range(2,20):
   #passline = "txt_output/1/2pass_results_order1_"
   #passline += str(i)
   #passline += ".txt"
   #results_2pass_curr = numpy.loadtxt(passline)
   
   treap_line_1 = "txt_output/1/treap_results_order1_"
   treap_line_3 = "txt_output/3/treap_results_order3_"
   treap_line_5 = "txt_output/5/treap_results_order5_"
   treap_line_7 = "txt_output/7/treap_results_order7_"
   treap_line_1 += str(i)
   treap_line_1 += ".txt"
   treap_line_3 += str(i)
   treap_line_3 += ".txt"
   treap_line_5 += str(i)
   treap_line_5 += ".txt"
   treap_line_7 += str(i)
   treap_line_7 += ".txt"  
   results_treap_curr_1 = numpy.loadtxt(treap_line_1)
   results_treap_curr_3 = numpy.loadtxt(treap_line_3)
   results_treap_curr_5 = numpy.loadtxt(treap_line_5)
   results_treap_curr_7 = numpy.loadtxt(treap_line_7)
   
   pass_line_1 = "txt_output/1/2pass_results_order1_"
   pass_line_3 = "txt_output/3/2pass_results_order3_"
   pass_line_5 = "txt_output/5/2pass_results_order5_"
   pass_line_7 = "txt_output/7/2pass_results_order7_"
   pass_line_1 += str(i)
   pass_line_1 += ".txt"
   pass_line_3 += str(i)
   pass_line_3 += ".txt"
   pass_line_5 += str(i)
   pass_line_5 += ".txt"
   pass_line_7 += str(i)
   pass_line_7 += ".txt"  
   results_2pass_curr_1 = numpy.loadtxt(pass_line_1)
   results_2pass_curr_3 = numpy.loadtxt(pass_line_3)
   results_2pass_curr_5 = numpy.loadtxt(pass_line_5)
   results_2pass_curr_7 = numpy.loadtxt(pass_line_7)   
   
   for j in range(len(results_treap_curr_1)):
      #target_results_2pass_1[j, i] = results_2pass_curr_1[j, 0]
      target_results_treap_1[j, i] = results_treap_curr_1[j, 0]
      target_results_treap_3[j, i] = results_treap_curr_3[j, 0]
      target_results_treap_5[j, i] = results_treap_curr_5[j, 0]
      target_results_treap_7[j, i] = results_treap_curr_7[j, 0]
      
      target_results_2pass_1[j, i] = results_2pass_curr_1[j, 0]
      target_results_2pass_3[j, i] = results_2pass_curr_3[j, 0]
      target_results_2pass_5[j, i] = results_2pass_curr_5[j, 0]
      target_results_2pass_7[j, i] = results_2pass_curr_7[j, 0]      


for i in range(len(target_results_treap_1)):
   #target_results_2pass[i] = numpy.sort(target_results_2pass[i])
   target_results_treap_1[i] = numpy.sort(target_results_treap_1[i])
   target_results_treap_3[i] = numpy.sort(target_results_treap_3[i])
   target_results_treap_5[i] = numpy.sort(target_results_treap_5[i])
   target_results_treap_7[i] = numpy.sort(target_results_treap_7[i])
   
   target_results_2pass_1[i] = numpy.sort(target_results_2pass_1[i])
   target_results_2pass_3[i] = numpy.sort(target_results_2pass_3[i])
   target_results_2pass_5[i] = numpy.sort(target_results_2pass_5[i])
   target_results_2pass_7[i] = numpy.sort(target_results_2pass_7[i])   



axis1 = numpy.arange(0, 99999, 1)
fig, axs = plt.subplots(2, 2)

for ax in axs.flat:
   ax.set(xlabel="Number of elements in sample", ylabel="Time to compute")
   ax.set_xlim(0, 100000)
   ax.set_ylim(0, 900000)
   
   
axs[0,0].plot(axis1, target_results_treap_1[:, 10], 'r', label = "Treap")
axs[0,0].plot(axis1, target_results_2pass_1[:, 10], 'black', label = "Classic")
axs[0,0].set_title('Order 1')
axs[0,1].plot(axis1, target_results_treap_3[:, 10], 'r', label = "Treap")
axs[0,1].plot(axis1, target_results_2pass_3[:, 10], 'black', label = "Classic")
axs[0,1].set_title('Order 3')
axs[1,0].plot(axis1, target_results_treap_5[:, 10], 'r', label = "Treap")
axs[1,0].plot(axis1, target_results_2pass_5[:, 10], 'black', label = "Classic")
axs[1,0].set_title('Order 5')
axs[1,1].plot(axis1, target_results_treap_7[:, 10], 'r', label = "Treap")
axs[1,1].plot(axis1, target_results_2pass_7[:, 10], 'black', label = "Classic")
axs[1,1].set_title('Order 7')

plt.show()

