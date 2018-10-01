import matplotlib.pyplot as plt
import numpy as np
import numpy.polynomial.polynomial as poly

filename="../run.log"
#filename="../../ProjectThingOutput/runs/run_007"

responses=[]

with open(filename,'r') as f:

    
    for line in f:

        line = line.strip()

        if not line:
            continue

        
        responses.append(float(line))


r1 = responses[0::8]
r2 = responses[1::8]
r3 = responses[2::8]
r4 = responses[3::8]
r5 = responses[4::8]
r6 = responses[5::8]
r7 = responses[6::8]
r8 = responses[7::8]

plt.plot(r1)
plt.plot(r2)
plt.plot(r3)
plt.plot(r4)

plt.xlabel("Run")
plt.ylabel("Output")
plt.show()

plt.plot(r5)
plt.plot(r6)
plt.plot(r7)
plt.plot(r8)
plt.show()

r5_coefs = poly.polyfit(range(100),r5,4)
r6_coefs = poly.polyfit(range(100),r6,4)
r7_coefs = poly.polyfit(range(100),r7,4)
r8_coefs = poly.polyfit(range(100),r8,4)
r5_fit = poly.polyval(range(100),r5_coefs)
r6_fit = poly.polyval(range(100),r6_coefs)
r7_fit = poly.polyval(range(100),r7_coefs)
r8_fit = poly.polyval(range(100),r8_coefs)
plt.plot(range(100),r5)
plt.plot(range(100),r8)
plt.plot(range(100),r5_fit)
plt.plot(range(100),r8_fit)
plt.plot(range(100),[50.0 for x in range(100)],'.')

plt.show()
