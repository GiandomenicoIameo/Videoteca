from socket import *
import webview
import os
import gzip
import subprocess
import signal
import sys

os.environ[ "QTWEBENGINE_CHROMIUM_FLAGS" ] = "--no-sandbox"

class WebApp:

    address   = None
    port      = None
    filename  = None
    client    = None

    def __init__( self, address, port, filename ):

        self.address   = address
        self.port      = port
        self.filename  = filename

        self.client = socket( AF_INET, SOCK_STREAM )
        self.client.connect( ( self.address, self.port ) )

    def authentication( self, data ):

        method = 0

        head = str( int( method ) ) + str( data[ 'action' ] )
        message = head + data[ 'username' ] + ' ' + data[ 'password' ]

        self.client.send( message.encode() )
        response = self.client.recv( 1024 )

        if not response:
            print( "Server disconnesso!" )
            self.quit()

        result = response.decode().split( '\n' )
        if data[ 'action' ] == 0:
            res = self.signin( result[ 1 ] )

        elif data[ 'action' ] == 1:
            res = self.signup( result[ 1 ] )

        return res

    def signin( self, response ):

        if response == "Accesso negato!":
            return 0
        elif response == "Connesso!":
            return 1
        else:
            return 2

    def signup( self, response ):

        if response == "Username già in uso!":
            return 0
        elif response == "Account creato!":
            return 1

    def session( self, data ):

        method = 1
        head = str( int( method ) ) + str( data[ 'action' ] )

        if data[ 'action' ] == 1:
            return self.toCart( head, data )

        elif data[ 'action' ] == 2:
            return self.returnMovie( head, data )

        elif data[ 'action' ] == 3:
            return self.checkout( head )

        elif data[ 'action' ] == 4:
            return self.showcart( head )

        elif data[ 'action' ] == 5:
            return self.showrented( head )

    def toCart( self, head, data ):

        message = (head + data[ 'filmname' ] + '\0'
                       + data[ 'number' ] + '\0'
                       + data[ 'date' ] + '\0')

        self.client.send( message.encode() )
        response = self.client.recv( 1024 )

        if not response:
            print( "Server disconnesso!" )
            self.quit()

        result = response.decode().split( '\n' )
        return result[ 1 ]

    def returnMovie( self, head, data ):

        message = (head + data[ 'filmname' ] + '\0'
                         + data[ 'number' ] + '\0'
                         + data[ 'date' ] + '\0')

        self.client.send( message.encode() )
        response = self.client.recv( 1024 )

        if not response:
            print( "Server disconnesso!" )
            self.quit()

        result = response.decode().split( '\n' )
        return result[ 1 ]

    def checkout( self, head ):

        message = head + ''

        self.client.send( message.encode() )
        response = self.client.recv( 1024 )

        if not response:
            print( "Server disconnesso!" )
            self.quit()

        result = response.decode().split( '\n' )
        return result[ 1 ]

    def showcart( self, head ):

        message = head + ''

        self.client.send( message.encode() )
        response = self.client.recv( 1024 )

        if not response:
            print( "Server disconnesso!" )
            self.quit()

        result = response.decode( 'utf-8' ).split( '\n' )
        compressed_data = bytes.fromhex( result[ 1 ] )

        try:
            decompressed_data = gzip.decompress( compressed_data )
            return decompressed_data.decode( 'utf-8' )
        except Exception as e:
            print( f"Errore durante la decompressione: {e}" )

    def showrented( self, head ):

        message = head + ''

        self.client.send( message.encode() )
        response = self.client.recv( 1024 )

        if not response:
            print( "Server disconnesso!" )
            self.quit()

        result = response.decode( 'utf-8' ).split( '\n' )
        compressed_data = bytes.fromhex( result[ 1 ] )

        try:
            decompressed_data = gzip.decompress( compressed_data )
            return decompressed_data.decode( 'utf-8' )
        except Exception as e:
            print( f"Errore durante la decompressione: {e}" )

    def release( self, data ):

        method = 2

        head = str( int( method ) ) + str( data )
        message = head + ''

        self.client.send( message.encode() )
        self.client.recv( 1024 ) # Questa istruzione è essenziale
                                 # per svuotare il buffer.

        # Si assume che non ci siano stati errori durante il percorso, quindi
        # il client si disinteressa della risposta del server. Il client presume che l'account
        # sia stato disconnesso oppure cancellato

    def view( self ):

        method = 3
        action = 0

        head = str( int( method ) ) + str( int( action ) )
        message = head + ''

        self.client.send( message.encode() )
        response = self.client.recv( 1024 )

        if not response:
            print( "Server disconnesso!" )
            self.quit()

        result = response.decode( 'utf-8' ).split( '\n' )
        compressed_data = bytes.fromhex( result[ 1 ] )

        try:
            decompressed_data = gzip.decompress( compressed_data )
            return decompressed_data.decode( 'utf-8' )
        except Exception as e:
                print( f"Errore durante la decompressione: {e}" )

    def quit( self ):

        webview.windows[ 0 ].destroy()

        self.client.close()
        sys.exit()

    def start_gui( self ):

        html_file_path = os.path.join( os.path.dirname( __file__ ), 'template', self.filename )

        with open( html_file_path, 'r' ) as file:
            html_content = file.read()

        subprocess.Popen(["python3", "-m", "http.server", "8000"])
        percorso = f"http://localhost:8000/template/{os.path.basename(html_file_path)}"
        window = webview.create_window( "Block Buster", url=percorso, js_api=self )
        window.fullscreen = True
        webview.start( gui='qt', args=['--no-sandbox'] )

def handler( signum, frame ):

    print( "\nConnessione terminata!" )
    sys.exit( 0 )

if __name__ == "__main__":

    signal.signal( signal.SIGINT, handler )
    signal.signal( signal.SIGTERM, handler )
    # 172.17.0.2
    app = WebApp( 'localhost', 8080, 'index.html' )
    app.start_gui()
