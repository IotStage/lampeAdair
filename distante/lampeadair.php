<!DOCTYPE html>
<html>
	<head>Lampe à d'air</head>
	<body>
        	<h1>Lampe à dair</h1>
		<div id="matab">
        		<table class="table table-striped table-bordered bootstrap-datatable datatable responsive" id="mytable">
                       		<thead>
                              		<tr class="active info">
                                		<th>#</th>
                                		<th>Mesure</th>
                                		<th>Date prise</th>
                                		<th>Heure prise</th>
                              		</tr>
                        	</thead>

    				<tbody id="tbody">
					
				</tbody>
			</table>
		</div>
		<form method="POST" action="excel.php">
			<input type="text" name="output" hidden="true" id="output"/>
        		<center><input type="submit" name="export_excel" class="btn btn-success" value="TELECHARGER FORMAT EXCEL"></center>
		</form>
	
	
		<script type="text/javascript" src="jquery.js"></script>	
		<script type="text/javascript">
			window.onload=function(){
				setInterval(function(){
					$.ajax({
			       			url: 'http://192.168.1.102/iot_stage/lampeAdair/machine/remote.php',
			        		method: 'POST',
						data: { auth:'sur'},
						//contentType: 'application/json; charset=utf-8',
						//dataType: 'html',
						crossDomain: true
						
			 		}).then(function(data) {
						var latable = document.getElementById("tbody");
						
						latable.innerHTML=data;
						document.getElementById("output").value = document.getElementById("matab").innerHTML;
				
						});
					}, 
					3000
				);
			}

			
		</script>
	
	</body>
</html>

