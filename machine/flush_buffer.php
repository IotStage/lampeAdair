<?php
	require_once("ControllerClass.php");

        //inserer dans la base de donnees locale
        $controller = new Controller();
        $bdd_local = $controller->connecteToDataBase();
	$controller->flushBuffer($bdd_local, "lampadaire");


	$bdd_senpluvio = $controller->connecteToDataBasePluvio();
        $controller->flushBuffer($bdd_senpluvio, "senpluvio");
?>
