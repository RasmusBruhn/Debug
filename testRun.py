import numpy as np
import subprocess as sp

Count = 1000
Vals = np.empty(Count)

for i in range(Count):
    String = sp.check_output("./Debug_tb_session", shell = True)
    Vals[i] = (String[-5:-1] == b'Done')
    
Unique, Counts = np.unique(Vals, return_counts = True)

print(Unique, Counts)