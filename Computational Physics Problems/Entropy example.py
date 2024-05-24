import numpy as np
import matplotlib.pyplot as plt
import math
import random
import time

#Calculate the entropy for the cream-in-your-coffee problem

start_time = time.time()

num_atoms = 400
atoms = np.ones([400,2])*100 #400 atoms centered at (100,100)
N = 15000 #15000 steps each for 400 molecules = 6000000 total steps
xedges = np.linspace(0, 200, 8, endpoint=True) #Split up into discrete distances along x-axis
yedges = np.linspace(0, 200, 8, endpoint=True) #Split up into discrete distances along y-axis
entropy_out = []
x_array = np.arange(N)

for i in range(N): #Go through the number of steps
    for j in range(num_atoms): #Loop through each molecule for each step
        r = random.random()
        if r < 0.25: #X go backward 1
            atoms[j, 0] -= 1
        elif r < 0.50: #X go forward 1
            atoms[j, 0] += 1
        elif r < 0.75: #Y go backward 1
            atoms[j, 1] -= 1
        else: #Y go forward 1
            atoms[j, 1] += 1
        x,y = (atoms[j, 0], atoms[j, 1]) #Get x and y position of this atom
        #Check boundaries and set the atom back
        if x == 200:
            atoms[j, 0] = 198
        elif x == 0:
            atoms[j, 0] = 2
        if y == 200:
            atoms[j, 1] = 198
        elif y == 0:
            atoms[j, 1] = 2
    entropy = 0.0 #Reset entropy for each step
    for yedge in range(len(yedges)-1): #Loop through yedge
        index = [a for a, value in enumerate(atoms[:,1]) if value < yedges[yedge+1] and value > yedges[yedge]] #Append index for values in yedge values
        for xedge in range(len(xedges)-1): #Loop through xedge
            count = 0.0 #Reset the count at each xedge because each indicates one square
            for k in index: #Loop through the values in the index in the yedges
                if atoms[k, 0] < xedges[xedge+1] and atoms[k, 0] > xedges[xedge]:
                    count += 1
            if count != 0.0: #If count is not zero
                temp = count/float(num_atoms)
                entropy -= temp*math.log(temp)
    entropy_out.append(entropy)

print(time.time()-start_time)
plt.figure()
plt.xlim(0, N)
plt.xlabel('Time (steps)')
plt.ylabel('Entropy')
plt.title('Entropy vs time')
plt.plot(x_array, entropy_out)
plt.show()
