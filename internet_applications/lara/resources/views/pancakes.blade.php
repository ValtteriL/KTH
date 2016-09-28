@extends('layouts.tastytemplate')
@section('title')
    <title> {{ $xml->recipe[0]->title }} | Tasy Recipes</title>
@stop
@section('additional_head')
    {{ Html::style("css/recipe.css") }} <!-- external reset.css -->
@section('middle_section')
@if (isset($xml))
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
                        if (false) { 
                            printcomments(2, htmlentities($_SESSION['username']), $mysqli, "pancakes.php");
                        } else {
                            //printcomments(2, NULL, $mysqli, "meatballs.php");
                        }
                    ?>
 
                    <div class="commentform">
                    <?php if (false) : ?>
                        <form id="usrform" action="pancakes.php" method="POST">
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
                    <?php foreach($xml->recipe[0]->nutrition->li as $ingredient) { echo '<li>' . $ingredient . '</li>'; } ?>
                </div>
            </div>
        </div>
@else
    <h3>Something went wrong!</h3>
@endif
@stop    

