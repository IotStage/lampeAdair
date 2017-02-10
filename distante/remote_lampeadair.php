<?php

	if(isset($_GET['mesure']) && isset($_GET['date_heure'])){
		$mesure = $_GET['mesure'];
		$ladate = $_GET['date_heure'];

		require_once ("connexion.php");
		
		//inserer dans la base de donnees
		(new Controller())->insertToDataBase($bdd, $mesure, $ladate);
	}

	Class Controller{
		
		public function __construct(){

		}


		/**
		* inserer dans la base de données
		*/
		function insertToDataBase($bdd, $mesure, $ladate){
			if($bdd != null){
				$req = "INSERT INTO lampeadair(mesure, date_heure) VALUES(:mesure,:ladate);";
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

		

	}
?>
