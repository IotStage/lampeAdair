#! /usr/bin/python
#-*- coding:utf -*-

"""
recevoir les données du RF
"""

class Database:
	def __init__(self, username, password, database, hostname="localhost"):
		self.__username = username
		self.__password = password
		if hostname=="":
			self.__hostname = "localhost"
		else
			self.__hostname = hostname
		self.__database = database
		
	def connect(self):
		

	def 

	@property
	def username(self):
		return self.__username

	@username.setter
	def username(self, new_username):
		if new_username != "":
			self.__username = new_username
	
		
		
		
