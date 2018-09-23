class Spiketrain:
    def __init__(self,filename):
        self.spikes=[]

        sp=filename.split('(')[0]
        self.name=sp[0]
        sp=filename.split('(')[1].split(')')[0]
        self.index=sp[0]

    def addspike(self,time):
        self.spikes.append(time)
        
