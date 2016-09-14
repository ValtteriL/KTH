<?php
include_once 'includes/functions.php';

session_start();

$_SESSION = array();
$params = session_get_cookie_params(); /* get session variables */

/* set the cookie expiration time into past */
setcookie(session_name(), '', time()-42000,
    $params["path"],
    $params["domain"],
    $params["secure"],
    $params["httponly"]);

/* destroy session */
session_destroy();

/* redirect user to index */
header('Location: index.php');
?>

