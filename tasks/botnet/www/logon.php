<?php
    session_start();
    header('Content-Type: text/html; charset=utf-8');
    setlocale(LC_ALL,'ru_RU.65001','rus_RUS.65001','Russian_Russia.65001','russian');
    if (isset($_POST['login'])) { $login = $_POST['login']; if ($login == '') { unset($login);} }
    if (isset($_POST['password'])) { $password=$_POST['password']; if ($password =='') { unset($password);} }
    if (empty($login) or empty($password)) {
        exit ("<body><div align='center'><br/><br/><br/><h3>Вы ввели не всю информацию, вернитесь назад и заполните все поля!" . "<a href='index.php'> <b>Назад</b> </a></h3></div></body>");
    }
    $login = stripslashes($login);
    $login = htmlspecialchars($login);
    $password = stripslashes($password);
    $password = htmlspecialchars($password);
    $login = trim($login);
    $password = trim($password);
    include 'dbauth.php';

    /* Здесь SQL-инъекция, которой нужно воспользоваться, чтобы войти */
    $result = mysql_query("SELECT * FROM users WHERE name='$login'", $dbcon);
    $myrow = mysql_fetch_array($result);
    if (empty($myrow["pass"])) {
        exit ("<body><div align='center'><br/><br/><br/>
	<h3>Извините, введённый вами логин или пароль неверный." . "<a href='index.php'> <b>Назад</b> </a></h3>".mysql_error()."</div></body>");
    }
    else {
        if ($myrow["pass"] == $password) {
            $_SESSION['login'] = $myrow["name"]; 
            $_SESSION['id'] = $myrow["id"];
            header("Location:index.php");
        } else {
            exit ("<body><div align='center'><br/><br/><br/>
	    <h3>Извините, введённый вами логин или пароль неверный." . "<a href='index.php'> <b>Назад</b> </a></h3>".mysql_error()."</div></body>");
        }
    }
?>
