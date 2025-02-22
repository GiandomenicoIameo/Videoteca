from socket import *

server_name = 'localhost'
server_port = 8080

client_socket = socket( AF_INET, SOCK_STREAM )
client_socket.connect( ( server_name, server_port ) )

method = input( 'Inserisci il tipo di richiesta: ' )

head = method
body = ""

method = int( method )

if method == 3:
    film = input( 'Nome film: ')
    body = film

elif method == 1:
    username = input( 'Username: ' )
    password = input( 'Password: ')
    body = username + ' ' + password

message = head + body
client_socket.send( message.encode() )

print( 'Messaggio inviato!' )

receive_message = client_socket.recv( 1024 )
print( 'Messaggio ricevuto dal server:', receive_message.decode() )

client_socket.close()
print( 'Connessione terminata' )
