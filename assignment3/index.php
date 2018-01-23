<!DOCTYPE html>
<html>
<head>
<meta charset="utf=8">
<title>Faculty Info</title>
<?php
$first_form_result='';
$second_form_result='';
$third_form_result='';
$forth_form_result="";
$fifth_form_result="";
$sixth_form_result="";
$seventh_form_result="";
$year3=-1;
$year5==1;
$status='default';
$DB_NAME='sample';
$DB_USER='root';
$DB_PASSWORD='7221';
$DB_HOST='127.0.0.1';
$select1=1;
$select2=1;
$conn;
if(isset($_POST["first_form"])) 
{
	$conn=mysqli_connect($DB_HOST,$DB_USER,$DB_PASSWORD,$DB_NAME) or die('could not connect');
	$status='connected ';
	$sqlquery = "Use sample;";
	if ($conn->query($sqlquery) === FALSE) echo "Error selecting to database: " . $conn->error;
	if (isset($_POST['respon']))
	{    		
		if($_POST['respon']=='Head, Centre for Educational Technology') $select1=1;
    	else if($_POST['respon']=="Chairman, Computer Purchase, Maintenance &amp; Networking Committee") $select1=2;
    	else if($_POST['respon']=='Head, Computer and Informatics Centre') $select1=3;
    	else if($_POST['respon']=='Chairman, Kalpana Chawla Space Technology Cell') $select1=4;
   	else if($_POST['respon']=='Dean, SRIC') $select1=5;
   	else if($_POST['respon']=='Chairman, Senate') $select1=6;
    	else if($_POST['respon']=='Head, Rajendra Mishra School of Engineering Entrepreneurship') $select1=7;
    	else if($_POST['respon']=='Prof-in-Charge, Rajarhat Research Park') $select1=8;
		else if($_POST['respon']=='Associate Head, Institute Information Cell') $select1=9;
		else if($_POST['respon']=='Chairman, GATE/JAM') $select1=10;
		else if($_POST['respon']=='Prof-in-Charge, B. C. Roy Technology Hospital') $select1=11;
		else if($_POST['respon']=='Vice-Chairman, ERP') $select1=12;
		else if($_POST['respon']=='Head, Institute Information Cell') $select1=13;
		else if($_POST['respon']=='Head, Computer Science and Engineering') $select1=14;
		else if($_POST['respon']=='Assistant Warden, Meghnath Saha Hall') $select1=15;
		else if($_POST['respon']=='Chief Vigilance Officer') $select1=16;
   }
   $sqlquery='select f.name "name1" from faculty_details f,responsibilities r where f.id=r.id and responsibilty="'.$_POST['respon'].'";';
	$queryresult=$conn->query($sqlquery);
	if ($queryresult->num_rows > 0) 
	{
   	while($row = $queryresult->fetch_assoc())
      	$first_form_result=$row['name1'];
	} 
	else 
    	$first_form_result=" None"; 
   $conn->close();
}
?>
</head>
<body>
<form action="index.php" method="post" >
<br>1: select Responsibility     
<select name="respon" >
   <option value="Head, Centre for Educational Technology" 
  <?php if($select1 == '1'){ echo('selected="selected"');}?> >Head, Centre for Educational Technology</option>
   <option value="Chairman, Computer Purchase, Maintenance &amp;amp; Networking Committee" 
  <?php if($select1 == '2'){ echo('selected="selected"');}?> >Chairman, Computer Purchase, Maintenance & Networking Committee </option>
    <option value="Head, Computer and Informatics Centre" 
  <?php if($select1 == '3'){ echo('selected="selected"');}?> >Head, Computer and Informatics Centre</option>
   <option value="Chairman, Kalpana Chawla Space Technology Cell"
  <?php if($select1 == '4'){ echo('selected="selected"');}?> >Chairman, Kalpana Chawla Space Technology Cell</option>
    <option value="Dean, SRIC"
  <?php if($select1 == '5'){ echo('selected="selected"');}?> >Dean, SRIC</option>
    <option value="Chairman, Senate"
  <?php if($select1 == '6'){ echo('selected="selected"');}?> >Chairman, Senate</option>
    <option value="Head, Rajendra Mishra School of Engineering Entrepreneurship"
  <?php if($select1 == '7'){ echo('selected="selected"');}?> >Head, Rajendra Mishra School of Engineering Entrepreneurship</option>
    <option value="Prof-in-Charge, Rajarhat Research Park"
  <?php if($select1 == '8'){ echo('selected="selected"');}?> >Prof-in-Charge, Rajarhat Research Park</option>
    <option value="Associate Head, Institute Information Cell"
  <?php if($select1 == '9'){ echo('selected="selected"');}?> >Associate Head, Institute Information Cell</option>
    <option value="Chairman, GATE/JAM"
  <?php if($select1 == '10'){ echo('selected="selected"');}?> >Chairman, GATE/JAM</option>
    <option value="Prof-in-Charge, B. C. Roy Technology Hospital"
  <?php if($select1 == '11'){ echo('selected="selected"');}?> >Prof-in-Charge, B. C. Roy Technology Hospital</option>
    <option value="Vice-Chairman, ERP"
  <?php if($select1 == '12'){ echo('selected="selected"');}?> >Vice-Chairman, ERP</option>
    <option value="Head, Institute Information Cell"
  <?php if($select1 == '13'){ echo('selected="selected"');}?> >Head, Institute Information Cell</option>
    <option value="Head, Computer Science and Engineering"
  <?php if($select1 == '14'){ echo('selected="selected"');}?> >Head, Computer Science and Engineering</option>
    <option value="Assistant Warden, Meghnath Saha Hall"
  <?php if($select1 == '15'){ echo('selected="selected"');}?> >Assistant Warden, Meghnath Saha Hall</option>
    <option value="Chief Vigilance Officer"
  <?php if($select1 == '16'){ echo('selected="selected"');}?> >Chief Vigilance Officer</option>
