<?php
    
    if(isset($_GET['donnees'])){
        $donnees = explode(" ",$_GET['donnees']);
        $mesure = $donnees[0];
        $ladate = $donnees[1] . " " . $donnees[2];  
    
        try {
            $bdd = new PDO('mysql:host=localhost;dbname=c3mdialloDB','c3mdialloesp','MdialloE16#');

            $req = "INSERT INTO lampedair(mesure, date_heure) VALUES(:mesure, :ladate);";
            $cursor = $bdd->prepare($req);
            $cursor->execute(array(":mesure" => $mesure, ":ladate" => $ladate));
        }
        catch (Exception $e) {
            die('Erreur : ' . $e->getMessage());
        }
        
    }

?>
