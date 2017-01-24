import socket    # gniazda
import datetime  # informacje o czasie

ip = ""  # puste ip znaczy "dowolne" (=INADDR_ANY)
port = 7878

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind( (ip, port) )  # jeden parametr (tupla)!

while True:
    data, addr = sock.recvfrom(1024)  # bufor na 1024 bajty
    if data == "czas":
        czas = str(datetime.datetime.now())
        sock.sendto(czas, addr)  # dwa argumenty
