@extends('layouts.tastytemplate')
@section('title')
    <title>{{ $xml->recipe[0]->title }} | Tasty Recipes</title>
@stop
@section('additional_head')
    {{ Html::style("css/recipe.css") }} 
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
    <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.5.7/angular.min.js"></script>
    {{ Html::script('js/recipe.js') }}
@stop
@section('middle_section')
@if (isset($xml))
        <!-- Middle section -->
        <div class="section group">
            <div class="col span_1_of_2"> <!-- Left div -->
                <!-- Food name -->
                <div id="foodname" class="subdiv">
                <h1>{{ $xml->recipe[0]->title }}</h1>
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
                <div id="comments" class="subdiv" ng-app="commentApp" ng-controller="commentsController as cctrl">
                    <h2>Comments</h2>
                        <div ng-repeat="comment in cctrl.commentData" class="comment">
                            <h3>{[{comment.username}]}</h3>
                            <p>{[{comment.comment}]}</p> 
                            @if(Auth::check())
                                <form ng-if="comment.username == {{Auth::user()->name}}" ng-submit="cctrl.deleteComment(comment.id)"> <!-- check if this user is author! -->
                                <input type="hidden" value="{[{comment.id}]}">
                                <input type="submit" value="Delete">
                                </form>
                            @endif
                        </div>
                    <div class="commentform">
                    @if (Auth::check())
                        <form ng-submit="cctrl.postComment()">
                            <input type="text" placeholder="Comment" ng-model="cctrl.formdata.comment">
                            <input type="submit" value="Post comment">
                        </form>
                    @else
                        <h3>Log in to comment</h3>
                    @endif
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
                    <ol>
                        <?php foreach($xml->recipe[0]->nutrition->li as $ingredient) { echo '<li>' . $ingredient . '</li>'; } ?>
                    </ol>
                </div>
            </div>
        </div>
@else
    <h3>Something went wrong!</h3>
@endif
@stop    

