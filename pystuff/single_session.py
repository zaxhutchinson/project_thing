import os
import struct
import sys
import numpy as np
import matplotlib.pyplot as plt

import spiketrain

home=os.path.expanduser('~')
plots=home+"/Projects/ProjectThingOutput/plots/"
recordings=home+"/Projects/ProjectThingOutput/recordings/"
directory="test_003"

spiketrains={}

test_id=0
tests=10
outputs=4
length=5000
start=0

for dirpath,dirs,files in os.walk(recordings+directory):
    cur_dir=dirpath.split('/')[-2] + "_" + dirpath.split('/')[-1]

    if len(files)==0:
        continue

    #print(cur_dir,files)

    spiketrains[cur_dir]=[]

    for f in files:
        st = spiketrain.Spiketrain(f)
        with open(os.path.join(dirpath,f),"rb") as bfile:
            while(True):
                data=bfile.read(8)
                if(data):
                    st.addspike(struct.unpack('@l',data)[0])
                else:
                    break
        spiketrains[cur_dir].append(st)



sdata=np.zeros([tests,outputs,length])

for k,v in spiketrains.items():

    stsp=k.split('_')
    print stsp
    if(test_id != int(stsp[1])):
        continue

    test_it=int(stsp[2])
    
    for st in v:
        if len(st.spikes)==0:
            continue
        for s in st.spikes:
            if s >= length:
                break;
            if s >= start:
                sdata[test_it][int(st.index)][s-start]+=1

for t in range(tests):
    plt.imshow(sdata[t],interpolation='nearest',aspect='auto')
    plt.show()
