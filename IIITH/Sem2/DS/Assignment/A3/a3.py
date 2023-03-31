class Node:
    def __init__(self,order):
        self.order = order
        self.keys=[]
        self.counts=[]
        self.childs=[]
        self.next = None
        self.isleaf = True      
    
    def __insert__(self,key):
        if not self.keys:
            self.keys.append(key)
            self.counts.append(1)
            return
        for  i in range(len(keys)):
            if(key == keys[i]):
                self.counts[i] += 1
                return
            if(key > keys[i]):
                continue

            self.keys = self.keys[:i] + [key] + self.keys[i:]
            self.counts = self.counts[:i] + [1] + self.counts[i:]
            return
        self.keys.append(key)
        self.counts.append(1)
    

        