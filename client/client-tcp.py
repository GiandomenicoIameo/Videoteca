from socket import *
import subprocess
import sys
import gzip
import io
import signal

from PyQt6.QtWidgets import QApplication, QMainWindow, QLineEdit
from PyQt6.QtGui import QIcon, QAction
from PyQt6 import QtCore
from gui.access import Ui_MainWindow

class MainWindow( QMainWindow ):

    def __init__( self, action ):
        super( MainWindow, self ).__init__()


        self.username = None
        self.password = None

        self.ui = Ui_MainWindow()
        self.ui.setupUi( self )

        _translate = QtCore.QCoreApplication.translate

        self.ui.label.setText( _translate( "MainWindow", action + " to GetMovie" ) )
        self.ui.pushButton.setText( _translate( "MainWindow", action ) )

        self.ui.pushButton.clicked.connect( self.get )

    def get( self ):
        if self.ui.lineEdit:
            self.username = self.ui.lineEdit.text()

        if self.ui.lineEdit_2:
            self.password = self.ui.lineEdit_2.text()

        self.close()

def authentication( head, action ):

    if action == 0:

        app = QApplication( sys.argv )

        window = MainWindow( "Sign in" )
        window.show()

        app.exec()

        username = window.username
        password = window.password

        message = head + username + ' ' + password

    elif action == 1:

        app = QApplication( sys.argv )

        window = MainWindow( "Sign up" )
        window.show()

        app.exec()

        username = window.username
        password = window.password

        message = head + username + ' ' + password
    else:
        message = head + ''

    return message

def session( head, action ):

    if action == 0: # richiesta di noleggio

        filmname = input( 'Nome film: ' )
        number   = input( 'Inserisci la quantità: ' )
        date     = input( 'Inserisci data: ' )

        message = head + filmname + '\0' + number + '\0' + date + '\0'

    elif action == 1: # Aggiornamento al carrello

        filmname = input( 'Nome film: ' )
        number   = input( 'Inserisci quantità: ' )
        date     = input( 'Inserisci data: ' )

        message = head + filmname + '\0' + number + '\0' + date + '\0'

    elif action == 2: # Richiesta di restituzione

        filmname = input( 'Nome film: ' )
        number   = input( 'Inserisci quantità: ' )
        date     = input( 'Inserisci data: ' )

        message = head + filmname + '\0' + number + '\0' + date + '\0'

    elif action == 3: # Richiesta di checkout

        message = head + ''

    elif action == 4 or action == 5: # Visualizzazione

        message = head + ''

    return message

def release( head, action ):

    if action == 0 or action == 1:
        return head + ''

def search( head ):

    filmname = input( 'Nome film: ' )
    message = head + filmname

    return message

def make_request():

    method = input( 'Inserisci il tipo di richiesta: ' )
    method = int( method )

    if method == 3:
        action = 0
    else:
        action = input( 'Inserisci il tipo di azione: ' )
        action = int( action )

    return method, action

def handler( signum, frame ):

    print( "\nConnessione terminata!" )
    sys.exit( 0 )


if __name__ == '__main__':

    server_name = 'localhost'
    server_port = 8080

    client_socket = socket( AF_INET, SOCK_STREAM )
    client_socket.connect( ( server_name, server_port ) )

    while True:

        signal.signal( signal.SIGINT, handler )
        signal.signal( signal.SIGTERM, handler )

        method, action = make_request()
        head = str( method ) + str( action )

        if method == 0:
            message = authentication( head, action )

        elif method == 1:
            message = session( head, action )

        elif method == 2:
            message = release( head, action )
            break

        elif method == 3:
            message = search( head )

        else:
            message = head + body
            break

        client_socket.send( message.encode() )
        response = client_socket.recv( 1024 )

        if method == 1 and ( action == 4 or action == 5 ):
            print( "" )
            compressed_data = bytes.fromhex( response.decode( 'utf-8' ) )

            try:
                decompressed_data = gzip.decompress( compressed_data )
                print( decompressed_data.decode( 'utf-8' ) )

            except Exception as e:
                print( f"Errore durante la decompressione: {e}" )

        else:
            if not response:
                print( 'Connessione chiusa dal server!' )
                sys.exit( 0 )

        print( 'Messaggio ricevuto dal server:', response.decode() )

    client_socket.send( message.encode() )
    response = client_socket.recv( 1024 )

    print( 'Messaggio ricevuto dal server:', response.decode() )
    client_socket.close()
    print( 'Connessione terminata' )
