<?php
include_once 'includes/db-connect.php';

if(isset($_POST['comment_id'])) {

    $comment_id = $_POST['comment_id'];

    /* TODO make sure this user has written this comment */

    if(!isset($mysqli)) {
        echo 'EI HELVETTI OLE MYSQLIÄ';
    }

    if ($stmt = $mysqli->prepare("DELETE FROM comments WHERE id = ?")) 
    {
        $stmt->bind_param('i', $comment_id);
        $stmt->execute();
        
        if($stmt->affected_rows > 0) {
            echo 'Comment deleted';
            // REDIR to the page
        } else {
            echo 'Could not delete comment';
        }
    }
    else
    {
	    echo 'Error preparing SQL statement..'; /* No user exists */
        die ("Mysql Error: " . $mysqli->error); /* for debugging */
	}

} 
