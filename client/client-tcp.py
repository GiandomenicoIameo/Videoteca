from socket import *
#import customtkinter as tk

#print( tk.__version__ )

server_name = 'cclient'
server_port = 8080

client_socket = socket( AF_INET, SOCK_STREAM )
client_socket.connect( ( server_name, server_port ) )

send_message = input( 'Componi messaggio: ' )
client_socket.send( send_message.encode() )

print( 'Messaggio inviato!' )

receive_message = client_socket.recv( 1024 )
print( 'Messaggio ricevuto dal server:', receive_message.decode() )

client_socket.close()
print( 'Connessione terminata' )
