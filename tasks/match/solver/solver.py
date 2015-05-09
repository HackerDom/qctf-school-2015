import operator
from decimal import *

getcontext().prec = 80
num = Decimal("0.3190518599914394933332201305924026187429073103280909299815648605402885")        
odict = [('Q', Decimal('0.075')), (' ', Decimal('0.05')), ('1', Decimal('0.05')), ('3', Decimal('0.05')), ('A', Decimal('0.05')), ('F', Decimal('0.05')), ('G', Decimal('0.05')), ('I', Decimal('0.05')), ('M', Decimal('0.05')), ('N', Decimal('0.05')), ('O', Decimal('0.05')), ('S', Decimal('0.05')), ('X', Decimal('0.05')), ('4', Decimal('0.025')), ('5', Decimal('0.025')), ('6', Decimal('0.025')), ('9', Decimal('0.025')), ('B', Decimal('0.025')), ('C', Decimal('0.025')), ('D', Decimal('0.025')), ('K', Decimal('0.025')), ('L', Decimal('0.025')), ('T', Decimal('0.025')), ('V', Decimal('0.025')), ('Y', Decimal('0.025')), ('Z', Decimal('0.025'))]
count = len(odict)

left = {}
right = {}

answer = ""     

tmp = Decimal(0)
for i in range(count):
	left[odict[i][0]] = tmp 
	tmp += Decimal(odict[i][1])
	right[odict[i][0]] = tmp	

for k in range(40):
	for i in range(count):
		if (num >= left[odict[i][0]]) and (num < right[odict[i][0]]):
			answer += odict[i][0]
			l = right[odict[i][0]] - left[odict[i][0]] 
			num = (num - left[odict[i][0]]) / l
			break
                                                                     
print(answer)