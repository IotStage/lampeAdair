<?php

	//$_GET['donnees'] = 19;
	//echo $_GET['donnees'];
	if(isset($_GET['donnees'])){
		$ladate = date("Y-m-d H:i:s");
		$donnees = $_GET['donnees'];
		$donnees = explode(" ",$donnees);
		$id_lampe = $donnees[0];

		require_once("ControllerClass.php");
		//inserer dans la base de donnees locale
                $controller = new Controller();
                $bdd_local = $controller->connecteToDataBase();
		
		$bdd_senpluvio = $controller->connecteToDataBasePluvio();

		if($id_lampe == "meteo"){
			$controller->insertMsg($bdd_senpluvio, $_GET['donnees'] . " $ladate");
			$vitesse_vent = floatval($donnees[0]);
			$direction_vent = floatval($donnees[1]);
			$cardinalite = $donnees[2];
			$temperature = floatval($donnees[3]);
			$humidite = floatval($donnees[4]);
			$rayonnement_solaire = floatval($donnees[5]);
			
			//stocker dans la base de donnees
                                //$controller->insertToDataBase($bdd_senpluvio, array($vitesse_vent, $direction_vent, $cardinalite, $temperature, $humidite, $rayonnement_solaire, $ladate));

                                //repliquer dans la base le site
                                //$controller->replicatePluvio($bdd_senpluvio, array($vitesse_vent, $direction_vent, $cardinalite, $temperature, $humidite, $rayonnement_solaire, $ladate));
		}
		else{
			$courant = $donnees[1];
			$tension_batterie = $donnees[2];
			$tension_panneau = $donnees[3];
			$etat_lampe = intval($donnees[5]);
			//$ladate = date("Y-m-d H:i:s");

			$heure_init = intval($donnees[4]);
		
			//stocker le message
			$controller->insertMsg($bdd_local, "heure_init $heure_init, courant $courant, batterie $tension_batterie, panneau $tension_panneau, lampe $id_lampe,etat $etat_lampe,date $ladate");

			if(is_numeric($heure_init) && is_numeric($courant) && is_numeric($tension_batterie) && is_numeric($tension_panneau) && is_numeric($id_lampe) && is_numeric($etat_lampe)){
				//stocker dans la base de donnees
				$controller->insertToDataBase($bdd_local, $courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate);
		
				//repliquer dans la base le site
				$controller->replicate($bdd_local, $courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate);

			}

		}

	}

?>
