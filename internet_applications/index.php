<?php
include_once 'includes/db-connect.php';
include_once 'includes/functions.php';

session_start(); 
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8"> <!-- define character set -->
        <link rel="stylesheet" type="text/css" href="css/reset.css"> <!-- external reset.css -->
        <link rel="stylesheet" type="text/css" href="css/style.css"> <!-- external stylesheet -->
        <title>Tasty Recipes | best recipes in the web</title>
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
                <li><a class="active" href="index.php">Home</a></li>
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
                <li class="login"><a href="register.php">Register</a></li>
                <?php endif; ?>

            </ul>
        </div>

        <!-- Middle section -->
        <div class="section group">
            <div class="col span_1_of_2"> <!-- Left div -->
                <p>Welcome to the website of Tasty Recipes, the maker of awesome and most delightful and delicious little treats that no one can resist. Here you can browse our recipes or try our stylish calender that will show you what kind of food we're teaching you to prepare.<br><br>It's a great pleasure to have you here!</p>
            </div>
            <div class="col span_1_of_2"> <!-- Right div -->
                <div id="promo">
                    <h3>Check out our awesome <a href="calendar.php">calendar</a>!!</h3>
                </div>
            </div>
        </div>

        <!-- Footer -->
        <footer>
            <p>(R) Tasty recipes</p>
            <p>Contact: admin@tastyrecipes.com</p>
        </footer>
    </body>
</html>
