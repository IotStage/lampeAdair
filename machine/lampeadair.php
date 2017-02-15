<?php

	//$_GET['donnees'] = 19;
	if(isset($_GET['donnees'])){
		$donnees = explode(" ",$_GET['donnees']);
		$mesure = $donnees[0];
		$ladate = $donnees[1] . " " . $donnees[2];
		//$ladate = date("Y-m-d H:i:s");
		
		require_once("ControllerClass.php");	
	
		//inserer dans la base de donnees locale
		$controller = new Controller();
		$bdd_local = $controller->connecteToDataBase("127.0.0.1", "iotStage", "abdoulaye", "kamstelecom");
		$controller->insertToDataBase($bdd_local, $mesure, $ladate);
		
		//repliquer dans la base de donnees disi
		$controller->replicate("http://edmi.ucad.sn/~moussadiallo/remote_lampedair.php", $_GET['donnees']);
//		$bdd_disi = $this->connecteToDataBase("srv01.crtp.ucad.sn", "c10pluviodic2", "PluvioDic2", "PluvioDic2");
  //              new Controller()->insertToDataBase($bdd_disi, $mesure, $ladate);

	}

?>
