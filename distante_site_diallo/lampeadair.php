<!DOCTYPE html>
<html>

<head></head>

<body>
        <?php
            $bdd = new PDO('mysql:host=localhost;dbname=c3mdialloDB','c3mdialloesp','MdialloE16#');
        ?>
        <table class="table table-striped table-bordered bootstrap-datatable datatable responsive" id="mytable">
                       <thead>
                              <tr class="active info">
                                <th>#</th>
                                <th>Mesure</th>
                                <th>Date prise</th>
                                <th>Heure prise</th>
                              </tr>
                        </thead>
    			<tbody>
                        <?php
                                
				$reponse = $bdd->query("SELECT * FROM lampedair");
                    $i=0;
                    while ($donnees= $reponse->fetch()) {
                        $id=$donnees['id'];
                        $mesure=$donnees['mesure'];
	
                        $date=explode(" ",$donnees['date_heure'])[0];
                        $heure=explode(" ",$donnees['date_heure'])[1];
                        $i++;
                        ?>
                        <tr>
                                <td><?php echo $id; ?></td>
                                <td><?php  echo $mesure;?> </td>
                                <td><?php  echo $date;?></td>
                                <td><?php  echo $heure;?></td>
                        </tr>
                        <?php

                    }
                    $reponse->closeCursor();
                ?>
    </tbody>
</table>
<form method="POST" action="excel_lampedair.php">
        <center><input type="submit" name="export_excel" class="btn btn-success" value="TELECHARGER FORMAT EXCEL"></center>
</form>

</body>
</html>

