<?php
	require_once("config.php");
	Class Controller{
		
		public function __construct(){

		}

		function connecteToDataBase($host=HOSTNAME, $basename=DATABASE, $username=USERNAME, $password=PASSWORD, $type_base="mysql"){
			$bdd = null;
			if($type_base == "mysql"){
				$req_con = "mysql:host=" . $host . ";dbname=" . $basename . "";
				//$req2 = "SET NAMES UTF8";
				try{
					$bdd = new PDO($req_con, $username, $password);
					$req = $bdd->prepare("SET NAMES UTF8");
					$req->execute();

				}catch(Exception $e){
					$bdd = null;
				}
			}
			return $bdd;
		}		

		/**
		* inserer dans la base de données
		*/
		function insertToDataBase($bdd, $mesure, $ladate){
			if($bdd != null){
				$req = "INSERT INTO lampeadair(mesure,date_heure) VALUES(:mesure,:ladate);";
				$cursor = $bdd->prepare($req);
				$cursor->execute(array(":mesure" => $mesure, ":ladate" => $ladate));
			}
		}

		/**
		* selection de tous les enregistrements
		*/
		function selectEntries($bdd, $methode=PDO::FETCH_NUM){
			if($bdd != null){
				$req = "SELECT * FROM lampeadair;";
				$cursor = $bdd->prepare($req);
				$cursor->execute();
				return $fetchall($methode);
			}
		}

		/**
		* insert datas to remote databases;
		*/
		function replicateToDISI($mesure, $ladate){
			/*$http_server = new HTTPRequest("srv01.crtp.ucad.sn/lampeadair.php", HTTP_METH_POST);
			$http_server->addPostFields(array('mesure' => $mesure, 'date_heure' => $ladate));
			try {
    				$http_server->send();
			} catch (HTTPException $ex) {
			    
			}*/
			$res = "http://srv01.crtp.ucad.sn/lampeadair.php?mesure=$mesure&date_heure=$ladate";
			http_get($res);
		}

	}
?>
