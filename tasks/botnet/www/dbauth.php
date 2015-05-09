<?php
    $dbcon = mysql_connect("localhost", "botnet", "32l9oopFduvkgEHaoxCE");
    mysql_select_db("botnet");
    if (! $dbcon) {
        echo "<p>Произошла ошибка при подсоединении к MySQL!</p>".mysql_error();
        exit();
    } else {
        if (! mysql_select_db("botnet", $dbcon)) {
            echo("<p>Выбранной базы данных не существует!</p>");
        }
    }
?>