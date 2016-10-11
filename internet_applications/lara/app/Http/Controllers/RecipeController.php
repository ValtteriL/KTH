<?php

namespace App\Http\Controllers;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\DB;
use Illuminate\Support\Facades\Auth;
use App\Comment;

class RecipeController extends Controller {
    public function showRecipe($recipe) {
        $filePath = resource_path() . '/recipes/' . $recipe . '.xml';
        $xml = $comments = NULL;

        // Load the XML
        if (file_exists($filePath)) {
            $xml = simplexml_load_file($filePath);

            // Which recipe is this?
            $recipeid = $xml->recipe[0]->id;

            // Get comments
            $comments = Comment::where('recipe', '=', $recipeid)->get();

        }
        // return the view
        return view('recipe', ['xml' => $xml, 'comments' => $comments, 'recipeid' => $recipeid]);
    }

    public function commentOperation(Request $request, $recipe) {
        $comment = $request->input('comment');
        

        // check that user is login
        if(Auth::check()) {

            $filePath = resource_path() . '/recipes/' . $recipe . '.xml';
            
            // read the recipe id
            if (file_exists($filePath)) {
                $xml = simplexml_load_file($filePath);

                // Which recipe is this?
                $recipeid = $xml->recipe[0]->id;

                // delete comment or write new?
                if($request->input('id') !== NULL) {
                    // delete the comment if written by this user
                    Comment::where('id', '=', $request->input('id'))->where('user', '=', Auth::user()->id)->delete();
                } else {
                    // put into db
                    $newcomment = new Comment;

                    $newcomment->comment = $comment;
                    $newcomment->user = Auth::user()->id;
                    $newcomment->username = Auth::user()->name;
                    $newcomment->recipe = $recipeid;

                    $newcomment->save();
                }
            }
        }
        // return view
        return $this->showRecipe($recipe);
    }
}
