<?php

namespace App\Http\Controllers;
use Illuminate\Http\Request;
use App\Http\Requests;
use Illuminate\Support\Facades\Auth;
use App\Comment;
use Illuminate\Support\Facades\Input;

class CommentController extends Controller
{
    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index($id)
    {
        return \Response::json(Comment::where('recipe', $id)->get());
    }

    /**
     * Store a newly created resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @return \Illuminate\Http\Response
     */
    public function store(Request $request)
    {
        //return $request->all();

        // If the user is logged in, accept the comment
        if(Auth::check()) {
            $newcomment = new Comment;

            $newcomment->comment = $request->input('comment');
            $newcomment->user = Auth::user()->id;
            $newcomment->username = Auth::user()->name;
            $newcomment->recipe = $request->input('recipe');
            $newcomment->save();

            return \Response::json(['success' => true]);

        } else {
            return \Response::json(['success' => false]);
        }
    }

    /**
     * Remove the specified resource from storage.
     *
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function destroy($id)
    {
        Comment::where('id', $id)->where('user', Auth::user()->id)->delete();

        return \Response::json(['success' => true]);
    }
}
