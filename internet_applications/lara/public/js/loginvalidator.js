$(function() {
    $("#loginformi").validate({
   
        rules: {
            email: {
                required: true,
                email: true
            },
            password: {
                required: true,
                minlength: 6
            }
        },

        messages: {
            email: "Please enter a valid email",
            password: {
                required: "Please enter a password",
                minlength: "Password must be atleast 6 characters"
            }
        },

        submitHandler: function(form) {
            form.submit();
        }
    });
});
