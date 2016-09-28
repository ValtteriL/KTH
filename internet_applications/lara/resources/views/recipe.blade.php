@extends('layouts.tastytemplate')
@section('title')
    <title>{{ $xml->recipe[0]->title }} | Tasty Recipes</title>
@stop
@section('additional_head')
    {{ Html::style("css/recipe.css") }} <!-- external reset.css -->
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
                <div id="comments" class="subdiv">
                    <h2>Comments</h2>
                    @if (Auth::check()) 
                        @foreach ($comments as $comment)
                            <div class="comment">
                            <h3>{{ $comment->username }}</h3>
                            <p>{{ $comment->comment }}</p>
                            @if ($comment->user == Auth::user()->id)
                                {{ Form::open(['url' => request()->path()]) }}
                                {{ Form::hidden('id', $comment->id) }}
                                <p>
                                {{ Form::submit('delete') }}</p>
                                {{ Form::close() }}
                            @endif
                            </div>
                        @endforeach
                    @else
                        @foreach ($comments as $comment)
                            <div class="comment">
                            <h3>{{ $comment->username }}</h3>
                            <p>{{ $comment->comment }}</p>
                            </div>
                        @endforeach
                    @endif
 
                    <div class="commentform">
                    @if (Auth::check())
                        {{ Form::open(['url' => request()->path()]) }}
                        <p>
                        {{ Form::label('comment', 'Comment') }}
                        {{ Form::text('comment') }}
                        </p>
                        <p>
                        {{ Form::submit('Post comment') }}</p>
                        {{ Form::close() }}
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