</select>
<input type="submit" name="first_form" value="Get Result"/>   Result: <?php echo($first_form_result); ?>
</form>
<?php
if(isset($_POST["second_form"])) 
{
	$conn=mysqli_connect($DB_HOST,$DB_USER,$DB_PASSWORD,$DB_NAME) or die('could not connect');
	$status='connected ';
	$sqlquery = "Use sample;";
	if ($conn->query($sqlquery) === FALSE) echo "Error selecting to database: " . $conn->error;
	if (isset($_POST['award']))
	{    		
		if($_POST['award']=='IBM Faculty Award') $select2=1;
    	else if($_POST['award']=="INAE Young Engg. Award") $select2=2;
    	else if($_POST['award']=='INSA Medal for YS') $select2=3;
    	else if($_POST['award']=='NSF TCPP/CDER') $select2=4;
   	else if($_POST['award']=='Simons Associateship, ICTP') $select2=5;
   	else if($_POST['award']=='DAAD-IIT Faculty Exchange') $select2=6;
    	else if($_POST['award']=='National Doctoral Fellowship') $select2=7;
    	else if($_POST['award']=='SAP Labs India Doctoral Fellow') $select2=8;
		else if($_POST['award']=='Microsoft Valued Professional') $select2=9;
		else if($_POST['award']=='IAS Young Associate') $select2=10;
		else if($_POST['award']=='Indo-USSTF Fellowship') $select1=11;
		else if($_POST['award']=='INSA Young Scientist') $select2=12;
		else if($_POST['award']=='ISA TechnoInventor Award') $select2=13;
		else if($_POST['award']=='Yahoo Faculty Award') $select2=14;
		else if($_POST['award']=='Fellow of INAE') $select2=15;
		else if($_POST['award']=='Fellow of Indian Academy of Sc') $select2=16;
		else if($_POST['award']=='IESA Technomentor Award') $select2=17;
		else if($_POST['award']=='INSA YoungScientist Medal') $select2=18;
		else if($_POST['award']=='President of India Gold Medal') $select2=19;
		else if($_POST['award']=='Shanti Swarup Bhatnagar Prize') $select2=20;
		else if($_POST['award']=='Swarnajayanti Fellowship') $select2=21;
		else if($_POST['award']=='IASc Young Associate') $select2=22;
		else if($_POST['award']=='UGC Young Teacher') $select2=23;
		else if($_POST['award']=='UNESCO/ROSTSCA Young Scientist') $select2=24;
		else if($_POST['award']=='Huboldt Research Fellow') $select2=25;
		else if($_POST['award']=='IBM SUR Award') $select2=26;
		else if($_POST['award']=='IEI Young Engineers Award') $select2=27;
		else if($_POST['award']=='RECI Award (RAE, U.K.)') $select2=28;
		else if($_POST['award']=='Best Paper ICDCN 15') $select2=29;
		else if($_POST['award']=='DAAD Fellowship') $select2=30;
		else if($_POST['award']=='Honorable mention COMSNETS') $select2=31;
		else if($_POST['award']=='INAE Student Project') $select2=32;
		else if($_POST['award']=='NIXI Fellowship') $select2=33;
		else if($_POST['award']=='Alexander von Humboldt Fellow') $select2=34;
		else if($_POST['award']=='University Gold Medal') $select2=35;
		else if($_POST['award']=='Rajiv Gandhi Research grant.') $select2=36;
		else if($_POST['award']=='Humboldt Fellowship') $select2=37;
		else if($_POST['award']=='IEEE ComSoc AP Young Res Award') $select2=38;
		else if($_POST['award']=='NASI Young Scientist Award') $select2=39;
		else if($_POST['award']=='Samsung Innovation Awards') $select2=40;
   }
   $sqlquery='select f.name "name1" from faculty_details f,awards_accolades a where f.id=a.id and title="'.$_POST['award'].'";';   
	$queryresult=$conn->query($sqlquery);
	if ($queryresult->num_rows > 0) 
	{
   	while($row = $queryresult->fetch_assoc())
      	$second_form_result=$second_form_result.$row['name1'].' ;  ';
	} 
	else 
    	$second_form_result=" None";
   $conn->close();
}
?>
<form name="f1" action="index.php"  method="post" >
<br>2 : select Award        
<select name="award" >
   <option value="IBM Faculty Award" 
  <?php if($select2 == '1'){ echo('selected="selected"');}?> >IBM Faculty Award</option>
   <option value="INAE Young Engg. Award" 
  <?php if($select2 == '2'){ echo('selected="selected"');}?> >INAE Young Engg. Award</option>
    <option value="INSA Medal for YS" 
  <?php if($select2 == '3'){ echo('selected="selected"');}?> >INSA Medal for YS</option>
   <option value="NSF TCPP/CDER"
  <?php if($select2 == '4'){ echo('selected="selected"');}?> >NSF TCPP/CDER</option>
    <option value="Simons Associateship, ICTP"
  <?php if($select2 == '5'){ echo('selected="selected"');}?> >Simons Associateship, ICTP</option>
    <option value="DAAD-IIT Faculty Exchange"
  <?php if($select2 == '6'){ echo('selected="selected"');}?> >DAAD-IIT Faculty Exchange</option>
    <option value="National Doctoral Fellowship"
  <?php if($select2 == '7'){ echo('selected="selected"');}?> >National Doctoral Fellowship</option>
    <option value="SAP Labs India Doctoral Fellow"
  <?php if($select2 == '8'){ echo('selected="selected"');}?> >SAP Labs India Doctoral Fellow</option>
    <option value="Microsoft Valued Professional"
  <?php if($select2 == '9'){ echo('selected="selected"');}?> >Microsoft Valued Professional</option>
    <option value="IAS Young Associate"
  <?php if($select2 == '10'){ echo('selected="selected"');}?> >IAS Young Associate</option>
    <option value="Indo-USSTF Fellowship"
  <?php if($select2 == '11'){ echo('selected="selected"');}?> >Indo-USSTF Fellowship</option>
    <option value="INSA Young Scientist"
  <?php if($select2 == '12'){ echo('selected="selected"');}?> >INSA Young Scientist</option>
    <option value="ISA TechnoInventor Award"
  <?php if($select2 == '13'){ echo('selected="selected"');}?> >ISA TechnoInventor Award</option>
    <option value="Yahoo Faculty Award"
  <?php if($select2 == '14'){ echo('selected="selected"');}?> >Yahoo Faculty Award</option>
    <option value="Fellow of INAE"
  <?php if($select2 == '15'){ echo('selected="selected"');}?> >Fellow of INAE</option>
    <option value="Fellow of Indian Academy of Sc"
  <?php if($select2 == '16'){ echo('selected="selected"');}?> >Fellow of Indian Academy of Sc</option>
  <option value="IESA Technomentor Award"
  <?php if($select2 == '17'){ echo('selected="selected"');}?> >IESA Technomentor Award</option>
  <option value="INSA YoungScientist Medal"
  <?php if($select2 == '18'){ echo('selected="selected"');}?> >INSA YoungScientist Medal</option>
  <option value="President of India Gold Medal"
  <?php if($select2 == '19'){ echo('selected="selected"');}?> >President of India Gold Medal</option>
  <option value="Shanti Swarup Bhatnagar Prize"
  <?php if($select2 == '20'){ echo('selected="selected"');}?> >Shanti Swarup Bhatnagar Prize</option>
  <option value="Swarnajayanti Fellowship"
  <?php if($select2 == '21'){ echo('selected="selected"');}?> >Swarnajayanti Fellowship</option>
  <option value="IASc Young Associate"
  <?php if($select2 == '22'){ echo('selected="selected"');}?> >IASc Young Associate</option>
  <option value="UGC Young Teacher"
  <?php if($select2 == '23'){ echo('selected="selected"');}?> >UGC Young Teacher</option>
  <option value="UNESCO/ROSTSCA Young Scientist"
  <?php if($select2 == '24'){ echo('selected="selected"');}?> >UNESCO/ROSTSCA Young Scientist</option>
  <option value="Huboldt Research Fellow"
  <?php if($select2 == '25'){ echo('selected="selected"');}?> >Huboldt Research Fellow</option>
  <option value="IBM SUR Award"
  <?php if($select2 == '26'){ echo('selected="selected"');}?> >IBM SUR Award</option>
  <option value="IEI Young Engineers Award"
  <?php if($select2 == '27'){ echo('selected="selected"');}?> >IEI Young Engineers Award</option>
  <option value="RECI Award (RAE, U.K.)"
  <?php if($select2 == '28'){ echo('selected="selected"');}?> >RECI Award (RAE, U.K.)</option>
  <option value="Best Paper ICDCN 15"
  <?php if($select2 == '29'){ echo('selected="selected"');}?> >Best Paper ICDCN 15</option>
  <option value="DAAD Fellowship"
  <?php if($select2 == '30'){ echo('selected="selected"');}?> >DAAD Fellowship</option>
  <option value="Honorable mention COMSNETS"
  <?php if($select2 == '31'){ echo('selected="selected"');}?> >Honorable mention COMSNETS</option>
  <option value="INAE Student Project"
  <?php if($select2 == '33'){ echo('selected="selected"');}?> >INAE Student Project</option>
  <option value="NIXI Fellowship"
  <?php if($select2 == '33'){ echo('selected="selected"');}?> >NIXI Fellowship</option>
  <option value="Alexander von Humboldt Fellow"
  <?php if($select2 == '34'){ echo('selected="selected"');}?> >Alexander von Humboldt Fellow</option>
  <option value="University Gold Medal"
  <?php if($select2 == '35'){ echo('selected="selected"');}?> >University Gold Medal</option>
  <option value="Rajiv Gandhi Research grant."
  <?php if($select2 == '36'){ echo('selected="selected"');}?> >Rajiv Gandhi Research grant.</option>
  <option value="Humboldt Fellowship"
  <?php if($select2 == '37'){ echo('selected="selected"');}?> >Humboldt Fellowship</option>
  <option value="IEEE ComSoc AP Young Res Award"
  <?php if($select2 == '38'){ echo('selected="selected"');}?> >IEEE ComSoc AP Young Res Award</option>
  <option value="NASI Young Scientist Award"
  <?php if($select2 == '39'){ echo('selected="selected"');}?> >NASI Young Scientist Award</option>
  <option value="Samsung Innovation Awards"
  <?php if($select2 == '40'){ echo('selected="selected"');}?> >Samsung Innovation Awards</option>
