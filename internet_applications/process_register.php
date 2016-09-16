<?php
include_once 'includes/db-connect.php';
 
$error_msg = "";
 
if (isset($_POST['username'], $_POST['p'])) {
    // Sanitize and validate the data passed in
    $username = filter_input(INPUT_POST, 'username', FILTER_SANITIZE_STRING);
    $password = filter_input(INPUT_POST, 'p', FILTER_SANITIZE_STRING);

    if (strlen($password) != 128) {
        // The hashed pwd should be 128 characters long.
        // If it's not, something really odd has happened
        $error_msg .= '<p class="error">Invalid password configuration.</p>';
    }
 
    // check existing username
    $prep_stmt = "SELECT id FROM users WHERE username = ? LIMIT 1";
    $stmt = $mysqli->prepare($prep_stmt);
 
    if ($stmt) {
        $stmt->bind_param('s', $username);
        $stmt->execute();
        $stmt->store_result();
 
                if ($stmt->num_rows == 1) {
                        // A user with this username already exists
                        $error_msg .= '<p class="error">A user with this username already exists</p>';
                        $stmt->close();
                }
        } else {
                $error_msg .= '<p class="error">Database error line 55</p>';
        }
 

    if (empty($error_msg)) {
 
        // Create hashed password using the password_hash function.
        // This function salts it with a random salt and can be verified with
        // the password_verify function.
        $password = password_hash($password, PASSWORD_BCRYPT);
 
        // Insert the new user into the database 
        if ($insert_stmt = $mysqli->prepare("INSERT INTO users (username, password) VALUES (?, ?)")) {
            $insert_stmt->bind_param('ss', $username, $password);
            if ($insert_stmt->execute()) {
                header('Location: ./register_success.php');
            } else {
                echo 'Error inserting to database'. $mysqli->error;
            }
        } else {
            echo 'Error preparing database query';
        }
    } else {
        echo $error_msg;
    }
} else {
    echo 'Did not get da parameters...';
}
?>
