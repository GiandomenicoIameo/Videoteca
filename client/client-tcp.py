from socket import *
import subprocess

def send( socket, message ):

    socket.send( message.encode() )
    print( 'Messaggio inviato!' )

    return socket.recv( 1024 )

def authentication( head ):

    username = input( 'Username: ' )
    password = input( 'Password: ')

    message = head + username + ' ' + password
    return message

def session( head, action ):

    if action == 0: # richiesta di noleggio
        filmname = input( 'Nome film: ' )
        number   = input( 'Inserisci la quantità: ' )
        date     = input( 'Inserisci data: ' )

        message = head + filmname + '\0' + number + '\0' + date + '\0'

    elif action == 1: # Rimuovi dal carrello
        filmname = input( 'Nome film: ' )
        number   = input( 'Inserisci la quantità: ' )

        path = "./remove.sh"
        result = subprocess.run( [ "bash", path, filmname, number ], capture_output = "True", text = "True" )
        print( result.stdout )

        message = head + filmname + '\0' + number + '\0'
    elif action == 2: # Aggiungi al carrello
        filmname = input( 'Nome film: ' )
        number   = input( 'Inserisci la quantità: ' )

        path = "./add.sh"
        result = subprocess.run( [ "bash", path, filmname, number ], capture_output = "True", text = "True" )
        print( result.stdout )

        message = head + filmname + '\0' + number + '\0'

    return message

def release( head, action ):

    if action == 1:
        return authentication( head )
    return head + ''

def search( head ):

    filmname = input( 'Nome film: ' )
    message = head + filmname
    return message

def request():

    method = input( 'Inserisci il tipo di richiesta: ' )
    method = int( method )

    if method == 3:
        action = 0
    else:
        action = input( 'Inserisci il tipo di azione: ' )
        action = int( action )

    return method, action

server_name = 'localhost'
server_port = 8080

client_socket = socket( AF_INET, SOCK_STREAM )
client_socket.connect( ( server_name, server_port ) )

while True:

    method, action = request()
    head = str( method ) + str( action )

    if method == 0:
        message = authentication( head )
    elif method == 1:
        message = session( head, action )
    elif method == 2:
        message = release( head, action )
        if action == 0 or action == 1:
            break
    elif method == 3:
        message = search( head )
    else:
        message = head + body
        break

    receive_message = send( client_socket, message )
    print( 'Messaggio ricevuto dal server:', receive_message.decode() )

receive_message = send( client_socket, message )
print( 'Messaggio ricevuto dal server:', receive_message.decode() )

client_socket.close()
print( 'Connessione terminata' )
