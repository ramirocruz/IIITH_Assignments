from Crypto.Cipher import DES3
from Crypto import Random
import hashlib

class GenerateKey:
    diffi_prime = 2410312426921032588552076022197566074856950548502459942654116941958108831682612228890093858261341614673227141477904012196503648957050582631942730706805009223062734745341073406696246014589361659774041027169249453200378729434170325843778659198143763193776859869524088940195577346119843545301547043747207749969763750084308926339295559968882457872412993810129130294592999947926365264059284647209730384947211681434464714438488520940127459844288859336526896320919633919
    diffi_gen = 2
    def __init__(self,rollno):
        pkey = str(Random.get_random_bytes(4)) + rollno
        self.pkey = int(hashlib.sha256(pkey.encode()).hexdigest(),16)
        self.hashkey = pow(self.diffi_gen,self.pkey,self.diffi_prime)

    def gen_key(self,rkey):
        self.finalkey = pow(rkey,self.pkey,self.diffi_prime)
        return self.finalkey

class Encryption:
    
    def __init__(self,key):
        self.key = bytearray.fromhex(hashlib.sha256(str(key).encode()).hexdigest()[:48])

    def encrypt(self,plain_text):
        pad = 8 - len(plain_text)%8
        pad = pad%8
        plain_text = plain_text + b' '*pad
        des3 = DES3.new(self.key,DES3.MODE_ECB)
        return des3.encrypt(plain_text)
    
    def decrypt(self,cipher_text):
        des3 = DES3.new(self.key,DES3.MODE_ECB)
        cipher =  des3.decrypt(cipher_text)
        return cipher


