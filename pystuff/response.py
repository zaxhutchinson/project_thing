import matplotlib.pyplot as plt

filename="../run.log"
#filename="../../ProjectThingOutput/runs/run_007"

responses=[]

with open(filename,'r') as f:

    
    for line in f:

        line = line.strip()

        if not line:
            continue

        
        responses.append(float(line))


r1 = responses[0::4]
r2 = responses[1::4]
r3 = responses[2::4]
r4 = responses[3::4]

plt.plot(r1)
plt.plot(r2)
plt.plot(r3)
plt.plot(r4)

plt.show()
