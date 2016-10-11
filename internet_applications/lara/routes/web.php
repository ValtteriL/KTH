<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| This file is where you may define all of the routes that are handled
| by your application. Just tell Laravel the URIs it should respond
| to using a Closure or controller method. Build something great!
|
*/

Route::get('/', function() {
    return view('index');
});

Route::get('/calendar', function() {
    return view('calendar');
});

Route::get('/recipes/{recipe}', 'RecipeController@showRecipe');

Route::post('/recipes/{recipe}', 'RecipeController@commentOperation')->name('commentroute');


// auth and home
Auth::routes();

Route::get('/home', 'HomeController@index');

// API
Route::get('/api/comments/{recipeid}', 'CommentController@index');
Route::post('/api/comments', 'CommentController@store');
Route::delete('/api/comments/{commentid}', 'CommentController@destroy');
