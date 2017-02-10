<?php
//	print_r($_GET);	
	if(isset($_POST['auth']) && $_POST['auth'] == "sur"){
                	require_once("ControllerClass.php");
                                
				$bdd = (new Controller())->connecteToDataBase();
				$reponse = $bdd->query("SELECT * FROM lampeadair");
                                    $i=0;
                                    while ($donnees= $reponse->fetch()) {
                                        $id=$donnees['id'];
                                        $mesure=$donnees['mesure'];
					
                                        $date=explode(" ",$donnees['date_heure'])[0];
                                        $heure=explode(" ",$donnees['date_heure'])[1];
                                        $i++;
                                        
                                        echo "<tr>
                                                <td>$id</td>
                                                <td>$mesure</td>
                                                <td>$date</td>
                                                <td>$heure</td>
                                        </tr>
                                        ";

                                    }
                                    $reponse->closeCursor();
	}
	else{
	//	echo 'probleme' . $_POST['auth'];
	}
?>

