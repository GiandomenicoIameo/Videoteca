from socket import *
#import customtkinter as tk

#print( tk.__version__ )

server_name = 'localhost'
server_port = 8080

client_socket = socket( AF_INET, SOCK_STREAM )
client_socket.connect( ( server_name, server_port ) )

client_socket.close()
