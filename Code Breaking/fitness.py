from math import log10 as log

class fitness(object):
    def __init__(self,ngramfile,sep=' '):
        self.ngrams = {}

        for line in open(ngramfile):
            key,count = line.split(sep) 
            self.ngrams[key] = float(count)

        self.L = len(key)
        self.N = sum(self.ngrams.values())

        #calculate log probabilities
        for key in self.ngrams:
            self.ngrams[key] = log(self.ngrams[key]/self.N)

        self.floor = log(0.01/self.N)

    def score(self,text):
        score = 0

        score = sum(self.ngrams.get(text[i:i+self.L],self.floor) for i in range(len(text)-self.L+1))
    
        return score
       