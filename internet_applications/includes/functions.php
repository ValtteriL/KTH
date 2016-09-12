<?php
include_once 'db-config.php';

function login($username, $password, $mysqli) {
    // Using prepared statements means that SQL injection is not possible. 
    if ($stmt = $mysqli->prepare("SELECT id, password FROM users WHERE username = ? LIMIT 1")) 
    {
        $stmt->bind_param('s', $username);
        $stmt->execute();
        $stmt->store_result();
						        
        $stmt->bind_result($user_id, $db_password);
        $stmt->fetch();
    
        if ($stmt->num_rows == 1)
        {
            if (password_verify($password, $db_password))
            {
                $user_browser = $_SERVER['HTTP_USER_AGENT']; /* Get the user-agent for secure login string */
                $_SESSION['user_id'] = $user_id;
                $_SESSION['username'] = $username;
                $_SESSION['login_string'] = hash('sha512', $db_password . $user_browser);
                return true; /* Login succesful */
            }
            else
            {
                return false; /* Invalid password */
            }
        }
    }
    else
    {
	    return false; /* No user exists */
	}
}

function login_check($mysqli) {
    // check if all session variables are set
    if(isset($_SESSION['user_id'], $_SESSION['username'], $_SESSION['login_string']))
    {
        $user_id = $_SESSION['user_id'];
        $login_string = $_SESSION['login_string'];
        $username = $_SESSION['username'];
        $user_browser = $_SERVER['HTTP_USER_AGENT'];

        if($stmt = $mysqli->prepare("SELECT password FROM users WHERE id = ? LIMIT 1"))
        {
            $stmt->bind_param('i', $user_id);
            $stmt->execute();
            $stmt->store_result();
            
            if($stmt->num_rows == 1)
            {
                $stmt->bind_result($password);
                $stmt->fetch();
                $login_check = hash('sha512', $password . $user_browser);

                if(hash_equals($login_check, $login_string))
                {
                    return true; // logged in
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

function printcomments($id, $username, $mysqli)
{
    if ($stmt = $mysqli->prepare("SELECT comment, username FROM comments WHERE recipeid = ?")) 
    {
        $stmt->bind_param('i', $id);
        $stmt->execute();
        $stmt->bind_result($comment, $author);
    
        while ($stmt->fetch())
        {
            if (true) /* if this comment was written by this user */
            {
                echo 'comment = ' . $comment . ', author = ' . $author . '<br>';
            }
        }
    }
    else
    {
	    echo 'Could not fetch comments'; /* No user exists */
	}

}
