#!/usr/bin/python
#coding=utf-8

from BaseHTTPServer import BaseHTTPRequestHandler
import RequestMap

class CommandHandler(BaseHTTPRequestHandler):    
    def do_GET(self):
        RequestMap.GETDispatch(self)
    def do_POST(self):
        RequestMap.POSTDispatch(self)
    def do_HEAD(self):
        pass
    def do_PUT(self):
        pass
    def do_DELETE(self):
        pass
    def do_OPTIONS(self):
        pass
    def do_TRACE(self):
        pass
  
    def response_file(self, filename, header = {'Content-type' : 'text/html'}):
        self.send_response(200)
        
        for key in header.keys():
            self.send_header(key, header[key])
        self.end_headers()
        
        with open(filename, 'r+') as file:
            try:
                self.wfile.write(file.read())
            finally:
                file.close()
    
    def response_content(self, content, header = {'Content-type' : 'text/html'}):
        self.send_response(200)
        
        for key in header.keys():
            self.send_header(key, header[key])
        self.end_headers()
        
        self.wfile.write(content)
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
        
        