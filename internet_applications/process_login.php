<?php
include_once 'includes/db-connect.php'; /* database credentials */
include_once 'includes/functions.php'; /* functions */

session_start(); /* start session */

if(isset($_POST['username'], $_POST['p']))
{
    $username = $_POST['username'];
    $password = $_POST['p']; /* hashed password */

    if(login($username, $password, $mysqli) == true)
    {
        header('Location: index.php'); /* success */
    }
    else
    {
        header('Location: login.php?err=1'); /* failure */
    }
}
else
{
    echo 'Invalid Request'; /* No valid POST variables */
}
