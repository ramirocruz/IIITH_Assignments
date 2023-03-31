# from ftplib import FTP
# home = ''
# ftp = FTP('')
# ftp.connect('localhost',2021)
# ftp.login()
# ftp.cwd(home) #replace with your directory
# ftp.retrlines('LIST')

# def uploadFile():
#    filename = 'testfile.txt' #replace with your file in your home folder
#    ftp.storbinary('STOR '+filename, open(filename, 'rb'))
#    ftp.quit()

# def downloadFile():
#    filename = 'testfile.txt' #replace with your file in the directory ('directory_name')
#    localfile = open(filename, 'wb')
#    ftp.retrbinary('RETR ' + filename, localfile.write, 1024)
#    ftp.quit()
#    localfile.close()

# uploadFile()
#downloadFile()

from rpclib import RPC
rpc = RPC()
function_list = rpc.get_function_list()
for key in function_list:
    print(function_list[key])