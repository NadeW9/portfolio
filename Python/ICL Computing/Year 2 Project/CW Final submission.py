# In this section I am importing all the libraries I will need -------------------------------
import numpy as np
import matplotlib.pyplot as pl
from matplotlib import cm
import math as mt
import time

# In this section I am setting the domain of solution and the discretised grid ---------------
c = 1/np.sqrt(2)  # constant c in wave eqn

### DOMAIN HANDLING
# x axis
lowX = 0
uppX = 1 
dx = 0.02 # 1x: 0.2 | 10x finer: 0.02 | 50x finer: 0.004
x = np.arange(lowX, uppX+dx, dx) # x grid points
Nx = len(x)

# y axis
lowY = 0
uppY = 1
dy = 0.02 # 1x: 0.2 | 10x finer: 0.02 | 50x finer: 0.004
y = np.arange(lowY, uppY+dy, dy) # y grid points
Ny = len(y)

# time
t_end = 20 # duration span
dt = 0.5*(1/c) *(1/np.sqrt(dx**(-2) + dy**(-2))) # stability condition
t = np.arange(0,t_end+dt,dt)
Nt = len(t)

# create meshgrid for reference
X, Y = np.meshgrid(x, y)

# coefficient expressions
C1 = (c**2)*(dt**2/dx**2)
C2 = (c**2)*(dt**2/dy**2)

# In this section I am defining arrays I would need (if needed) ---------------------------------

# In this section I am setting the boundary conditions/initial values ---------------------------
### THE PHYSICS for 2D sine wave
# initial conditions for 2D sine wave
def initial_cond(x,y):
    return np.sin(2*np.pi*x + 2*np.pi*y)

# 1st initial condition
U = initial_cond(X,Y)
# retrieve values for later
B1 = U[:,0]
B2 = U[:,-1]
B3 = U[0,:]
B4 = U[-1,:]

# 2nd initial condition
U1 = np.zeros((Nx,Ny))
U1[1:-1,1:-1] = (U[1:-1, 1:-1]
                 + (C1/2)*(U[0:-2,1:-1] - 2*U[1:-1,1:-1] + U[2:,1:-1])
                 + (C2/2)*(U[1:-1,0:-2] - 2*U[1:-1,1:-1] + U[1:-1, 2:]))

# reinforce surface boundary conditions
U1[:,0] = B1
U1[:,-1] = B2
U1[0,:] = B3
U1[-1,:] = B4

# grab new conditions again
B5 = U1[:,0]
B6 = U1[:,-1]
B7 = U1[0,:]
B8 = U1[-1,:]

# In this section I am implementing the numerical method ----------------------------------------
### SOLUTION
# leading value to be found
U2 = np.zeros((Nx,Ny))
# results matrix to contain all values
U_main = np.zeros((Nt,Nx,Ny))
# dump the first 2 conditions found
U_main[0,:,:] = U
U_main[1,:,:] = U1

for n in range (2, Nt): # t range is also u range
    # discretised equation
    U2[1:-1,1:-1] = (2*U1[1:-1,1:-1] - U[1:-1,1:-1]
                     + C1*(U1[0:-2,1:-1] - 2*U1[1:-1,1:-1] + U1[2:, 1:-1])
                     + C2*(U1[1:-1,0:-2] - 2*U1[1:-1,1:-1] + U1[1:-1, 2:]))
    
    # Direchlet boundary conditions
    U2[:,0] = B5
    U2[:,-1] = B6
    U2[0,:] = B7
    U2[-1,:] = B8

    U1[:,0] = B5
    U1[:,-1] = B6
    U1[0,:] = B7
    U1[-1,:] = B8

    U[:,0] = B1
    U[:,-1] = B2
    U[0,:] = B3
    U[-1,:] = B4
    
    # update result matrix
    U_main[n,:,:] = U2
    
    # update trailing wave values
    U = U1
    U1 = U2

### PLOTTING -- for individual time points
fig = pl.figure()
fig.set_figheight(12)
fig.set_figwidth(15)
ax = fig.add_subplot(2,2,1)
print(Nt) # check number of time steps
timept = 100 # change this value to plot wave at diff time points

# 2D heatmap
cm = pl.get_cmap("jet")
im = ax.pcolor(X, Y, U_main[timept,:,:], cmap=cm, vmin=np.min(U_main[:,:,:]), vmax=np.max(U_main[:,:,:]))
pl.colorbar(im, ax=ax)

# 3D coloured surface
ax = fig.add_subplot(2, 2, 2, projection='3d')
ax.plot_surface(X,Y,U_main[timept,:,:], cmap=cm)
ax.set_title('2D Sine Wave Surface')

# 3D black and white contour
ax = fig.add_subplot(2, 2, 3, projection='3d')
ax.contour3D(X, Y, U_main[timept,:,:], 50, cmap='binary')
ax.set_title('2D Sine Wave Contour')

# 3D scatter
ax = fig.add_subplot(2, 2, 4, projection='3d')
ax.scatter3D(X, Y, U_main[timept,:,:])
ax.set_title('2D Sine Wave Scatter')

# axis labels
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

pl.show()

# In this section I am celebrating
print('CW done: I deserve a good mark')
