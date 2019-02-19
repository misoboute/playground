from random import seed
from random import random
from random import uniform
import math

def InCircle(x, y, r, x0, y0):
   d = ((x - x0) ** 2  + (y - y0) ** 2) ** .5
   return d <= r

def solution(x1, y1, r1, x2, y2, r2):
   # write your code in Python 3.6
   d = ((x1 - x2) ** 2  + (y1 - y2) ** 2) ** .5
   if d > (r1+r2):
       return 0
   
   seed(1)

   if r1 <= r2:
       X = x1
       Y = y1
       R = r1

       X0 = x2
       Y0 = y2
       R0 = r2
   else:
       X = x2
       Y = y2
       R = r2

       X0 = x1
       Y0 = y1
       R0 = r1

   N = 2000000

   Xs = [(X-R) + (uniform(0,1) * 2 * R)  for i in range(N)];
   Ys = [(Y-R) + (uniform(0,1) * 2 * R)  for i in range(N)];

   count = 0
   countc = 0
   condition1 = [InCircle(X,Y,R, Xs[i], Ys[i]) for i in range(N)]
   condition2 = [InCircle(X0,Y0,R0, Xs[i], Ys[i]) for i in range(N)]
   condition3 = [condition1[i] and condition2[i] for i in range(N)]
   


   count = sum(condition1)
   countc = sum(condition3)

   area = (countc / count) * math.pi * R * R

   return area