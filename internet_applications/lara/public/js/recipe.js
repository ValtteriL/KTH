// Comment application
var commentingapp = angular.module('commentApp', []);

// Use different kind of markers because of Laravel
commentingapp.config(function($interpolateProvider){
    $interpolateProvider.startSymbol('{[{').endSymbol('}]}');
});

// Service for ajax calls
commentingapp.factory('CommentService', function($http) {
    return {

        fetch : function(id) {
            return $http.get('/api/comments/' + id);
        },

        save : function(commentData) {
            return $http({
                method: 'POST',
                url : '/api/comments',
                headers: {'Content-Type' : 'application/x-www-form-urlencoded'},
                data: $.param(commentData)
            });
        },

        delete : function(id) {
            return $http.delete('/api/comments/' + id);
        }
    }
});

// Controller
commentingapp.controller('commentsController', function($window, $http, CommentService) {
    var cctrl = this;

    // get the recipe id
    cctrl.recipeId = $window._recipeId;
    
    // data to be sent when comment is sumbitted
    cctrl.formdata = {
        'recipe' : cctrl.recipeId
    };

    // first fetch comments
    CommentService.fetch(cctrl.recipeId)
        .success(function(data) {
            cctrl.commentData = data;
        });

    // post new comment
    cctrl.postComment = function() {

        CommentService.save(cctrl.formdata)
            .success(function(data) {
                // refresh comments
                CommentService.fetch(cctrl.recipeId)
                    .success(function(data) {
                        cctrl.commentData = data;
                    });
                cctrl.formdata.comment = '';
            })

            .error(function(data) {
                console.log(data);
            });
    };

    // delete comment
    cctrl.deleteComment = function(commentId) {
        CommentService.delete(commentId)
            .success(function(data) {
                // refresh comments
                CommentService.fetch(cctrl.recipeId)
                    .success(function(data) {
                        cctrl.commentData = data;
                    });
            });
    };
});
