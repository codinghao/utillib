#!/usr/bin/python
#coding=utf-8

from BaseHTTPServer import HTTPServer
from CommandHandler import CommandHandler

class WebServer:
    def __init__(self):
        self.__server = HTTPServer(('0.0.0.0', 8088), CommandHandler)
    def run(self):
        print 'Web server runing ......'
        self.__server.serve_forever()

if __name__ == '__main__':
    WebServer().run()