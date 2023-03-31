import numpy as np

class Encryption:
    def __init__(self):
        temp = "3 0 1 1 1 2 5 0 1 1 3 0 2 0 1 1"
        tempinv = "1 0 0 -1 2 3 -5 -2 -1 -1 2 1 -1 1 -2 2"
        self.A = np.array([int(num) for num in temp.split()]).reshape((4,4))
        self.Ainv = np.array([int(num) for num in tempinv.split()]).reshape((4,4))
        self.CRCG = "10011"
    
    def encrypt(self,data):
        n = len(data)
        pad = 4 - n%4
        pad = pad%4
        data = data + ' '*pad
        n = len(data)
        data = [ord(i) for i in data]
        databits = ''.join(format(i,'08b') for i in data)
        crc_code = self.getcrc(databits,self.CRCG)
        data = np.array(data).reshape((4,n//4))
        cipher_msg = self.A@data
        data = ','.join(map(str,cipher_msg.reshape((1,n))[0]))
        data+= ','+crc_code
        return data
    
    def decrypt(self,data):
        data = data.split(',')
        crc_part = data[-1]
        data = data[:-1]
        n = len(data)
        data = [int(i) for i in data]
        data = np.array(data).reshape((4,n//4))
        orig_text = (self.Ainv@data).reshape((1,n))[0]
        databits = ''.join(format(i,'08b') for i in orig_text)
        crc_code = self.getcrc(databits,self.CRCG,crc_part)
        orig_text = ''.join(map(chr,orig_text))
        return [orig_text,crc_code=="0000"]




    def getcrc(self,databits, gen, crc_code='0000'):
        databits+=crc_code
        msg = [int(bit) for bit in databits]
        crc_g = [int(bit) for bit in gen]
        n_msg = len(msg)
        n_g = len(crc_g)
        n = n_msg - n_g
        for i in range(n):
            if msg[i] == 1:
                for j in range(n_g):
                    msg[i + j] = msg[i + j] ^ crc_g[j]
        return ''.join(map(str,msg[-len(crc_code):]))


