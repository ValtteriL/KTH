<?php
include_once '../includes/db-connect.php';
include_once '../includes/functions.php';

session_start();

/* if the user is logged in and tries to delete comments or write one */
if (login_check($mysqli) == true) { 
    $username = htmlentities($_SESSION['username']);
    
    if(isset($_POST['comment_id'])) {
        $comment_id = $_POST['comment_id'];

        if ($stmt = $mysqli->prepare("DELETE FROM comments WHERE id = ? AND username = ?")) 
        {
            $stmt->bind_param('is', $comment_id, $username);
            $stmt->execute();
        
            if($stmt->affected_rows > 0) {
                /* comment deleted */
            } else {
                echo '<div class="err">Could not delete comment. Are you the author?</div>';
            }
        } else {
            echo '<div class="err">Mysql Error: ' . $mysqli->error . '</div>'; /* for debugging */
        }
    }
    else if(isset($_POST['comment'])) {
        $comment = $_POST['comment'];
        $recipe_id = 1;

        if ($stmt = $mysqli->prepare("INSERT INTO comments (recipeid, username, comment) VALUES(?, ?, ?)")) 
        {
            $stmt->bind_param('iss', $recipe_id, $username, $comment);
            $stmt->execute();
        
            if($stmt->affected_rows > 0) {
                /* comment added */
            } else {
                echo '<div class="err">Could not insert comment</div>';
            }
        } else {
            echo '<div class="err">Mysql Error: ' . $mysqli->error . '</div>'; /* for debugging */
        }
    }
}

/* Load the XML */
if (file_exists('meatballs.xml')) {
    $xml = simplexml_load_file('meatballs.xml');
} else {
    die('Failed to open the recipe file.');
}

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
                <li><a href="../calendar.php">Calendar</a></li>
                <li class="dropdown">
                    <a href="#" class="dropbtn">Recipes</a>
                    <div class="dropdown-content">
                        <a class="active" href="meatballs.php">Meatballs</a>
                        <a href="pancakes.php">Pancakes</a>
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
        <div class="section group">
            <div class="col span_1_of_2"> <!-- Left div -->
                <!-- Food name -->
                <div id="foodname" class="subdiv">
                <h1><?php echo $xml->recipe[0]->title;?></h1>
                </div>
                <!-- Ingredients -->
                <div id="ingredients" class="subdiv">
                    <h2>Ingredients</h2>
                    <ul>
                        <?php foreach($xml->recipe[0]->ingredient->li as $ingredient) { echo '<li>' . $ingredient . '</li>'; } ?>
                    </ul>
                </div>
                <!-- Directions -->
                <div id="directions" class="subdiv">
                    <h2>Directions</h2>
                    <ol>
                        <?php foreach($xml->recipe[0]->recipetext->li as $ingredient) { echo '<li>' . $ingredient . '</li>'; } ?>
                    </ol>
                </div>
                <!-- Comments -->
                <div id="comments" class="subdiv">
                    <h2>Comments</h2>
                    <?php
                        if (login_check($mysqli) == true) { 
                            printcomments(1, htmlentities($_SESSION['username']), $mysqli, "meatballs.php");
                        } else {
                            printcomments(1, NULL, $mysqli, "meatballs.php");
                        }
                    ?>
 
                    <div class="commentform">
                    <?php if (login_check($mysqli) == true) : ?>
                        <form id="usrform" action="meatballs.php" method="POST">
                            <label for="comment">Comment:</label>
                            <textarea name="comment" form="usrform"></textarea><br>
                            <input type=submit value="Post comment"></input>
                        </form>
                    <?php else : ?>
                        <h3>Log in to comment</h3>
                    <?php endif; ?>
                    </div>
                </div>
            </div>
            <div class="col span_1_of_2"> <!-- Right div -->
                <!-- Picture -->
                <div id="picture" class="subdiv">
                <img src="<?php echo $xml->recipe[0]->imagepath;?>" alt="<?php echo $xml->recipe[0]->imagepath;?>">
                </div>
                <!-- Nutritional facts -->
                <div id="nutrifacts" class="subdiv">
                    <h2>Nutritional facts</h2>
                    <ul>
                    <?php foreach($xml->recipe[0]->nutrition->li as $ingredient) { echo '<li>' . $ingredient . '</li>'; } ?>
                    </ul>
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
