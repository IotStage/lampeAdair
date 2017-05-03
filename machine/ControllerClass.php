<?php
	require_once("config.php");
	Class Controller{
		
		public function __construct(){

		}

		function connecteToDataBase($host=HOSTNAME, $basename=DATABASE, $username=USERNAME, $password=PASSWORD, $type_base="mysql"){
			$bdd = null;
			if($type_base == "mysql"){
				$req_con = "mysql:host=localhost;port=8889;dbname=" . $basename . "";
				//$req2 = "SET NAMES UTF8";
				try{
					$bdd = new PDO($req_con, $username, $password);
					$req = $bdd->prepare("SET NAMES UTF8");
					$req->execute();

					//$this->writeLog("Connexion à la base de données ", "log", "lampadaire");

				}catch(Exception $e){
					$bdd = null;
					$this->writeLog("Problème de connexion à la base de données ", "log", "lampadaire");
					$this->writeLog("Problème de connexion à la base de données ", "error", "lampadaire");
				}
			}
			return $bdd;
		}		

		
		function connecteToDataBasePluvio($host=HOSTNAME, $basename=DATABASE_PLV, $username=USERNAME, $password=PASSWORD, $type_base="mysql"){
                        $bdd = null;
                        if($type_base == "mysql"){
                                $req_con = "mysql:host=localhost;port=8889;dbname=" . $basename . "";
                                //$req2 = "SET NAMES UTF8";
                                try{
                                        $bdd = new PDO($req_con, $username, $password);
                                        $req = $bdd->prepare("SET NAMES UTF8");
                                        $req->execute();

					//$this->writeLog("Connexion à la base de données ", "log", "senpluvio");

                                }catch(Exception $e){
                                        $bdd = null;
					$this->writeLog("Problème de connexion à la base de données ", "log", "senpluvio");
					$this->writeLog("Problème de connexion à la base de données ", "error", "senpluvio");
                                }
                        }
                        return $bdd;
                }

		/**
		* inserer dans la base de données
		*/
		//function insertToDataBase($bdd, $data_array, $plateforme $courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate){
	 	function insertToDataBase($bdd, $data_array, $plateforme){	
			if($bdd != null){
				
				if($plateforme == "lampadaire"){
					//echo "$courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate\n";
					$req = "INSERT INTO lampeadair(courant, tension_batterie, tension_panneau, lampe, etat_lampe, date_heure) VALUES(:courant,:tension_batterie, :tension_panneau, :lampe, :etat_lampe, :ladate);";
					$cursor = $bdd->prepare($req);
					$cursor->execute(array( 
						":ladate" => $data_array['date_heure'], 
						":courant" => $data_array['courant'], 
						":tension_batterie" => $data_array['tension_batterie'], 
						":tension_panneau" => $data_array['tension_panneau'],
						":etat_lampe" => $data_array['etat_lampe'],
						":lampe" => $data_array['lampe']
					));
				}
				else if($plateforme == "senpluvio"){
//					print_r($data_array);
					$req = "INSERT INTO station_meteo(vitesse_vent, direction_vent, cardinalite, temperature, humidite, rayonnement_solaire, date_heure) VALUES(:vitesse_vent, :direction_vent, :cardinalite, :temperature, :humidite, :rayonnement_solaire, :ladate);";
                                        $cursor = $bdd->prepare($req);
                                        $cursor->execute(array(
                                                ":ladate" => $data_array['date_heure'],
                                                ":vitesse_vent" => $data_array['vitesse_vent'],
                                                ":direction_vent" => $data_array['direction_vent'], 
                                                ":cardinalite" => $data_array['cardinalite'],
                                                ":temperature" => $data_array['temperature'],
                                                ":humidite" => $data_array['humidite'],
						":rayonnement_solaire" => $data_array['rayonnement_solaire']
                                        ));

				}
			}
		}

	//$controller->insertToDataBase($bdd_senpluvio, array($vitesse_vent, $direction_vent, $cardinalite, $temperature, $humidite, $rayonnement_solaire, $ladate));
		/**
                * sauvegarder dans le tampon
                */
                //function insertToBuffer($bdd, $courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate){
                function insertToBuffer($bdd, $data_array, $plateforme){  
		      if($bdd != null){
                                if($plateforme == "lampadaire"){
                                        //echo "$courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate\n";
                                        $req = "INSERT INTO buffer(courant, tension_batterie, tension_panneau, lampe, etat_lampe, date_heure) VALUES(:courant,:tension_batterie, :tension_panneau, :lampe, :etat_lampe, :ladate);";
                                        $cursor = $bdd->prepare($req);
                                        $cursor->execute(array(
                                                ":ladate" => $data_array['date_heure'],
                                                ":courant" => $data_array['courant'],
                                                ":tension_batterie" => $data_array['tension_batterie'],
                                                ":tension_panneau" => $data_array['tension_panneau'],
                                                ":etat_lampe" => $data_array['etat_lampe'],
                                                ":lampe" => $data_array['lampe']
                                        ));
                                }
                                else if($plateforme == "senpluvio"){
                                        $req = "INSERT INTO buffer(vitesse_vent, direction_vent, cardinalite, temperature, humidite, rayonnement_solaire, date_heure) VALUES(:vitesse_vent, :direction_vent, :cardinalite, :temperature, :humidite, :rayonnement_solaire, :ladate);";
                                        $cursor = $bdd->prepare($req);
                                        $cursor->execute(array(
                                                ":ladate" => $data_array['date_heure'],
                                                ":vitesse_vent" => $data_array['vitesse_vent'],
                                                ":direction_vent" => $data_array['direction_vent'],
                                                ":cardinalite" => $data_array['cardinalite'],
                                                ":temperature" => $data_array['temperature'],
                                                ":humidite" => $data_array['humidite'],
                                                ":rayonnement_solaire" => $data_array['rayonnement_solaire']
                                        ));

                                }
                        }
                }


		function insertMsg($bdd, $msg){
                        if($bdd != null){
                                //echo "$courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate\n";
                                $req = "INSERT INTO message(msg) VALUES(:msg);";
                                $cursor = $bdd->prepare($req);
                                $cursor->execute(array(
                                        ":msg" => $msg
                                ));
				
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
				return $cursor->fetchall($methode);
			}
		}

		/**
		* insert datas to remote databases;
		*/
		//function replicate($bdd, $courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate){
		function replicate($bdd, $data_array, $plateforme){

			//print_r($data_array);
			$donnees = implode(" ", $data_array);

			if($this->isConnected()){
				if($plateforme == "lampadaire"){
					$this->sendData(URL_SITE_MOUSSA_DIALLO, $donnees);
					
				}
				else if($plateforme == "senpluvio"){
					$this->sendData(URL_SITE_MOUSSA_DIALLO_PLV, $donnees);
					
				}
				$this->writeLog("Envoie des données $donnees", "log", $plateforme);
			}else{
				$this->insertToBuffer($bdd, $data_array, $plateforme);
				$this->writeLog("Insertion des données $donnees dans le buffer", "log", $plateforme);
				$this->writeLog("Insertion des données $donnees", "buffer", $plateforme);
			}
		}

		/**
		* envoyer les données 
		*/
		function sendData($url, $donnees){
			$donnees = str_replace(" ","%20",$donnees);
                        $URL="$url?donnees=$donnees";
                        $url = urlencode($URL);
                        $resultat = file_get_contents("$URL");
			
		}

		/**
		* verifier si internet est disponible
		*/
		function isConnected(){
			if($connected = @fsockopen('www.google.com', 80, $num, $error, 5)){
				fclose($connected);
				return true;
			}
			$this->writeLog("Problème de connexion internet", "error");
			return false;
		}

		/**
		* envoyer les donnees qui sont dans le buffer et vider le buffer
		*/
		function flushBuffer($bdd, $plateforme="lampadaire"){
			if($bdd != null){
				$this->writeLog("Début opération de vidage du buffer", "buffer", $plateforme);

                                $req = "SELECT * FROM buffer;";
                                $cursor = $bdd->prepare($req);
                                $cursor->execute();
                                $donnees=$cursor->fetchall(PDO::FETCH_ASSOC);
				foreach($donnees as $index => $valeur){
					$this->deleteEntry($bdd, $valeur['id']);
					array_shift($valeur);
					$this->replicate($bdd, $valeur, $plateforme);
					//print_r($valeur);
				}
			
				if(count($donnees)>=1){
					$this->writeLog("Vidage complète", "buffer", $plateforme);
				}else{
					$this->writeLog("Pas de données", "buffer", $plateforme);
				}
                        }
		}

		/**
		* supprimer une entree
		*/
		function deleteEntry($bdd, $id){
			//echo "$id\n";
			if($bdd != null){
				
				$req = "DELETE FROM buffer WHERE id=:id;";
				$cursor = $bdd->prepare($req);
                                $cursor->execute(array(":id" => $id));
			}
		}

		/**
		* Ecrire dans les fichiers logs
		*/
		function writeLog($event, $type_event, $plateforme=""){
			$date_heure = date("Y-m-d H:i:s");
			if($type_event == "error"){
				exec("echo \"$date_heure -> ERROR: $event\" >> /var/log/senpluvio/error.log");
				exec("echo \"$date_heure -> ERROR: $event\" >> /var/log/lampadaire/error.log");
			} 
			else if($type_event == "log"){
                                exec("echo \"$date_heure -> EVENT: $event\" >> /var/log/$plateforme/$plateforme.log");
                        }
			else if($type_event == "buffer"){
                                exec("echo \"$date_heure -> BUFFER: $event\" >> /var/log/$plateforme/buffer.log");
				
                        }
		}

	}

	
?>
