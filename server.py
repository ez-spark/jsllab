
#!/usr/bin/env python
from http.server import BaseHTTPRequestHandler, HTTPServer
from os import curdir, sep
import os
from io import StringIO
import urllib
import cgi
import sys
import mimetypes
import ssl

class HTTPServer_RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        cwd = os.getcwd()
        if self.path=="/":
            self.path=cwd+'/index.html'
        else:
            self.path = cwd+self.path
        
        try:
            print(self.path.endswith(".html"))
            sendReply = False
            if self.path.endswith(".html"):
                mimetype='text/html'
                sendReply = True
            elif self.path.endswith(".jpg"):
                mimetype='image/x-icon'
                sendReply = True
            elif self.path.endswith(".ico"):
                mimetype='image/jpg'
                sendReply = True
            elif self.path.endswith(".mem"):
                mimetype='image/x-tga'
                sendReply = True
            elif self.path.endswith(".gif"):
                mimetype='image/gif'
                sendReply = True
            elif self.path.endswith(".js"):
                mimetype='application/javascript'
                sendReply = True
            elif self.path.endswith(".wasm"):
                mimetype='application/wasm'
                sendReply = True              
            elif self.path.endswith(".css"):
                mimetype='text/css'
                sendReply = True
            elif self.path.endswith(".data"):
                mimetype='text/data'
                sendReply = True

            else:
                mimetype='inode/directory'

            if sendReply == True:
                if mimetype != 'application/wasm' and mimetype != 'image/x-tga' and mimetype != 'text/data':
                    f = open(self.path)
                else:
                    f = open(self.path,'rb')
                   
                self.send_response(200)
                
                self.send_header('Access-Control-Allow-Origin', '*');
                self.send_header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content, Accept, Content-Type, Authorization');
                self.send_header('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, PATCH, OPTIONS');
                self.send_header('Cross-origin-Embedder-Policy', 'require-corp');
                #self.send_header('content-security-policy', 'upgrade-insecure-requests');
                self.send_header('Cross-origin-Opener-Policy','same-origin');
                self.send_header('Content-type',mimetype)

                self.end_headers()
                if mimetype != 'application/wasm' and mimetype != 'image/x-tga' and mimetype != 'text/data':
                    self.wfile.write(bytes(f.read(),"utf-8"))
                else:
                    self.wfile.write(f.read())
                f.close()
                
            elif mimetype == 'inode/directory':
                self.send_header('Access-Control-Allow-Origin', '*');
                self.send_header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content, Accept, Content-Type, Authorization');
                self.send_header('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, PATCH, OPTIONS');
                self.send_header('Cross-origin-Embedder-Policy', 'require-corp');
                #self.send_header('content-security-policy', 'upgrade-insecure-requests');
                self.send_header('Cross-origin-Opener-Policy','same-origin');
                self.wfile.write(bytes(self.list_directory(self.path),"utf-8"))
            return
                


        except IOError:
            self.send_header('Access-Control-Allow-Origin', '*');
            self.send_header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content, Accept, Content-Type, Authorization');
            self.send_header('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, PATCH, OPTIONS');
            self.send_header('Cross-origin-Embedder-Policy', 'require-corp');
            #self.send_header('content-security-policy', 'upgrade-insecure-requests');
            self.send_header('Cross-origin-Opener-Policy','same-origin');
            self.send_error(404,'File Not Found: %s' % self.path)
        
        
        
            
            
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(bytes("<html><body><h1>POST!</h1></body></html>","utf-8"))
    
    def list_directory(self, path):
        try:
            list = os.listdir(path)
        except os.error:
            self.send_error(404, "No permission to list directory")
            return None
        list.sort(key=lambda a: a.lower())
        f = StringIO()
        displaypath = cgi.escape(urllib.parse.unquote(self.path))
        f.write('<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">')
        f.write("<html>\n<title>Directory listing for %s</title>\n" % displaypath)
        f.write("<body>\n<h2>Directory listing for %s</h2>\n" % displaypath)
        f.write("<hr>\n<ul>\n")
        for name in list:
            fullname = os.path.join(path, name)
            displayname = linkname = name
            if os.path.isdir(fullname):
                displayname = name + "/"
                linkname = name + "/"
            if os.path.islink(fullname):
                displayname = name + "@"
            f.write('<li><a href="%s">%s</a>\n'
                    % (urllib.parse.quote(linkname), cgi.escape(displayname)))
        f.write("</ul>\n<hr>\n</body>\n</html>\n")
        length = f.tell()
        f.seek(0)
        self.send_response(200)
        encoding = sys.getfilesystemencoding()
        self.send_header("Content-type", "text/html; charset=%s" % encoding)
        self.send_header("Content-Length", str(length))
        self.end_headers()
        
        return f.getvalue()
        
def run():
    directory = os.path.join(os.path.dirname(__file__), '.')
    os.chdir(directory)
    print('Server Starting...')
    server_address = ('0.0.0.0', 9001)
    httpd = HTTPServer(server_address, HTTPServer_RequestHandler)
    print('Server Running...')
    #httpd.socket = ssl.wrap_socket (httpd.socket, 
    #    keyfile="./key.pem", 
    #    certfile='./cert.pem', server_side=True)
    httpd.serve_forever()

run()
'''
# Python 3

import sys
import socketserver
from http.server import SimpleHTTPRequestHandler

class WasmHandler(SimpleHTTPRequestHandler):
    def end_headers(self):        
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        self.send_header('Access-Control-Allow-Origin', '*')
        SimpleHTTPRequestHandler.end_headers(self)


# Python 3.7.5 adds in the WebAssembly Media Type. If this is an older
# version, add in the Media Type.
if sys.version_info < (3, 7, 5):
    WasmHandler.extensions_map['.wasm'] = 'application/wasm'


if __name__ == '__main__':
    PORT = 9000
    with socketserver.TCPServer(("", PORT), WasmHandler) as httpd:
        print("Listening on port {}. Press Ctrl+C to stop.".format(PORT))
        httpd.serve_forever()
'''
