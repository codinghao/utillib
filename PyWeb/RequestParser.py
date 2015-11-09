#!/usr/bin/python
#coding=utf-8

class RequestParser:
    def __init__(self, request):
        self.__request = request;
        
    def __parser_params(self, params):
        param_dict = {}
        
        param_list = params.split('&')
        for param in param_list:
            idx = param.find('=')
            if idx == -1:
                break
            param_dict[param[0:idx]] = param[idx + 1:len(param)]
        
        return param_dict 
    
    def parser(self):
        idx = self.__request.find('?')      
        if idx == -1:
            return self.__request, {}
        
        return self.__request[0:idx], self.__parser_params(self.__request[idx + 1:len(self.__request)])
    
    
