import random
import numpy as np
import matplotlib.pyplot as plt
import pandas

#Brilli the ant is trying to get from point A to point B in a grid. The coordinates of
#point AA is (1,1) (this is top left corner), and the coordinates of point B is (n,n) (this is bottom right corner, n is the size of the grid).
#Once Brilli starts moving, there are four options, it can go right, left, up or down (no diagonal
#movement allowed) with the corresponding probability of 0.5, 0.3, 0.2 and 0.1, respectively. If
#any of these four options satisfy the following:
# The new point should still be within the boundaries of the n×n grid
# The new point should not be visited previously.
#the new point will be selected randomly among the remaining valid options.
#If P is the probability of Brilli the ant reaching point B for a 6x6 grid, use Monte Carlo simulation to compute P.
#Calculate P for 4x4, 6x6, 8x8, 10x10 grids to obtain a dependence trend of P on the grid size.

#Defining the lattice#
#The extra 2 columns and rows are to define the boundaries
#It will output an n+2 x n+2 matrix with the edges having a value of -1.0 to indicate that these are boundaries
#Removing the boundaries will yield an n x n matrix that will be the grid size we want to test
#The extra columns and rows are to simplify the code for testing the boundaries
#Places where the walker has been is indicated by positive integer values that start at 1
#Places that haven't been visited and are not boundaries have values of 0.0

def initialize(n): #Initialize a grid with size n+2 x n+2 but size of use is n x n
    lattice = np.zeros((n+2, n+2))
    lattice[:, 0] = -1.0
    lattice[:, -1] = -1.0
    lattice[0, :] = -1.0
    lattice[-1, :] = -1.0
    lattice[1, 1] = 1 #Starting position of the walker is at (1,1)
    return lattice

def SAW_walk(num_walkers, n):
    count = 0.0 #Reset at each calculation of n x n grid size; this counts how many walkers made it to the desired spot
    p_tot = 1.1 #The total probability of ecah decision: 0.5 + 0.3 + 0.2 + 0.1 = 1.1
    for walk in range(num_walkers): #Loop through the walk
        lattice = initialize(n) #Initialize the lattice at the start of each walk
        x = 1 #Get the x position of the initial walker
        y = 1 #Get the y position of the initial walker
        i = 1 #This is the integer value of the current position of the walker
        while lattice[n, n] == 0.0: #While the desired location (n, n) remains unoccupied, which is assigned a value of 0.0, keep looping
            options = [] #This array is to determine what directions the walker is able to go
            if lattice[int(x)-1, int(y)] == 0.0: #If the spot to the left of the walker is unoccupied, left is an option
                options.append('Left')
            if lattice[int(x)+1, int(y)] == 0.0: #If the spot to the right of the walker is unoccupied, right is an option
                options.append('Right')
            if lattice[int(x), int(y)-1] == 0.0: #If the spot above the walker is unoccupied, up is an option
                options.append('Up')
            if lattice[int(x), int(y)+1] == 0.0: #If the spot below the walker is unoccupied, down is an option
                options.append('Down')
            if options == []: #If there are no options, means the walker is trapped -- therefore, break the loop since it failed
                break
            i += 1 #Increase the integer value to indicate the next positions of the walker -- 1 means first position (starting), 2 means one step, 3 means two steps, etc...
            while True: #Keep looping until a decision is made out of the available options -- break will stop the this loop after a decision is made
                number = random.uniform(0, p_tot) #Generate a random number to determine the direction to walk
                if number < 0.5: #0.5 means right
                    if 'Right' in options: #Only move right if it is an option
                        lattice[int(x)+1, int(y)] = i #Assign the integer value to keep track of the path the walker takes
                        x += 1 #Increase the x value because the walker moved right one spot
                        break
                elif number < 0.8: #0.3 means left
                    if 'Left' in options: #Only move left if it is an option
                        lattice[int(x)-1, int(y)] = i
                        x -= 1
                        break
                elif number < 1.0: #0.2 means up
                    if 'Up' in options: #Only move up if it is an option
                        lattice[int(x), int(y)-1] = i
                        y -= 1
                        break
                else:
                    if 'Down' in options: #0.1 means down
                        lattice[int(x), int(y)+1] = i #Only move down if it is an option
                        y += 1
                        break
        #print(pandas.DataFrame(lattice)) #This line will print the path of each walk
        if lattice[n, n] != 0.0: #If the end goal (n, n) is reached (means this spot has an integer value), increase the count
            count += 1
    probability = float(count/num_walkers) #how many made it / number of walkers = probability of making it -- count/num_walkers
    return probability

grids = [4, 6, 8, 10] #Grid sizes to be tested
num_walkers = 5000 #Number of walkers
p_trend = [] #Array to append the probabilities at each grid size tested
for n in grids: #Loop through the values of the grid sizes
    probability = SAW_walk(num_walkers, n)
    p_trend.append(probability)

print(p_trend)
plt.plot(grids, p_trend)
plt.xlabel('Grid size')
plt.ylabel('Probability')
plt.show()
