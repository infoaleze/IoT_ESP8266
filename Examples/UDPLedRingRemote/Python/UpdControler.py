import socket
import struct

from tkinter import *

def test(couleur):
    try:
        ch.config(bg=couleur)
    except:
        return False
    else:
        return True
    



def envoieA(event=None):
    msg='A#'+ch.get()
    ch.delete(0, END)
    if msg=='A#':
        msg='A#FFFFFF'
    if test(msg[1:]):
        send(msg)


def envoieP(event=None):
    msg='P#'+ch.get()
    ch.delete(0, END)
    if msg=='P#':
        msg='P#FFFFFF'
    if test(msg[1:]):
        send(msg)

    
def envoie(event=None):
    msg=ch.get()
    ch.delete(0, END)
    if test(msg[1:]):
        send(msg)

def send(msg):
    sock.sendto(bytes(msg, "ASCII"), (MULTICATS_GROUPIP, MUTLICATS_PORT))
    print("Message envoyé: "+msg)


MULTICATS_GROUPIP = "239.192.0.100"
MUTLICATS_PORT    = 4242

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

sock.bind(('', MUTLICATS_PORT))

mreq = struct.pack("=4sl", socket.inet_aton(MULTICATS_GROUPIP), socket.INADDR_ANY)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

fen=Tk()
fen.title("Envoyeur de messages à l'objet connecté")
ch=Entry(fen)
ch.pack()
ch.bind("<Return>", envoie)
Button(fen, text='A', command=envoieA).pack(side=LEFT)
Button(fen, text='P', command=envoieP).pack(side=RIGHT)
Button(fen, text='RESET', command=lambda: send('R')).pack()
fen.mainloop()
