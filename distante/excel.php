<?php  
//require_once("connexion.php");
// $output = '';  

 if(isset($_POST["output"]))  {  
      /*$sql = "SELECT * FROM lampeadair";
      $bdd = (new Controller())->connecteToDataBase();
      //(new Controller())->selectEntries($bdd);
      $result = $bdd->query("SELECT * FROM lampeadair");
   
           $output .= '  
                <table class="table" bordered="1">  
                     <tr>  
                          <th>Mesure</th>  
                          <th>Date</th>  
                          <th>Heure</th>  
                     </tr>  
           ';  
	
           while($row =$result->fetch())  
           {  
                $output .= '  
                     <tr>  
                          <td>' . $row["mesure"] . '</td>  
                          <td>' . (explode(" ",$row["date_heure"])[0]) . '</td>  
                          <td>' . (explode(" ",$row["date_heure"])[1]) . '</td>  
                     </tr>  
                ';  
           }*/  
           $output = $_POST['output'];  
           header("Content-Type: application/xls");   
           header("Content-Disposition: attachment; filename=download.xls"); 
           echo  $output;  
        
}  
 ?>
