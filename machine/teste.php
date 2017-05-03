<?php

	//$_GET['donnees'] = 19;
	echo $_GET['donnees'] = "2 0 12 17 1 0";
//	echo $_GET['donnees'] = "meteo 14.41 0.00 N 0.00 0.00 0.0";
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
//			$controller->insertMsg($bdd_senpluvio, $_GET['donnees'] . " $ladate");
			$data_to_send = array(
				"vitesse_vent" => floatval($donnees[1]),
				"direction_vent" => floatval($donnees[2]),
				"cardinalite" => $donnees[3],
				"temperature" => floatval($donnees[4]),
				"humidite" => floatval($donnees[5]),
				"rayonnement_solaire" => floatval($donnees[6]),
				"date_heure" => $ladate
			);

//			print_r($data_to_send);
			
			//stocker dans la base de donnees
                                $controller->insertToBuffer($bdd_senpluvio, $data_to_send, "senpluvio");

                                //repliquer dans la base le site
                        //$controller->replicate($bdd_senpluvio, $data_to_send, "senpluvio");
		}
		else{
			$courant = $donnees[1];
			$tension_batterie = $donnees[2];
			$tension_panneau = $donnees[3];
			$etat_lampe = intval($donnees[5]);
			//$ladate = date("Y-m-d H:i:s");

			$heure_init = intval($donnees[4]);
		
			//stocker le message
			//$controller->insertMsg($bdd_local, "heure_init $heure_init, courant $courant, batterie $tension_batterie, panneau $tension_panneau, lampe $id_lampe,etat $etat_lampe,date $ladate");

			if(is_numeric($heure_init) && is_numeric($courant) && is_numeric($tension_batterie) && is_numeric($tension_panneau) && is_numeric($id_lampe) && is_numeric($etat_lampe)){
				//stocker dans la base de donnees
				$data_to_send = array(
					"courant" => $courant,
					"tension_batterie" => $tension_batterie,
					"tension_panneau" => $tension_panneau,
					"lampe" => $id_lampe,
					"etat_lampe" => $etat_lampe,
					"date_heure" => $ladate
				);
				
				//print_r($data_to_send);
				$controller->insertToBuffer($bdd_local, $data_to_send, "lampadaire");
		
				//repliquer dans la base le site
			//	$controller->replicate($bdd_local, $data_to_send, "lampadaire");

			}

		}

	}

?>
