 <?php
 	if(isset($_GET['trame'])){
 		$trame = explode("|",$_GET['trame']);
 		//var_dump($trame);
 		$url = 'http://localhost:8888/lampeAdair/machine/lampeadair.php';
 		foreach ($trame as $key => $value) {
 			$donnees = str_replace(" ","%20",$value);
            $URL="$url?donnees=$donnees";
            $resultat = file_get_contents("$URL");
	    	
 		}

 	}