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
    public function index()
    {
        return \Response::json(Comment::get());
    }

    /**
     * Store a newly created resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @return \Illuminate\Http\Response
     */
    public function store(Request $request)
    {
        return $request->all();

        // If the user is logged in, accept the comment
        if(Auth::check()) {
            Comment::create([
                'recipe' => Input::get('recipe'),
                'user' => Auth::user()->id,
                'comment' => Input::get('comment'),
                'username' => Auth::user()->name
            ]);

            return Response::json(['success' => true]);

        } else {
            return Response::json(['success' => false]);
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
        Comment::destroy($id);

        return Response::json(['success' => true]);
    }
}
