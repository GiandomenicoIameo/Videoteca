from socket import *

server_name = 'localhost'
server_port = 8080

client_socket = socket( AF_INET, SOCK_STREAM )
client_socket.connect( ( server_name, server_port ) )

method = input( 'Inserisci il tipo di richiesta: ' )
action = input( 'Inserisci il tipo di azione: ' )

method = int( method )
action = int( action )

if method == 0:
    if action == 0 or action == 1:
        username = input( 'Username: ' )
        password = input( 'Password: ')

        body = username + ' ' + password
        message = str( method ) + str( action ) + body
elif method == 2:
    if action == 1:
        username = input( 'Username: ' )
        password = input( 'Password: ')

        body = username + ' ' + password
        message = str( method ) + str( action ) + body
    else:
        message = str( method ) + str( action )

elif method == 3:
    film = input( 'Nome film: ' )
    message = str( method ) + str( action ) + film

client_socket.send( message.encode() )

print( 'Messaggio inviato!' )

receive_message = client_socket.recv( 1024 )
print( 'Messaggio ricevuto dal server:', receive_message.decode() )

client_socket.close()
print( 'Connessione terminata' )
