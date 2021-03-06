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
        <link rel="stylesheet" type="text/css" href="css/calendar.css"> <!-- calendar style -->
        <title>Calendar | Tasty Recipes</title>
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
                <li><a class="active" href="calendar.php">Calendar</a></li>
                <li class="dropdown">
                    <a href="#" class="dropbtn">Recipes</a>
                    <div class="dropdown-content">
                        <a href="recipes/meatballs.php">Meatballs</a>
                        <a href="recipes/pancakes.php">Pancakes</a>
                    </div>
                </li>
                <?php if (login_check($mysqli) == true) : ?>
                <li class="login"><a href="#">Logged in as <?php echo htmlentities($_SESSION['username']); ?></a></li>
                <li class="login"><a href="../logout.php">Log out</a></li>
                <?php else : ?>
                <li class="login"><a href="../login.php">Log in</a></li>
                <li class="login"><a href="../register.php">Register</a></li>
                <?php endif; ?>
            </ul>
        </div>

        <!-- Middle section -->
        <div id="calendar">
            <div class="month">
                <ul>
                    <li class="prev">prev</li>
                    <li class="next">next</li>
                    <li style="text-align:center">September</li>
                </ul>
            </div>
            <ul class="weekdays">
                <li>Mo</li>
                <li>Tu</li>
                <li>We</li>
                <li>Th</li>
                <li>Fr</li>
                <li>Sa</li>
                <li>Su</li>
            </ul>
            <ul class="days">
                <li></li>
                <li></li>
                <li></li>
                <li>1</li>
                <li>2</li>
                <li>3</li>
                <li>4</li>
                <li>5</li>
                <li>6</li>
                <li>7</li>
                <li>8</li>
                <li>9</li>
                <li>10</li>
                <li>11</li>
                <li>12</li>
                <li><p>13</p><a href="recipes/meatballs.php"><img src="images/meatballs.png" alt="Meatballs image"></a></li> <!-- Use p tag so the number will stay on top of the image -->
                <li>14</li>
                <li>15</li>
                <li>16</li>
                <li>17</li>
                <li>18</li>
                <li>19</li>
                <li>20</li>
                <li>21</li>
                <li>22</li>
                <li>23</li>
                <li>24</li>
                <li>25</li>
                <li><p>26</p><a href="recipes/pancakes.php"><img src="images/pancakes.png" alt="Pancakes image"></a></li>
                <li>27</li>
                <li>28</li>
                <li>29</li>
                <li>30</li>
            </ul>

        </div>

        <!-- Footer -->
        <footer>
            <p>(R) Tasty recipes</p>
            <p>Contact: admin@tastyrecipes.com</p>
        </footer>

    </body>
</html>
