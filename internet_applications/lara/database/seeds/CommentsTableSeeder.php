<?php
use App\User;
use Illuminate\Database\Seeder;
class CommentsTableSeeder extends Seeder {

    public function run() {
        DB::table('comments')->insert([
            'recipe' => 1,
            'comment' => 'Hello world!',
            'user' => 1,
        ]);
    }
}
