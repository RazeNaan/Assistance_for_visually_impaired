import os
f = open('1.txt', 'r')
for a in f:
    if int(a) == 1:
        os.system("lxterminal -e python servo.py")
        
