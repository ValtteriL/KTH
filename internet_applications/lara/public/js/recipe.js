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
commentingapp.controller('commentsController', function($http, CommentService) {
    var cctrl = this;
    cctrl.recipeid = 1;
    cctrl.formdata = {
        'recipe' : 1
    };

    // first fetch comments
    CommentService.fetch(cctrl.recipeId)
        .success(function(data) {
            cctrl.commentData = data;
        });

    // post new comment
    cctrl.postComment = function() {
        console.log(cctrl.formdata);

        CommentService.save(cctrl.formdata)
            .success(function(data) {
                CommentService.fetch(cctrl.recipeId)
                    .success(function(data) {
                        cctrl.commentData = data;
                    });
                cctrl.comment = '';
            })

            .error(function(data) {
                console.log(data);
            });
    };

    // delete comment
    cctrl.deleteComment = function(commentId) {
        CommentService.destroy(commentId)
            .success(function(data) {
                CommentService.fetch(cctrl.recipeId)
                    .success(function(data) {
                        cctrl.comments = data;
                    });
            });
    };
});
