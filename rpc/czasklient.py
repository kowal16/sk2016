import socket    # gniazda

ip = "127.0.0.1"
port = 7878
msg = "czas"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.sendto(msg, (ip, port))  # dwa argumenty
data, addr = sock.recvfrom(1024)
print "odebralem", data
