#!/usr/bin/python
#coding=utf-8

from RequestParser import RequestParser
import RequestHandler


GETRequestMap = {
    '/'               : RequestHandler.index_request,
    '/service_status' : RequestHandler.service_status
    };

POSTRequestMap = {       
           }

def GETDispatch(httpd):
    path, params = RequestParser(httpd.path).parser()
    if path in GETRequestMap.keys():
        GETRequestMap[path](httpd, params)

def POSTDispatch(httpd):
    path, params = RequestParser(httpd.path).parser()
    if path in POSTRequestMap.keys():
        POSTRequestMap[path](httpd, params)
        