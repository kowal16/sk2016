import xmlrpclib

imie = raw_input("Podaj imie: ")
proxy = xmlrpclib.ServerProxy("http://localhost:8008/")
#print "czy 3 jest parzyste: %s" % str(proxy.podzielna(3, 2))
print "Jaki czas: %s" % str(proxy.czas())
print "%s" % str(proxy.przywitaj(imie))
