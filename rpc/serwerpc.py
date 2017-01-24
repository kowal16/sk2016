import datetime  # informacje o czasie
import SimpleXMLRPCServer as xserv

def czas():
	return str(datetime.datetime.now())

def przywitaj(imie):
	return "Witaj " + imie + "!"


server = xserv.SimpleXMLRPCServer(("localhost", 8008))
#server.register_function(podzielna, "podzielna")
server.register_function(czas, "czas")
server.register_function(przywitaj, "przywitaj")
server.serve_forever()
