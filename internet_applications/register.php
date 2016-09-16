<?php
include_once 'includes/db-connect.php';
include_once 'includes/functions.php';

session_start(); 
?>

<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" type="text/css" href="css/reset.css"> <!-- reset.css -->
        <link rel="stylesheet" type="text/css" href="css/style.css"> <!-- main stylesheet -->
        <link rel="stylesheet" type="text/css" href="css/login.css"> <!-- login stylesheet -->
        <script type="text/JavaScript" src="js/sha512.js"></script> <!-- SHA512 javascript --> 
        <script type="text/JavaScript" src="js/forms.js"></script> <!-- form submit function -->
        <title>Register | Tasty Recipes</title>
    </head>
    <body>
        <!-- Header -->
        <header>
            <h1>Tasty Recipes</h1>
            <p>Welcome to the tastiest site you'll ever visit</p>
        </header>

        <!-- Menu -->
        <label for="show-menu" class="show-menu">Show Menu</label>
        <input type="checkbox" id="show-menu" role="button">
        <div id="menu">
            <ul>
                <li><a href="index.php">Home</a></li>
                <li><a href="calendar.php">Calendar</a></li>
                <li class="dropdown">
                    <a href="#" class="dropbtn">Recipes</a>
                    <div class="dropdown-content">
                        <a href="recipes/meatballs.php">Meatballs</a>
                        <a href="recipes/pancakes.php">Pancakes</a>
                    </div>
                </li>
                <?php if (login_check($mysqli) == true) : ?>
                <li class="login"><a href="#">Logged in as <?php echo htmlentities($_SESSION['username']); ?></a></li>
                <li class="login"><a href="logout.php">Log out</a></li>
                <?php else : ?>
                <li class="login"><a href="login.php">Log in</a></li>
                <li class="active login"><a href="register.php">Register</a></li>
                <?php endif; ?>
            </ul>
        </div>

        <!-- Middle section -->
        <div id="login">
            <form action="process_register.php" method="post" name="login_form">
                <label for="username">Username</label>
                <input type="text" name="username"/>
                <label for="password">Password</label>
                <input type="password" name="password" id="password"/>
                <label for="passwordagain">Confirm password</label>
                <input type="password" name="passwordagain" id="passwordagain"/>
                <input type="button" value="login" onclick="regformhash(this.form, form.username, form.password, form.passwordagain)"/>
            </form>
        </div>

        <!-- Footer -->
        <footer>
            <p>(R) Tasty recipes</p>
            <p>Contact: admin@tastyrecipes.com</p>
        </footer>

    </body>
</html>
