<?php  

 if(isset($_POST["output"]))  {  
      
	$output = $_POST['output'];  
        header("Content-Type: application/xls");   
        header("Content-Disposition: attachment; filename=download.xls"); 
        echo  $output;  
        
}  
 ?>