</select>
<input type="submit" name="second_form" value="Get Result"/>   Result: <?php echo($second_form_result); ?>
</form>
<?php if(isset($_POST["third_form"])) 
{
	$i=1;
	$conn=mysqli_connect($DB_HOST,$DB_USER,$DB_PASSWORD,$DB_NAME) or die('could not connect');
	$status='connected ';
	$sqlquery = "Use sample;";
	if ($conn->query($sqlquery) === FALSE) echo "Error selecting to database: " . $conn->error;
	if (is_numeric($_POST['year3']))	
   	$year3=$_POST['year3'];
   else $year3=-1;
	$sqlquery='select title from faculty_details f,publications p where f.id=p.id and f.name="'.$_POST['name3'].'" and year='.$year3.';';   
	$queryresult=$conn->query($sqlquery);
	if ($queryresult->num_rows > 0) 
	{
   	while($row = $queryresult->fetch_assoc())
		{
      	$third_form_result=$third_form_result.'<font color="red">'.$i.'</font>'.'   '.$row['title'].'<br>';
      	$i=$i+1;
      }	
	} 
	else 
    	$third_form_result=" None";
   $conn->close();
}
?>
<form action="index.php"  method="post" >
<br>3 : Enter Faculty Name <input type="text" name="name3" maxlength="50" value=<?php echo ('"'.$_POST['name3'].'"'); ?> />
  Year <input type="text" name="year3" maxlength="4" value=<?php echo ('"'.$_POST['year3'].'"'); ?> />
