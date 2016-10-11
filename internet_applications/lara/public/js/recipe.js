var commentingapp = angular.module('commentApp', []);

commentingapp.config(function($interpolateProvider){
    $interpolateProvider.startSymbol('{[{').endSymbol('}]}');
});


commentingapp.controller('commentsController', function($http) {
    var cctrl = this;

    // first fetch comments
    $http.get('/api/comments/' + cctrl.recipeId).success( function(response) {
        cctrl.commentData = response;
    });

    // function to fetch comments
    cctrl.fetchComments = function() {
       $http.get('/api/comments/' + cctrl.recipeId).success( function(response) {
           cctrl.commentData = response;
       }); 
    };

    // post new comment
    cctrl.postComment = function() {
        cctrl.comment = '';
        cctrl.fetchComments(); // refresh comments
    };

    // delete comment
    cctrl.deleteComment = function(commentId) {
        $http.delete('/api/comments/' + commentId);
        cctrl.fetchComments(); // refresh comments
    };
});
