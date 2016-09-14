<?php
include_once 'includes/db-connect.php';
include_once 'includes/functions.php';

if(isset($_POST['name'], $_POST['comment'])) {

    $comment = $_POST['comment'];
    $name = $_POST['name'];

    /* TODO: XSS protection + verify that this user is really who they tell they are */

    if ($stmt = $mysqli->prepare("INSERT INTO comments (recipeid, username, comment) = VALUES(?, ?, ?)")) 
    {
        $stmt->bind_param('iss', $recipe_id, $name, $comment);
        $stmt->execute();
        
        if($stmt->affected_rows > 0) {
            echo 'Comment added!';
            // REDIR to the page
        } else {
            echo 'Could not add comment :(';
        }
    }
    else
    {
	    echo 'Error preparing SQL statement..'; /* No user exists */
        die ("Mysql Error: " . $mysqli->error); /* for debugging */
	}

}