<input type="submit" name="third_form" value="Get Result"/> <br><b>Result Publications:</b><br> <?php echo($third_form_result); ?>
</form>
<?php if(isset($_POST["forth_form"])) 
{
	$i=1;
	$conn=mysqli_connect($DB_HOST,$DB_USER,$DB_PASSWORD,$DB_NAME) or die('could not connect');
	$status='connected ';
	$sqlquery = "Use sample;";
	if ($conn->query($sqlquery) === FALSE) echo "Error selecting to database: " . $conn->error;
	$sqlquery='select name from faculty_details f,(select t.id_count, id from (select COUNT(id) id_count,id from group_members GROUP BY id order by COUNT(id) DESC) t LIMIT 1) m where f.id=m.id;'; 
	$queryresult=$conn->query($sqlquery);
	if ($queryresult->num_rows > 0) 
	{
   	while($row = $queryresult->fetch_assoc())
		{
      	$forth_form_result=$forth_form_result.'<font color="red">'.$i.'</font>'.'  '.$row['name'].'   ';
      	$i=$i+1;
      }	
	} 
	else 
    	$forth_form_result=" None";
   $conn->close();
}
?>
<form action="index.php"  method="post" >
<br>4 : Get Faculty name Who has max number of Students
<input type="submit" name="forth_form" value="Get Result"/> Result : <?php echo($forth_form_result); ?>
</form>
<?php if(isset($_POST["fifth_form"])) 
{
	$i=1;
	$conn=mysqli_connect($DB_HOST,$DB_USER,$DB_PASSWORD,$DB_NAME) or die('could not connect');
	$status='connected ';
	$sqlquery = "Use sample;";
	if ($conn->query($sqlquery) === FALSE) echo "Error selecting to database: " . $conn->error;
	if (is_numeric($_POST['year5']))	
   	$year5=$_POST['year5'];
   else $year5=-1;
	$sqlquery='select f.name "name" from faculty_details f,(select t.id_count, id from (select COUNT(id) id_count,id from publications where year='.$year5.' GROUP BY id order by COUNT(id) DESC) t limit 1) m where f.id=m.id;';	
	$queryresult=$conn->query($sqlquery);
	if ($queryresult->num_rows > 0) 
	{
   	while($row = $queryresult->fetch_assoc())
		{
      	$fifth_form_result=$fifth_form_result.'<font color="red">'.$i.'</font>'.'  '.$row['name'].'   ';
      	$i=$i+1;
      }	
	} 
	else 
    	$fifth_form_result=" None";
   $conn->close();
}
?>
<form action="index.php"  method="post" >
<br>5 : Get Faculty who has the maximum number of publications in a given year
<input type="text" name="year5" maxlength="4" value=<?php echo ('"'.$_POST['year5'].'"'); ?> />
<input type="submit" name="fifth_form" value="Get Result"/> Result : <?php echo($fifth_form_result); ?>
</form>
<?php if(isset($_POST["sixth_form"])) 
{
	$i=1;
	$conn=mysqli_connect($DB_HOST,$DB_USER,$DB_PASSWORD,$DB_NAME) or die('could not connect');
	$status='connected ';
	$sqlquery = "Use sample;";
	if ($conn->query($sqlquery) === FALSE) echo "Error selecting to database: " . $conn->error;
	$sqlquery='select name from faculty_details f,(select t.id_count, id from (select COUNT(id) id_count,id from current_projects  GROUP BY id order by COUNT(id) DESC) t limit 1) m where f.id=m.id;';
	$queryresult=$conn->query($sqlquery);
	if ($queryresult->num_rows > 0) 
	{
   	while($row = $queryresult->fetch_assoc())
		{
      	$sixth_form_result=$sixth_form_result.'<font color="red">'.$i.'</font>'.'  '.$row['name'].'   ';
      	$i=$i+1;
      }	
	} 
	else 
    	$sixth_form_result=" None";
   $conn->close();
}
?>
<form action="index.php"  method="post" >
<br>6 : Get Faculty has the maximum number of projects
<input type="submit" name="sixth_form" value="Get Result"/> Result : <?php echo($sixth_form_result); ?>
</form>
<form action="index.php"  method="post" >
<br>7 : Enter User Defined query:
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>Schema For Tables:</b>
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;faculty_details(id,name,designation,email,phone_no,website,research)
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;publications(id,title,year)
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;responsibilities(id,responsibilty)
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;current_projects(id,title,sponsoring_agency)
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;group_members(id,name,area_of_research)
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;awards_accolades(id,title,year)
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="text" name="name7" maxlength="200"size="100" value=<?php echo ('"'.$_POST['name7'].'"'); ?> />
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="submit" name="seventh_form" value="Get Result"/>
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Result :<br>
<?php if(isset($_POST["seventh_form"])) 
{
	$conn=mysqli_connect($DB_HOST,$DB_USER,$DB_PASSWORD,$DB_NAME) or die('could not connect');
	$status='connected ';
	$sqlquery = "Use sample;";
	if ($conn->query($sqlquery) === FALSE) echo "Error selecting to database: " . $conn->error;
	$sqlquery=$_POST['name7'];
	$result=$conn->query($sqlquery);
	if ($result->num_rows > 0) 
	{
		echo '<table border="1"><tr>';
		while ($property = mysqli_fetch_field($result)) 
		{
    		echo '<td>'.$property->name.'</td>';
		}
		echo "</tr>\n";
		while($row = mysqli_fetch_row($result))
		{
    		echo "<tr>";
    		foreach($row as $cell)
        		echo '<td>'.$cell.'</td>';
        	echo "</tr>\n";
		}
		echo '</table>';
	}
	else 
    	echo " None";
   $conn->close();
}
?>
</form>
</body>
</html>





