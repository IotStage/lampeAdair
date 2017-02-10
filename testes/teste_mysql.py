#! /usr/bin/python
#-*- coding:utf-8 -*-

import MySQLdb

db = MySQLdb.connect(host="localhost",user="abdoulaye",passwd="kamstelecom",db="teste_bdd")
cur = db.cursor()

cur.execute("SELECT * FROM matab1")
