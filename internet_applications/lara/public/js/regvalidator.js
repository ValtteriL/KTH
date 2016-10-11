$(function() {
    $("#kek").validate({
   
        rules: {
            name: "required",
            email: {
                required: true,
                email: true
            },
            password: {
                required: true,
                minlength: 6
            },
            password_confirm: {
                required: true,
                minlength: 6,
                equalTo: "#password"
            }
        },

        messages: {
            name: "Please enter your name",
            email: "Please enter a valid email",
            password: {
                required: "Please enter a password",
                minlength: "Password must be atleast 6 characters"
            },
            password_confirmation: {
                required: "Please enter your password again",
                minlength: "Password must be atleast 6 characters",
                equalTo: "The passwords don't match"
            }
        },

        submitHandler: function(form) {
            form.submit();
        }
    });
});
