#!/usr/bin/python
#coding=utf-8

import subprocess

def index_request(httpd, params):
    httpd.response_file('./template/service.html')

def service_status(httpd, params):
    sp = subprocess.Popen('dir', shell = True, stdout = subprocess.PIPE, stderr = subprocess.STDOUT)
    httpd.response_content(sp.stdout.read())