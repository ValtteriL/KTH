<?php
include_once '../includes/db-connect.php';
include_once '../includes/functions.php';

session_start(); 
?>

<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" type="text/css" href="../css/reset.css"> <!-- reset.css -->
        <link rel="stylesheet" type="text/css" href="../css/style.css"> <!-- site theme -->
        <link rel="stylesheet" type="text/css" href="../css/recipe.css"> <!-- recipe.css -->
        <title>Meatballs | Tasty recipes</title>
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
                <li><a href="../index.php">Home</a></li>
                <li><a href="../calendar.html">Calendar</a></li>
                <li class="dropdown">
                    <a href="#" class="dropbtn">Recipes</a>
                    <div class="dropdown-content">
                        <a class="active" href="meatballs.php">Meatballs</a>
                        <a href="pancakes.php">Pancakes</a>
                    </div>
                </li>
                <li><a href="login.php">Log in</a></li>
            </ul>
        </div>

        <!-- Middle section -->
        <div class="section group">
            <div class="col span_1_of_2"> <!-- Left div -->
                <!-- Food name -->
                <div id="foodname" class="subdiv">
                    <h1>Meatballs</h1>
                </div>
                <!-- Ingredients -->
                <div id="ingredients" class="subdiv">
                    <h2>Ingredients</h2>
                    <ul>
                        <li>1lb ground beef</li>
                        <li>1 large egg</li>
                        <li>1/4 cup onion</li>
                        <li>1/3 cup italian seasoned breadcrumbs</li>
                        <li>1/4 cup milk</li>
                        <li>1 teaspoon Worcestershire sauce</li>
                        <li>salt, to taste</li>
                        <li>fresh ground black pepper, to taste</li>
                    </ul>
                </div>
                <!-- Directions -->
                <div id="directions" class="subdiv">
                    <h2>Directions</h2>
                    <ol>
                        <li>Mix the sirloin, egg, milk, salt/pepper, and the Worcestershire sauce.</li>
                        <li>Add as much of the bread crumbs as you need (mixture should be slightly moist).</li>
                        <li>Roll into desired sized balls.</li>
                        <li>Preheat oven to 375F.</li>
                        <li>Plane onto a cookie sheet or 13x9 baking dish.</li>
                        <li>Bake meatballs for 15-23minutes or until no pink remains.</li>
                        <li>Drain onto papel towels.</li>
                    </ol>
                </div>
                <!-- Comments -->
                <div id="comments" class="subdiv">
                    <h2>Comments</h2>
                    <?php
                        printcomments(1, htmlentities($_SESSION['username']), $mysqli);
                    ?>
                    <div class="comment">
                        <h3>Anna</h3>
                        <p>Love your site!</p>
                    </div>
                    <div class="comment">
                        <h3>Lisa</h3>
                        <p>I am real please click on this link to earn 1000€ a day from home!!!! www.scam.com</p>
                    </div>
 
                    <div class="commentform">
                        <form id="usrform" action="../postcomment.php" method="POST">
                            <label for="name">Name:</label>
                            <input type=text id="name"></input><br>
                            <label for="comment">Comment:</label>
                            <textarea name="comment" form="usrform"></textarea><br>
                            <input type=submit value="Post comment"></input>
                        </form>
                    </div>
                </div>
            </div>
            <div class="col span_1_of_2"> <!-- Right div -->
                <!-- Picture -->
                <div id="picture" class="subdiv">
                    <img src="../images/meatballs.png" alt="Meatballs picture">
                </div>
                <!-- Nutritional facts -->
                <div id="nutrifacts" class="subdiv">
                    <h2>Nutritional facts</h2>
                    <table>
                        <tr>
                            <th>Näringsvärde</th>
                            <th>/100g</th>
                        </tr>
                        <tr>
                            <th>Energi</th>
                            <th>1820kJ/433kcal</th>
                        </tr>
                        <tr>
                            <th>Protein</th>
                            <th>8,7g</th>
                        </tr>
                        <tr>
                            <th>Kolhydrater</th>
                            <th>66g</th>
                        </tr>
                        <tr>
                            <th>Fett</th>
                            <th>15g</th>
                        </tr>
                        <tr>
                            <th>Kostfiber</th>
                            <th>5,6g</th>
                        </tr>
                        <tr>
                            <th>Natrium</th>
                            <th>0,08g</th>
                        </tr>
                    </table>
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
