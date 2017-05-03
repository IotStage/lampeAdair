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

		
		function connecteToDataBasePluvio($host=HOSTNAME, $basename=DATABASE_PLV, $username=USERNAME, $password=PASSWORD, $type_base="mysql"){
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
		function insertToDataBase($bdd, $courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate){
			if($bdd != null){
				echo "$courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate\n";
				$req = "INSERT INTO lampeadair(courant, tension_batterie, tension_panneau, lampe, etat_lampe, date_heure) VALUES(:courant,:tension_batterie, :tension_panneau, :lampe, :etat_lampe, :ladate);";
				$cursor = $bdd->prepare($req);
				$cursor->execute(array( 
					":ladate" => $ladate, 
					":courant" => $courant, 
					":tension_batterie" => $tension_batterie, 
					":tension_panneau" => $tension_panneau,
					":etat_lampe" => $etat_lampe,
					":lampe" => $id_lampe
				));
			}
		}

		/**
                * sauvegarder dans le tampon
                */
                function insertToBuffer($bdd, $courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate){
                        if($bdd != null){
                                $req = "INSERT INTO buffer(courant, tension_batterie, tension_panneau, lampe, etat_lampe, date_heure) VALUES(:courant,:tension_batterie, :tension_panneau, :lampe, :etat_lampe, :ladate);";
                                $cursor = $bdd->prepare($req);
                                $cursor->execute(array(
            
                                        ":ladate" => $ladate, 
                                        ":courant" => $courant, 
                                        ":tension_batterie" => $tension_batterie,
					":tension_panneau" => $tension_panneau, 
					":etat_lampe" => $etat_lampe,
                                        ":lampe" => $id_lampe
                                ));
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
		function replicate($bdd, $courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate){
			$donnees = implode(" ", [$id_lampe, $courant, $tension_batterie, $tension_panneau, $ladate, $etat_lampe]);

			if($this->isConnected()){
				$this->sendData(URL_SITE_MOUSSA_DIALLO, $donnees);	
			}else{
				$this->insertToBuffer($bdd, $courant, $tension_batterie, $tension_panneau, $id_lampe, $etat_lampe, $ladate);
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
			return false;
		}

		/**
		* envoyer les donnees qui sont dans le buffer et vider le buffer
		*/
		function flushBuffer($bdd){
			if($bdd != null){
                                $req = "SELECT * FROM buffer;";
                                $cursor = $bdd->prepare($req);
                                $cursor->execute();
                                $donnees=$cursor->fetchall(PDO::FETCH_NUM);
				foreach($donnees as $index => $valeur){
					//print_r($valeur);
					$this->replicate($bdd, $valeur[1], $valeur[2], $valeur[3], $valeur[4], $valeur[5], $valeur[6]);
					$this->deleteEntry($bdd, $valeur[0]);
				}
                        }
		}

		/**
		* supprimer une entree
		*/
		function deleteEntry($bdd, $id){
			echo "$id\n";
			if($bdd != null){
				
				$req = "DELETE FROM buffer WHERE id=:id;";
				$cursor = $bdd->prepare($req);
                                $cursor->execute(array(":id" => $id));
			}
		}

	}
?>
