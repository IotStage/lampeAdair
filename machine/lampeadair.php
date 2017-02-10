<?php

	//$_GET['donnees'] = 19;
	if(isset($_GET['donnees'])){
		$mesure = $_GET['donnees'];
		$ladate = date("Y-m-d H:i:s");
		
		require_once("ControllerClass.php");	
	
		//inserer dans la base de donnees locale
		$controller = new Controller();
		$bdd_local = $controller->connecteToDataBase("127.0.0.1", "iotStage", "abdoulaye", "kamstelecom");
		$controller->insertToDataBase($bdd_local, $mesure, $ladate);
		
		//repliquer dans la base de donnees disi
//		$controller->replicateToDISI($mesure, $ladate);
//		$bdd_disi = $this->connecteToDataBase("srv01.crtp.ucad.sn", "c10pluviodic2", "PluvioDic2", "PluvioDic2");
  //              new Controller()->insertToDataBase($bdd_disi, $mesure, $ladate);

	}

?>
